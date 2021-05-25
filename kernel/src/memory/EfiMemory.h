#pragma once

#include <cstdint>

struct EFI_MEMORY_DESCRIPTOR
{
    uint32_t Type;
    void* PhysicalAddress;
    void* VirtualAddress;
    uint64_t NumberOfPages;
    uint64_t Attribute;
};

enum EFI_MEMORY_TYPE
{
    EfiReservedMemoryType = 0,
    EfiLoaderCode = 1,
    EfiLoaderData = 2,
    EfiBootServicesCode = 3,
    EfiBootServicesData = 4,
    EfiRuntimeServicesCode = 5,
    EfiRuntimeServicesData = 6,
    EfiConventionalMemory = 7,
    EfiUnusableMemory,
    EfiACPIReclaimMemory,
    EfiACPIMemoryNVS,
    EfiMemoryMappedIO,
    EfiMemoryMappedIOPortSpace,
    EfiPalCode
};

extern const char* EFI_MEMORY_TYPE_STRINGS[];

struct MemoryMap
{
    EFI_MEMORY_DESCRIPTOR *Map;
    unsigned long long MapSize;
    unsigned long long MapDescriptorSize;
};

