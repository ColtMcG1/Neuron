#include <efi.h>
#include <efilib.h>
#include <elf.h>

typedef unsigned long long size_t;

typedef struct 
{
	void* BaseAddress;
	size_t BufferSize;
	unsigned int Width;
	unsigned int Height;
	unsigned int PixelsPerLine;
} Framebuffer;

typedef struct
{
	EFI_MEMORY_DESCRIPTOR* Map;
	UINTN MapSize;
	UINTN MapDescriptorSize;
} MemoryMap;


EFI_FILE* LoadFile(EFI_FILE* Directory, CHAR16* Path, EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable)
{
	EFI_FILE* LoadedFile;

	EFI_LOADED_IMAGE_PROTOCOL* LoadedImage;
	SystemTable->BootServices->HandleProtocol(ImageHandle, &gEfiLoadedImageProtocolGuid, (void**)&LoadedImage);

	EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* FileSystem;
	SystemTable->BootServices->HandleProtocol(LoadedImage->DeviceHandle, &gEfiSimpleFileSystemProtocolGuid, (void**)&FileSystem);

	if(Directory == NULL)
	{
		FileSystem->OpenVolume(FileSystem, &Directory);
	}

	EFI_STATUS status = Directory->Open(Directory, &LoadedFile, Path, EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY);

	if(status != EFI_SUCCESS)
	{
		return NULL;
	}
	else
	{
		return LoadedFile;
	}
}

#define PSF1_MAGIC0 0x36
#define PSF1_MAGIC1 0x04

typedef struct 
{
	unsigned char magic[2];
	unsigned char mode;
	unsigned char charSize;
} PSF1_HEADER;
typedef struct
{
	PSF1_HEADER* psf1_Header;
	void* glyphBuffer;
} PSF1_FONT;

PSF1_FONT* LoadPSF1Font(EFI_FILE* Directory, CHAR16* Path, EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable)
{
	EFI_FILE* font = LoadFile(Directory, Path, ImageHandle, SystemTable);

	if(font == NULL)
		return NULL;

	PSF1_HEADER* header;
	UINTN size = sizeof(PSF1_HEADER);

	SystemTable->BootServices->AllocatePool(EfiLoaderData, sizeof(PSF1_HEADER), (void**)&header);
	font->Read(font, &size, header);

	if(header->magic[0] != PSF1_MAGIC0 || header->magic[1] != PSF1_MAGIC1 )
	{
		return NULL;
	}

	UINTN glyphBufferSize = header->charSize * 256;

	if(header->mode == 1)
	{
		glyphBufferSize = header->charSize * 5112;
	}

	void* glyphBuffer;
	{
		font->SetPosition(font, sizeof(PSF1_HEADER));
		SystemTable->BootServices->AllocatePool(EfiLoaderData, glyphBufferSize, (void**)&glyphBuffer);
		font->Read(font, &glyphBufferSize, glyphBuffer);
	}

	PSF1_FONT* finishedFont;
	SystemTable->BootServices->AllocatePool(EfiLoaderData, sizeof(PSF1_FONT), (void**)&finishedFont);
	finishedFont->psf1_Header = header;
	finishedFont->glyphBuffer = glyphBuffer;

	return finishedFont;
}

Framebuffer LoadGOP()
{
	EFI_GUID gopGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
	EFI_GRAPHICS_OUTPUT_PROTOCOL* gop;
	EFI_STATUS status;
	status = uefi_call_wrapper(BS->LocateProtocol, 3, &gopGuid, NULL, (void**)&gop);

	if(EFI_ERROR(status))
	{
		Print(L"Unable to load GOP\n");
		//return;
	}

	Framebuffer framebuffer;

	framebuffer.BaseAddress = (void*)gop->Mode->FrameBufferBase;
	framebuffer.BufferSize = gop->Mode->FrameBufferSize;
	framebuffer.Width = gop->Mode->Info->HorizontalResolution;
	framebuffer.Height = gop->Mode->Info->VerticalResolution;
	framebuffer.PixelsPerLine = gop->Mode->Info->PixelsPerScanLine;

	return framebuffer;

}

EFI_STATUS efi_main (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
	
	InitializeLib(ImageHandle, SystemTable);

	EFI_FILE* Kernel;

	if((Kernel = LoadFile(NULL, L"kernel.elf", ImageHandle, SystemTable)) == NULL)
	{
		Print(L"Unable to load kernel\n");
		return -1;
	}

	Elf64_Ehdr header;
	{
		UINTN FileInfoSize;
		EFI_FILE_INFO* FileInfo;
		Kernel->GetInfo(Kernel, &gEfiFileInfoGuid, &FileInfoSize, NULL);
		SystemTable->BootServices->AllocatePool(EfiLoaderData, FileInfoSize, (void**)&FileInfo);
		Kernel->GetInfo(Kernel, &gEfiFileInfoGuid, &FileInfoSize, (void**)&FileInfo);

		UINTN size = sizeof(header);
		Kernel->Read(Kernel, &size, &header);
	}

	if( 
		CompareMem(&header.e_ident[EI_MAG0], ELFMAG, SELFMAG) != 0 || 
		header.e_ident[EI_CLASS] != ELFCLASS64 || 
		header.e_ident[EI_DATA] != ELFDATA2LSB ||
		header.e_type != ET_EXEC ||
		header.e_machine != EM_X86_64 ||
		header.e_version != EV_CURRENT)
	{
		Print(L"ERROR : BAD_KERNEL_FORMAT\n");
		return -1;
	}

	Elf64_Phdr* phdrs;
	{
		Kernel->SetPosition(Kernel, header.e_phoff);
		UINTN size = header.e_phnum * header.e_phentsize;
		SystemTable->BootServices->AllocatePool(EfiLoaderData, size, (void**)&phdrs);
		Kernel->Read(Kernel, &size, phdrs);
	}

	for(Elf64_Phdr* phdr = phdrs; (char*)phdr < (char*)phdrs + header.e_phnum * header.e_phentsize; phdr = (Elf64_Phdr*)((char*)phdr + header.e_phentsize))
	{
		switch (phdr->p_type)
		{
			case PT_LOAD:
			{
				int pages = (phdr->p_memsz + 0x1000 - 1) / 0x1000;
				Elf64_Addr segment = phdr->p_paddr;
				SystemTable->BootServices->AllocatePages(AllocateAddress, EfiLoaderData, pages, &segment);

				Kernel->SetPosition(Kernel, phdr->p_offset);
				UINTN size = phdr->p_filesz;
				Kernel->Read(Kernel, &size, (void*)segment);
				break;
			}
		}
	}

	Framebuffer framebuffer = LoadGOP();



	EFI_MEMORY_DESCRIPTOR* Map = NULL;
	UINTN MapSize, MapKey;
	UINTN DescriptorSize;
	UINT32 DescriptorVersion;
	{
		SystemTable->BootServices->GetMemoryMap(&MapSize, Map, &MapKey, &DescriptorSize, &DescriptorVersion);
		SystemTable->BootServices->AllocatePool(EfiLoaderData, MapSize, (void**)&Map);	
		SystemTable->BootServices->GetMemoryMap(&MapSize, Map, &MapKey, &DescriptorSize, &DescriptorVersion);
	}

	MemoryMap memoryMap;
	memoryMap.Map = Map;
	memoryMap.MapSize = MapSize;
	memoryMap.MapDescriptorSize = DescriptorSize;

	PSF1_FONT* font = LoadPSF1Font(NULL, L"zap-vga16.psf", ImageHandle, SystemTable);
	if(!font)
	{
		Print(L"Unable to loader font\n");
	}

	void (*KernelInitalize)(Framebuffer, MemoryMap, PSF1_FONT*) = ((__attribute__((sysv_abi)) void (*)(Framebuffer, MemoryMap, PSF1_FONT*) ) header.e_entry);

	SystemTable->BootServices->ExitBootServices(ImageHandle, MapKey);

	KernelInitalize(framebuffer, memoryMap, font);

	return EFI_SUCCESS; // Exit the UEFI application
}
