#include <memory.h>
#include "new.h"


void* operator new(size_t _Size)
{

}

void* operator new(size_t _Size, std::nothrow_t const&) noexcept
{

}

void* operator new[](size_t _Size)
{

}

void* operator new[](size_t _Size, std::nothrow_t const&) noexcept
{

}

void operator delete(void* _Block) noexcept
{

}

void operator delete(void* _Block, std::nothrow_t const&) noexcept
{

}

void operator delete[](void* _Block) noexcept
{

}

void operator delete[](void* _Block, std::nothrow_t const&) noexcept
{

}

void operator delete(void*  _Block, size_t _Size) noexcept
{

}

void operator delete[](void* _Block, size_t _Size) noexcept
{

}

#ifdef __cpp_aligned_new
void* operator new(size_t _Size, std::align_val_t _Al)
{

}
void* operator new(size_t size,std::align_val_t _Al, std::nothrow_t const&) noexcept
{

}

void* operator new[](size_t size, std::align_val_t _Al)
{

}

void* operator new[](size_t size, std::align_val_t _Al, std::nothrow_t const&) noexcept
{

}

void operator delete(void* _Block, std::align_val_t _Al) noexcept
{

}

void operator delete(void* block, std::align_val_t _Al, std::nothrow_t const&) noexcept
{

}

void operator delete[](void* block, std::align_val_t _Al) noexcept
{

}

void operator delete[](void* block, std::align_val_t _Al, std::nothrow_t const&) noexcept
{

}

void operator delete(void* block, size_t size, std::align_val_t _Al) noexcept
{

}

void operator delete[](void* block, size_t size, std::align_val_t _Al) noexcept
{
    
}

#endif //__cpp_aligned_new