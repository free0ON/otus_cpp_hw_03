/**
 * template class Allocator
 * 
 * Stateful and stateless allocators
 * Every Allocator type is either stateful or stateless. 
 * Generally, a stateful allocator type can have unequal values 
 * which denote distinct memory resources, while a stateless allocator 
 * type denotes a single memory resource.
 * Although custom allocators are not required to be stateless, 
 * whether and how the use of stateful allocators in the standard library 
 * is implementation-defined. Use of unequal allocator values may result 
 * in implementation-defined runtime errors or undefined behavior 
 * if the implementation does not support such usage.
 * (until C++11)
 * Custom allocators may contain state. 
 * Each container or another allocator-aware object stores an instance 
 * of the supplied allocator and controls allocator replacement through std::allocator_traits.
 * (since C++11)
 * Instances of a stateless allocator type always compare equal. Stateless allocator types 
 * are typically implemented as empty classes and suitable for empty base class optimization.
 * The member type is_always_equal of std::allocator_traits is intendedly used for determining 
 * whether an allocator type is stateless.
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
};

class TNoDebugTag : TAbstractTag
{
};

class TPrettyTag: TAbstractTag
{
};

class TDebugInfoTag: TAbstractTag
{
};


enum class FunctionType
{
    allocate, deallocate, construct, destroy
};

constexpr void DebugInfo(FunctionType type, size_t n = 0)
{
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
}

template <typename T, size_t capasity, typename TDebugTag = TNoDebugTag>
class TAllocator {
    static_assert(!std::is_same_v<T, void>, "Type of the allocator can not be void");
public:
    using debug_tag = TDebugTag; // debug tag
    using value_type = T; // required

    using pointer = T *;
        // optional
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
        using other = TAllocator<U, capasity, TDebugTag>;
    };

    using is_always_equal = std::true_type;
    
    TAllocator() noexcept {
        UsedMemory = 0;
        ReservedMemory = nullptr;
    }

    ~TAllocator() noexcept {
    };

    /**
     * Copy constructor
     * @tparam U 
     */
    template<typename U>
    TAllocator(const TAllocator<U, capasity, TDebugTag> & other) noexcept {

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
        if (n > capasity || UsedMemory >= capasity) // size is bigger then capasity
            throw std::bad_alloc();
        else if(ReservedMemory == nullptr) { // memory not reserved yet
            ReservedMemory = TryToReserveMemory(capasity);
            if ( ReservedMemory != nullptr) {
                // all memory reserved
                if (typeid(debug_tag) == typeid(TDebugInfoTag))
                    DebugInfo(FunctionType::allocate, capasity);
                if (typeid(debug_tag) == typeid(TPrettyTag))
                    ALLOC_PRETTY_INFO(capasity);
            }
        }
        return ReservedMemory + UsedMemory;
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
    void deallocate(pointer p, size_type n)
    {
        if(ReservedMemory == nullptr || p == nullptr || n > capasity) // try to deallocate not reserved memory
            throw std::bad_alloc();
        else if (UsedMemory == 0) { // deallocate all reserved memory
            if (typeid(debug_tag) == typeid(TDebugInfoTag))
                DebugInfo(FunctionType::deallocate, capasity);
            if (typeid(debug_tag) == typeid(TPrettyTag))
                ALLOC_PRETTY_INFO(capasity);
            std::free(ReservedMemory);
        }
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
    void construct(X *xp, Args &&...args) {
        auto Result = ::new (static_cast<void*>(xp)) X(std::forward<Args>(args)...); // placement new to create object at xp address
        if(Result != 0) {
            ++UsedMemory;
            std::cout << "pointer: " << xp << " " << UsedMemory << " in pull of " << capasity << "*" << sizeof (size_type) << " bytes " << " at " << ReservedMemory << " ";
            if (typeid(debug_tag) == typeid(TDebugInfoTag))
                DebugInfo(FunctionType::construct);
            if (typeid(debug_tag) == typeid(TPrettyTag))
                PRETTY_INFO();
        }
    }

    /** optional
     * destroy
     * Destructs an object of type X pointed to by xp, but does not deallocate any storage.
     * @tparam X
     * @param xp
     */
    template <typename X>
    void destroy(X *xp) {
        std::cout << "pointer: " << xp << " " << UsedMemory << " of pull "<< capasity << "*" << sizeof (size_type) << " at " << ReservedMemory << " ";
        if(typeid(debug_tag) == typeid(TDebugInfoTag))
            DebugInfo(FunctionType::destroy);
        if(typeid(debug_tag) == typeid(TPrettyTag))
            PRETTY_INFO();
        xp->~X();
        --UsedMemory;
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

private:
   size_type UsedMemory;
   pointer ReservedMemory;
    /**
     * Reserve memory n-blocks sizeof(value_type) length
     * if malloc return nullptr - throw bad_alloc
     * @param n - number of blocks
     */
    [[nodiscard]]
    pointer TryToReserveMemory(size_t n)
    {
        pointer p = static_cast<pointer>(std::malloc(n * sizeof(value_type)));
        if (p == nullptr) {
            throw std::bad_alloc();
        }
        return p;
    }
    /**
     *
     */
};
#endif //OTUS_CPP_HW_03_ALLOCATOR_H