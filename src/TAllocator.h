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
            std::cout << "allocate: [n = " << n << "] " << std::endl;
        } break;
        case(FunctionType::deallocate):{
            std::cout << "deallocate: [n  = " << n << "] " << std::endl;
        } break;
        case(FunctionType::construct): {
            std::cout << "construct ";
        } break;
        case (FunctionType::destroy):{
            std::cout << "destroy ";
        }
    }
}

template <typename T, size_t capacity, typename TDebugTag = TNoDebugTag>
class TAllocator {
    static_assert(!std::is_same_v<T, void>, "Type of the allocator can not be void");
    static_assert(capacity != 0, "Zero capacity allocator");
public:
    using debug_tag = TDebugTag; // debug tag
    using value_type = T; // required

    using size_type = std::size_t; // optional
        // An unsigned integer type.
        // Can represent the size of the largest object A can allocate.

    using difference_type = typename std::pointer_traits<value_type*>::difference_type; // optional
        // A signed integer type.
        // Can represent the difference of any two pointers to the objects allocated by A.

    template<class OtherType> // to change type of allocator.
        //For example: list node and list value has different types
            struct rebind{
                typedef TAllocator<OtherType, capacity, TDebugTag> other;
        };
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
    TAllocator(const TAllocator<U, capacity, TDebugTag> & other) noexcept {

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
    T* allocate(size_type n /*, const_void_pointer cvp = 0*/)
    {
        if (n > capacity || UsedMemory >= capacity) // size is bigger then capasity
            throw std::bad_alloc();
        else if(ReservedMemory == nullptr) { // memory not reserved yet
            ReservedMemory = TryToReserveMemory(capacity);
            if ( ReservedMemory != nullptr) {
                // all memory reserved
                if (typeid(debug_tag) == typeid(TDebugInfoTag))
                    DebugInfo(FunctionType::allocate, capacity);
                if (typeid(debug_tag) == typeid(TPrettyTag))
                    ALLOC_PRETTY_INFO(capacity);
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
    void deallocate(T* p, size_type n)
    {
        if(ReservedMemory == nullptr || p == nullptr || n > capacity) // try to deallocate not reserved memory
            throw std::bad_alloc();
        else if (UsedMemory == 0) { // deallocate all reserved memory
            if (typeid(debug_tag) == typeid(TDebugInfoTag))
                DebugInfo(FunctionType::deallocate, capacity);
            if (typeid(debug_tag) == typeid(TPrettyTag))
                ALLOC_PRETTY_INFO(capacity);
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
            if (typeid(debug_tag) == typeid(TDebugInfoTag))
            {
                std::cout << " pointer: " << xp
                          << " " << UsedMemory << " bytes "
                          << " in pull of " << capacity << "*" << sizeof (size_type) << " bytes "
                          << " at " << ReservedMemory << std::endl;
                DebugInfo(FunctionType::construct);
            }
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
        --UsedMemory;
        xp->~X();
        if(typeid(debug_tag) == typeid(TDebugInfoTag))
        {
            std::cout << "pointer: " << xp << " "
                      << UsedMemory << " bytes" <<
                      " of pull " << capacity << "*" << sizeof (size_type)
                      << " at " << ReservedMemory << std::endl;
            DebugInfo(FunctionType::destroy);
        }
        if(typeid(debug_tag) == typeid(TPrettyTag))
            PRETTY_INFO();

    }

    /**
     *
     */

private:
   size_type UsedMemory;
   T* ReservedMemory;
    /**
     * Reserve memory n-blocks sizeof(value_type) length
     * if malloc return nullptr - throw bad_alloc
     * @param n - number of blocks
     */
    [[nodiscard]]
    T* TryToReserveMemory(size_t n)
    {
        T* p = static_cast<T*>(std::malloc(n * sizeof(value_type)));
        if (p == nullptr) {
            throw std::bad_alloc();
        }
        return p;
    }
    /**
     *
     */
};

/**
 * ==
 * @tparam T
 * @tparam U
 * @return
 */
template<typename T, typename U>
constexpr bool operator==(const TAllocator<T, 0> &,
                          const TAllocator<U, 0> &) noexcept
{
    return true;
}

/**
 * !=
 * @tparam T
 * @tparam U
 * @return
 */
template<typename T, typename U>
constexpr bool operator!=(const TAllocator<T, 0> &,
                          const TAllocator<U, 0> &) noexcept
{
    return false;
}
#endif //OTUS_CPP_HW_03_ALLOCATOR_H