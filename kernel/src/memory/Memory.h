#pragma once

#include "EfiMemory.h"
#include "Bitmap.h"

uint64_t GetMemorySize(EFI_MEMORY_DESCRIPTOR* mMap, uint64_t mMapEntries, uint64_t mMapDescriptorSize);

void memset(void *buffer, uint64_t size, uint8_t value);
void memcpy(void *dest, void *src, uint64_t size);

#define PAGE_SIZE 0x1000

class PageFrameAllocator
{

public:
    void Initalize(EFI_MEMORY_DESCRIPTOR* map, size_t mapSize, size_t descriptorSize);

    void LockPage(void* address);
    void FreePage(void* address);

    void* AllocatePage();
    
    void LockPages(void* address, uint64_t page_count);
    void FreePages(void* address, uint64_t page_count);

    const uint64_t& GetFreeMemory() const;
    const uint64_t& GetUsedMemory() const;
    const uint64_t& GetReservedMemory() const;
    

private:
    void InitalizeBitmap(void* bufferAddress, size_t bitmapSize);

    void ReservePage(void* address);
    void UnreservePage(void* address);

    void ReservePages(void* address, uint64_t page_count);
    void UnreservePages(void* address, uint64_t page_count);

    uint64_t m_FreeMemory = 0;
    uint64_t m_ReservedMemory = 0;
    uint64_t m_UsedMemory = 0;
    bool m_Initalized = false;
    Bitmap m_Bitmap = Bitmap(nullptr, 0);
};

extern PageFrameAllocator GlobalAllocator;

class PageMapIndexer
{

public:
    PageMapIndexer(uint64_t virtualAddress);

    const uint64_t &GetPDP() const;
    const uint64_t &GetPD() const;
    const uint64_t &GetPT() const;
    const uint64_t &GetP() const;

private:
    uint64_t m_PDP;
    uint64_t m_PD;
    uint64_t m_PT;
    uint64_t m_P;
};

struct PageDirectoryEntry
{

    bool Present : 1;
    bool ReadWrite : 1;
    bool UserSuper : 1;
    bool WriteThrough : 1;
    bool CacheDisabled : 1;
    bool Accessed : 1;
    bool ignore0 : 1;
    bool LargePages : 1;
    bool ignore1 : 1;
    uint8_t Available : 3;
    uint64_t Address : 52;
};

struct PageTable
{
    PageDirectoryEntry entries[512];
} __attribute__((aligned(PAGE_SIZE)));

class PageTableManager
{

public:

    void Initalize(PageTable *PML4Address);
    void MapMemory(void *virtualMemeory, void *physicalMemory);

    PageTable *&GetPageTableAddress();

private:
    PageTable* m_PML4Address;
};