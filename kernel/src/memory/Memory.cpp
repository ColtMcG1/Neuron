#include "Memory.h"
#include "../graphics/Graphics.h"

extern uint64_t _KernelStart;
extern uint64_t _KernelEnd;

PageFrameAllocator GlobalAllocator;

uint64_t GetMemorySize(EFI_MEMORY_DESCRIPTOR* mMap, uint64_t mMapSize, uint64_t mMapDescriptorSize)
{
    static uint64_t memorySize = 0;
    
    if(memorySize > 0)
        return memorySize;

    for (int i = 0; i < (mMapSize / mMapDescriptorSize); i++)
    {
        EFI_MEMORY_DESCRIPTOR* descriptor = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)mMap + i * mMapDescriptorSize);
        memorySize += descriptor->NumberOfPages * PAGE_SIZE;
    }

    return memorySize;
}

void memset(void *buffer, uint64_t size, uint8_t value)
{
    for (uint64_t i = 0; i < size; i++)
    {
        *(uint8_t *)((uint64_t)buffer + i) = value;
    }
}
void memcpy(void *dest, void *src, uint64_t size)
{
    uint8_t *d_Index = (uint8_t*)dest;
    uint8_t *s_Index = (uint8_t *)src;
    uint8_t *s_End = (uint8_t *)src + size;

    while(s_Index != s_End)
    {
        *(d_Index++) = *(s_Index++);
    }
}

void PageFrameAllocator::Initalize(EFI_MEMORY_DESCRIPTOR* map, size_t mapSize, size_t descriptorSize)
{
    if(m_Initalized)
        return;
    else
        m_Initalized = true;

    uint64_t mapEntries = mapSize / descriptorSize;
    
    void* largestPtr = nullptr;
    size_t lagestSize = 0;


    for(uint64_t i = 0; i < mapEntries; i++)
    {
        EFI_MEMORY_DESCRIPTOR* descriptor = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)map + (i * descriptorSize));

        if(descriptor->Type == EfiConventionalMemory)
        {
            if(descriptor->NumberOfPages * PAGE_SIZE > lagestSize)
            {
                largestPtr = descriptor->PhysicalAddress;
                lagestSize = descriptor->NumberOfPages * PAGE_SIZE;
            }
        }
    }

    uint64_t memorySize = GetMemorySize(map, mapEntries, descriptorSize);
    m_FreeMemory = memorySize;

    uint64_t bitmapSize = ((memorySize / PAGE_SIZE) / 8) + 1;

    InitalizeBitmap(largestPtr, bitmapSize);

    LockPages(largestPtr, (bitmapSize / PAGE_SIZE) + 1);

    
    for(int i = 0; i < mapEntries; i++)
    {
        EFI_MEMORY_DESCRIPTOR* descriptor = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)map + (i * descriptorSize));

        if(descriptor->Type != EfiConventionalMemory)
        {
            ReservePages(descriptor->PhysicalAddress, descriptor->NumberOfPages);
        }
    }


    uint64_t kernelSize = (uint64_t)&_KernelEnd - (uint64_t)&_KernelStart;
    uint64_t kernelPages = (uint64_t)(kernelSize / PAGE_SIZE) + 1;

    LockPages(&_KernelStart, kernelPages);

}
void PageFrameAllocator::InitalizeBitmap(void* bufferAddress, size_t bufferSize)
{
    m_Bitmap = Bitmap((uint8_t*)bufferAddress, bufferSize);
    memset(bufferAddress, bufferSize, 0);
}


void PageFrameAllocator::LockPage(void* address)
{
    uint64_t index = (uint64_t)address / PAGE_SIZE;

    if(m_Bitmap.Get(index))
    {
        return;
    }
    else
    {
        m_Bitmap.Set(index, true);
    }

    m_FreeMemory -= PAGE_SIZE;
    m_UsedMemory += PAGE_SIZE;
}
void PageFrameAllocator::FreePage(void* address)
{
    uint64_t index = (uint64_t)address / PAGE_SIZE;

    if(!m_Bitmap.Get(index))
    {
        return;
    }
    else
    {
        m_Bitmap.Set(index, false);
    }

    m_FreeMemory += PAGE_SIZE;
    m_UsedMemory -= PAGE_SIZE;
}

void PageFrameAllocator::LockPages(void* address, uint64_t page_count)
{
    for(uint64_t i = 0; i < page_count; i++)
    {
        LockPage((void*)((uint64_t)address + (i * PAGE_SIZE)));
    }
}
void PageFrameAllocator::FreePages(void* address, uint64_t page_count)
{
    for(uint64_t i = 0; i < page_count; i++)
    {
        FreePage((void*)((uint64_t)address + (i * PAGE_SIZE)));
    }
}

void PageFrameAllocator::ReservePage(void* address)
{
    uint64_t index = (uint64_t)address / PAGE_SIZE;

    if(m_Bitmap.Get(index))
    {
        return;
    }
    else
    {
        m_Bitmap.Set(index, true);
    }

    m_FreeMemory -= PAGE_SIZE;
    m_ReservedMemory += PAGE_SIZE;
}
void PageFrameAllocator::UnreservePage(void* address)
{
    uint64_t index = (uint64_t)address / PAGE_SIZE;

    if(!m_Bitmap.Get(index))
    {
        return;
    }
    else
    {
        m_Bitmap.Set(index, false);
    }

    m_FreeMemory += PAGE_SIZE;
    m_ReservedMemory -= PAGE_SIZE;
}

void PageFrameAllocator::ReservePages(void* address, uint64_t page_count)
{
    for(uint64_t i = 0; i < page_count; i++)
    {
        ReservePage((void*)((uint64_t)address + (i * PAGE_SIZE)));
    }
}
void PageFrameAllocator::UnreservePages(void* address, uint64_t page_count)
{
    for(uint64_t i = 0; i < page_count; i++)
    {
        FreePage((void*)((uint64_t)address + (i * PAGE_SIZE)));
    }
}

void* PageFrameAllocator::AllocatePage()
{
    for(uint64_t i = 0; i < m_Bitmap.Size() * 8; i++)
    {
        if(m_Bitmap.Get(i))
        {
            continue;
        }
        else 
        {
            void* location = (void*)(i * PAGE_SIZE);
            LockPage(location); 
            return location;
        }
    }

    return nullptr;
}

const uint64_t& PageFrameAllocator::GetFreeMemory() const
{
    return m_FreeMemory;
}
const uint64_t& PageFrameAllocator::GetUsedMemory() const
{
    return m_UsedMemory;
}
const uint64_t& PageFrameAllocator::GetReservedMemory() const
{
    return m_ReservedMemory;
}

PageMapIndexer::PageMapIndexer(uint64_t virtualAddress)
{
    virtualAddress >>= 12;
    m_P = virtualAddress & 0x1ff;
    virtualAddress >>= 9;
    m_PT = virtualAddress & 0x1ff;
    virtualAddress >>= 9;
    m_PD = virtualAddress & 0x1ff;
    virtualAddress >>= 9;
    m_PDP = virtualAddress & 0x1ff;
}

const uint64_t &PageMapIndexer::GetPDP() const
{
    return m_PDP;
}
const uint64_t &PageMapIndexer::GetPD() const
{
    return m_PD;
}
const uint64_t &PageMapIndexer::GetPT() const
{
    return m_PT;
}
const uint64_t &PageMapIndexer::GetP() const
{
    return m_P;
}

void PageTableManager::Initalize(PageTable *PML4Address)
{
    m_PML4Address = PML4Address;
}

void PageTableManager::MapMemory(void *virtualAddress, void *physicalAddress)
{
    PageMapIndexer indexer((uint64_t)virtualAddress);
    PageDirectoryEntry entry;

    entry = m_PML4Address->entries[indexer.GetPDP()];
    PageTable *pageTableLvl4;
    if(!entry.Present)
    {
        pageTableLvl4 = (PageTable *)GlobalAllocator.AllocatePage();
        memset(pageTableLvl4, PAGE_SIZE, 0);

        entry.Address = (uint64_t)pageTableLvl4 >> 12;
        entry.Present = true;
        entry.ReadWrite = true;

        m_PML4Address->entries[indexer.GetPDP()] = entry;
    }
    else
    {
        pageTableLvl4 = (PageTable *)((uint64_t)entry.Address << 12);
    }

    entry = pageTableLvl4->entries[indexer.GetPD()];
    PageTable *pageTableLvl3;
    if (!entry.Present)
    {
        pageTableLvl3 = (PageTable *)GlobalAllocator.AllocatePage();
        memset(pageTableLvl3, PAGE_SIZE, 0);

        entry.Address = (uint64_t)pageTableLvl3 >> 12;
        entry.Present = true;
        entry.ReadWrite = true;

        pageTableLvl4->entries[indexer.GetPD()] = entry;
    }
    else
    {
        pageTableLvl3 = (PageTable *)((uint64_t)entry.Address << 12);
    }

    entry = pageTableLvl3->entries[indexer.GetPT()];
    PageTable *pageTableLvl2;
    if (!entry.Present)
    {
        pageTableLvl2 = (PageTable *)GlobalAllocator.AllocatePage();
        memset(pageTableLvl2, PAGE_SIZE, 0);

        entry.Address = (uint64_t)pageTableLvl2 >> 12;
        entry.Present = true;
        entry.ReadWrite = true;

        pageTableLvl3->entries[indexer.GetPT()] = entry;
    }
    else
    {
        pageTableLvl2 = (PageTable *)((uint64_t)entry.Address << 12);
    }

    entry = pageTableLvl2->entries[indexer.GetP()];
    entry.Address = (uint64_t)physicalAddress >> 12;
    entry.Present = true;
    entry.ReadWrite = true;
    pageTableLvl2->entries[indexer.GetP()] = entry;
}

PageTable *&PageTableManager::GetPageTableAddress()
{
    return m_PML4Address;
}