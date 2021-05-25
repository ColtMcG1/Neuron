#include "kernel.h"

extern "C" void KernelInitalize(Framebuffer framebuffer, MemoryMap memoryMap, PSF1_FONT *font)
{

    InitalizeConsole(&framebuffer, font);

    const char *string = "Hello";
    print(string);

    /*
    const char beginning[] = "Beginning\n";
    print(beginning);

    GlobalAllocator.Initalize(memoryMap.Map, memoryMap.MapSize, memoryMap.MapDescriptorSize);

    PageTable *PML4 = (PageTable *)GlobalAllocator.AllocatePage();
    memset(PML4, PAGE_SIZE, 0);

    PageTableManager pageManager(PML4);

    const char beginningMapping[] = "Beginning page mapping\n";
    print(beginningMapping);

    for (uint64_t i = 0; i < GetMemorySize(memoryMap.Map, memoryMap.MapSize, memoryMap.MapDescriptorSize); i += PAGE_SIZE)
    {
        pageManager.MapMemory((void *)i, (void *)i);
    }
    const char mappedPages[] = "Mapped pages\n";
    print(mappedPages);

    for (uint64_t* i = (uint64_t*)framebuffer.BaseAddress; i < (uint64_t*)framebuffer.BaseAddress + framebuffer.BufferSize; i += PAGE_SIZE)
    {
        pageManager.MapMemory((void *)i, (void *)i);
    }
    const char mappedGraphics[] = "Mapped graphics\n";
    print(mappedGraphics);

    asm("mov %0, %%cr3" : : "r" (PML4));

    const char end[] = "Endding\n";
    print(end);
    */
}