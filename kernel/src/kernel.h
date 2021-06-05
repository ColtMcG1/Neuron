#pragma once

#include "graphics/Graphics.h"
#include "memory/Memory.h"

struct KernelData
{
    MemoryMap *GlobalMemoryMap = 0;
    PageTable *GlobalPageTable = 0;
    PageTableManager GlobalPageTableManager;
};

KernelData InitalizeMemory();

extern "C" void KernelInitalize(Framebuffer framebuffer, MemoryMap memoryMap, PSF1_FONT *font);