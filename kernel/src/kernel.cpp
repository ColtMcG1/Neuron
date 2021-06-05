#include "kernel.h"
#include "graphics/Graphics.h"

KernelData InitalizeMemory()
{
    KernelData kernelData;

    GlobalAllocator.Initalize(kernelData.GlobalMemoryMap->Map, kernelData.GlobalMemoryMap->MapSize, kernelData.GlobalMemoryMap->MapDescriptorSize);

    kernelData.GlobalPageTable = (PageTable *)GlobalAllocator.AllocatePage();
    memset(kernelData.GlobalPageTable, PAGE_SIZE, 0);

    kernelData.GlobalPageTableManager.Initalize(kernelData.GlobalPageTable);

    for (uint64_t i = 0; i < GetMemorySize(kernelData.GlobalMemoryMap->Map, kernelData.GlobalMemoryMap->MapSize, kernelData.GlobalMemoryMap->MapDescriptorSize); i += PAGE_SIZE)
    {
        kernelData.GlobalPageTableManager.MapMemory((void *)i, (void *)i);
    }

    print("Mapped memory\n");

    GlobalAllocator.LockPages((void *)GlobalConsole.Buffer->BaseAddress, GlobalConsole.Buffer->BufferSize / PAGE_SIZE + 1);
    print("Locked graphics buffer\n");

    uint64_t fb_Base = (uint64_t)GlobalConsole.Buffer->BaseAddress;
    uint64_t fb_Size = (uint64_t)GlobalConsole.Buffer->BufferSize + 0x1000;

    print("Location : ");
    print(fb_Base);
    print("\nSize   : ");
    print(fb_Size);
    print("\n");

    for (uint64_t i = fb_Base; i < fb_Base + fb_Size; i += 4096)
    {
        GlobalConsole.Clear(0);
        print(i);
        kernelData.GlobalPageTableManager.MapMemory((void *)i, (void *)i);

        for (uint64_t j = 0; j < 1000; j++)
            ;
    }

    print("Mapped graphics\n");

    asm("mov %0, %%cr3"
        :
        : "r"(kernelData.GlobalPageTable));
        

    return kernelData;
}

extern "C" void KernelInitalize(Framebuffer framebuffer, MemoryMap memoryMap, PSF1_FONT *font)
{
    GlobalConsole.Initalize(&framebuffer, font);
    print("Console Initalized\n");

    KernelData data = InitalizeMemory();

    print("Initalization finished\n");

    while(true)
        ;
}