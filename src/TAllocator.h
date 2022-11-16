/**
 * template class Allocator
 */
#ifndef OTUS_CPP_HW_03_ALLOCATOR_H
#define OTUS_CPP_HW_03_ALLOCATOR_H

#include <cstdlib>
#include <memory>
#include <iostream>

#define ALLOC_PRETTY_INFO(n) std::cout << __PRETTY_FUNCTION__ << "[n = " << n << "]" << std::endl;
#define PRETTY_INFO()  std::cout << __PRETTY_FUNCTION__ << std::endl;

class TAbstractTag
{
    TAbstractTag();
};

class TNoDebugTag : TAbstractTag
{
    TNoDebugTag();
};

class TPrettyTag: TAbstractTag
{
    TPrettyTag();
};

class TDebugInfoTag: TAbstractTag
{
    TDebugInfoTag();
};


enum class FunctionType
{
    allocate, deallocate, construct, destroy
};

constexpr void DebugInfo(FunctionType type, size_t n = 0)
{
//#ifndef USE_PRETTY
    switch (type) {
        case(FunctionType::allocate): {
            std::cout << "allocate: [n = " << n << "]" << std::endl;
        } break;
        case(FunctionType::deallocate):{
            std::cout << "deallocate: [n  = " << n << "] " << std::endl;
        } break;
        case(FunctionType::construct): {
            std::cout << "construct" << std::endl;
        } break;
        case (FunctionType::destroy):{
            std::cout << "destroy" << std::endl;
        }
    }
//#endif
}

//template<typename T>
//struct pointer_traits {
//    using reference = T &;
//    using const_reference = const T &;
//};
//
//// Avoid declaring a reference to void with an empty specialization
//template<>
//struct pointer_traits<void> {
//};

template <typename T, typename TDebugTag = TNoDebugTag>
class TAllocator {
    static_assert(!std::is_same_v<T, void>, "Type of the allocator can not be void");
public:
    using debug_tag = TDebugTag; // debug tag
    using value_type = T; // required

    using pointer = T *; // optional
        // Satisfies NullablePointer, LegacyRandomAccessIterator,
        // and LegacyContiguousIterator.
    using const_pointer = const T *; // optional
        // Satisfies NullablePointer, LegacyRandomAccessIterator,
        // and LegacyContiguousIterator. A::pointer is convertible to A::const_pointer.
    using void_pointer = void *; // optional
        // Satisfies NullablePointer.  A::pointer is convertible to A::void_pointer.
        //B::void_pointer and A::void_pointer are the same type.
    using const_void_pointer = const void *; // optional
        // Satisfies NullablePointer.
        // A::pointer, A::const_pointer, and A::void_pointer are convertible to A::const_void_pointer.
    using size_type = std::size_t; // optional
        // An unsigned integer type.
        // Can represent the size of the largest object A can allocate.
    using difference_type = typename std::pointer_traits<pointer>::difference_type; // optional
        // A signed integer type.
        // Can represent the difference of any two pointers to the objects allocated by A.

    // rebind is only optional (provided by std::allocator_traits)
    // if this allocator is a template of the form SomeAllocator<T, Args>,
    // where Args is zero or more additional template type parameters.
    template<typename U> // optional For any U, B::template rebind<T>::other is A.
    struct rebind {
        using other = TAllocator<U, TDebugTag>;
    };

    TAllocator() noexcept {}

    TAllocator(size_t n)
    {
        allocate(n);
    }

    ~TAllocator() noexcept {};

    template<typename U>
    TAllocator(const TAllocator<U, TDebugTag> &) noexcept {

    }

    /** required
     * Allocates storage suitable for an array object of type T[n] and creates the array,
     * but does not construct array elements.
     * May throw exceptions. If n == 0, the return value is unspecified.
     *
     * May be used with cvp (nullptr or a pointer obtained from a.allocate())
     * in unspecified manner to aid locality.
     * @param n - number of allocated memory blocks sizeof(T)
     * @return
     */
    [[nodiscard]]
    pointer allocate(size_type n /*, const_void_pointer cvp = 0*/)
    {
        if(typeid(debug_tag) == typeid(TDebugInfoTag))
            DebugInfo(FunctionType::allocate, n);
        if(typeid(debug_tag) == typeid(TPrettyTag))
            ALLOC_PRETTY_INFO(n);
        auto p = std::malloc(n * sizeof(value_type));
        if (!p)
            throw std::bad_alloc();
        return reinterpret_cast<pointer>(p);

    }
    /** required
     * Deallocates storage pointed to p, which must be a value returned by a previous call to allocate
     * or allocate_at_least (since C++23) that has not been invalidated by an intervening call to deallocate.
     * n must match the value previously passed to allocate or be between the request and returned
     * number of elements via allocate_at_least (may be equal to either bound) (since C++23).
     * Does not throw exceptions.
     * @param p
     * @param n
     */
    void deallocate(pointer p, size_type n) noexcept
    {
        if(typeid(debug_tag) == typeid(TDebugInfoTag))
            DebugInfo(FunctionType::deallocate, n);
        if(typeid(debug_tag) == typeid(TPrettyTag))
            ALLOC_PRETTY_INFO(n);
        std::free(p);
    }

    /** optional
     * construct
     * Constructs an object of type X in previously-allocated storage
     * at the address pointed to by xp,
     * using Args as the constructor arguments.
     * @tparam X - type
     * @tparam Args - type os args
     * @param xp - pointer
     * @param args - arguments ...
     */
    template <typename X, typename... Args>
    void construct(X *xp, Args &&...args) const {
        if(typeid(debug_tag) == typeid(TDebugInfoTag))
            DebugInfo(FunctionType::construct);
        if(typeid(debug_tag) == typeid(TPrettyTag))
            PRETTY_INFO();
        new (xp) X(std::forward<Args>(args)...); // placement new to create object at xp address
    }

    /** optional
     * destroy
     * Destructs an object of type X pointed to by xp, but does not deallocate any storage.
     * @tparam X
     * @param xp
     */
    template <typename X>
    void destroy(X *xp) {
        if(typeid(debug_tag) == typeid(TDebugInfoTag))
            DebugInfo(FunctionType::destroy);
        if(typeid(debug_tag) == typeid(TPrettyTag))
            PRETTY_INFO();
        xp->~X();
    }
    /**
     *  The largest value that can be passed to A::allocate().
     */
    size_type max_size() {
        return size_type();
    }

    /**
     *
     */
};
#endif //OTUS_CPP_HW_03_ALLOCATOR_H