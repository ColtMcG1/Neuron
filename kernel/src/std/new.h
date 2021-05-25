#pragma once

#pragma push_macro("new")
#undef new


#ifdef __cpp_aligned_new
namespace std
{
    enum class align_val_t : size_t {};
}
#endif // __cpp_aligned_new

#ifndef __NOTHROW_T_DEFINED
#define __NOTHROW_T_DEFINED
    namespace std
    {
        struct nothrow_t {
#ifndef _CRTBLD // TRANSITION, VSO#406237
            explicit nothrow_t() = default;
#endif // _CRTBLD
        };

        #ifdef _CRT_ENABLE_SELECTANY_NOTHROW
            extern __declspec(selectany) nothrow_t const nothrow;
        #else
            extern nothrow_t const nothrow;
        #endif
    }
#endif

void* operator new(
    size_t _Size
    );

void* operator new(
    size_t _Size,
    std::nothrow_t const&
    ) noexcept;

void* operator new[](
    size_t _Size
    );

void* operator new[](
    size_t _Size,
    std::nothrow_t const&
    ) noexcept;

void operator delete(
    void* _Block
    ) noexcept;

void operator delete(
    void* _Block,
    std::nothrow_t const&
    ) noexcept;

void operator delete[](
    void* _Block
    ) noexcept;

void operator delete[](
    void* _Block,
    std::nothrow_t const&
    ) noexcept;

void operator delete(
    void*  _Block,
    size_t _Size
    ) noexcept;

void operator delete[](
    void* _Block,
    size_t _Size
    ) noexcept;

#ifdef __cpp_aligned_new
void* operator new(
    size_t           _Size,
    std::align_val_t _Al
    );

void* operator new(
    size_t                _Size,
    std::align_val_t      _Al,
    std::nothrow_t const&
    ) noexcept;

void* operator new[](
    size_t           _Size,
    std::align_val_t _Al
    );

void* operator new[](
    size_t                _Size,
    std::align_val_t      _Al,
    std::nothrow_t const&
    ) noexcept;

void operator delete(
    void*            _Block,
    std::align_val_t _Al
    ) noexcept;

void operator delete(
    void*                 _Block,
    std::align_val_t      _Al,
    std::nothrow_t const&
    ) noexcept;

void operator delete[](
    void*            _Block,
    std::align_val_t _Al
    ) noexcept;

void operator delete[](
    void*                 _Block,
    std::align_val_t      _Al,
    std::nothrow_t const&
    ) noexcept;

void operator delete(
    void*            _Block,
    size_t           _Size,
    std::align_val_t _Al
    ) noexcept;

void operator delete[](
    void*            _Block,
    size_t           _Size,
    std::align_val_t _Al
    ) noexcept;
#endif // __cpp_aligned_new

#pragma warning(push)
#pragma warning(disable: 4577) // 'noexcept' used with no exception handling mode specified
#pragma warning(disable: 4514) // 'operator new': unreferenced inline function has been removed
#ifndef __PLACEMENT_NEW_INLINE
    #define __PLACEMENT_NEW_INLINE
    
    inline void* operator new(size_t _Size, void* _Where) noexcept
    {
        (void)_Size;
        return _Where;
    }

    inline void operator delete(void*, void*) noexcept
    {
        return;
    }
#endif

#ifndef __PLACEMENT_VEC_NEW_INLINE
    #define __PLACEMENT_VEC_NEW_INLINE
    
    inline void* operator new[](size_t _Size, void* _Where) noexcept
    {
        (void)_Size;
        return _Where;
    }

    inline void operator delete[](void*, void*) noexcept
    {
    }
#endif
#pragma warning(pop)

#pragma pop_macro("new")