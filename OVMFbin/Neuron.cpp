#include "Neuron.h"

extern "C" void KernelInitalize(Framebuffer framebuffer, MemoryMap memoryMap, PSF1_FONT *font)
{

    InitalizeConsole(&framebuffer, font);

    const char* const string = "Hello\n";

    putc(string[0]);

    const char *index = string;

    while(*index != '\0')
    {
        putc(*index++);
    }

    //print(string);

    return;

    GlobalAllocator.Initalize(memoryMap.Map, memoryMap.MapSize, memoryMap.MapDescriptorSize);

    PageTable *PML4 = (PageTable *)GlobalAllocator.AllocatePage();
    memset(PML4, PAGE_SIZE, 0);

    PageTableManager pageManager(PML4);

    for (uint64_t i = 0; i < GetMemorySize(memoryMap.Map, memoryMap.MapSize, memoryMap.MapDescriptorSize); i += PAGE_SIZE)
    {
        pageManager.MapMemory((void *)i, (void *)i);
    }
    for (uint64_t* i = (uint64_t*)framebuffer.BaseAddress; i < (uint64_t*)framebuffer.BaseAddress + framebuffer.BufferSize; i += PAGE_SIZE)
    {
        pageManager.MapMemory((void *)i, (void *)i);
    }

    asm("mov %0, %%cr3" : : "r" (PML4));
}