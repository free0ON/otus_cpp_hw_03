/**
 * TVector
 */

#ifndef ALLOCATOR_TVECTOR_H
#define ALLOCATOR_TVECTOR_H

#include <cstddef>
#include <cstdint>
#include <memory>
#include <stdexcept>

template<typename T, typename Alloc = std::allocator<T>>
class TVector {
private:
    T* arr;
    size_t sz;
    size_t cap;
    Alloc alloc;
public:
//    TVector(TVector&& v)
//    {
//        sz = v.sz;
//        cap = v.cap;
//        alloc = v.alloc;
//        arr = v.arr;
//    }



    TVector(const Alloc& alloc = Alloc()): alloc(alloc)
    {
        sz = 0;
        cap = 0;
    }
    TVector(size_t count, const T& value, const Alloc& alloc = Alloc()): alloc(alloc)
    {
        cap = 0;
        reserve(count);
        for (size_t i = 0; i < count; ++i)
        {
            alloc.construct(arr + i, value);
        }
    }

    size_t size()
    {
        return sz;
    }

    size_t capacity()
    {
        return cap;
    }

    void reserve(size_t n)
    {
        if(n <= cap) return;
        T* new_arr = alloc.allocate(n);
        if (cap > 0 && sz > 0)
        {
            try
            {
                std::uninitialized_copy(arr, arr + sz, new_arr);
                for (size_t i = 0; i < sz; alloc.destroy(arr + i), ++i);
                alloc.deallocate(arr, sz);
            }
            catch (...)
            {
                alloc.deallocate(new_arr, n);
                throw;
            }
        }
        arr = new_arr;
        cap = n;
    }

    void resize(size_t n)
    {
        if(n > cap) {
            reserve(n);
        }
        if (n < sz)
        {
            for(size_t i = n; i < sz; alloc.destroy(arr + i), ++i);
            sz = n;
        }
    }

    void push_back(const T & value)
    {
        if (sz == 0)
            resize(1);
        if (sz == cap)
            resize(2 * sz);
       alloc.construct((arr + sz++), value);
    }

    void pop_back()
    {
        if(sz > 0)
            alloc.destroy(arr + --sz);
    }

    T & operator[] (size_t i)
    {
        return arr[i];
    }

    const T & operator [] (size_t i) const
    {
        return arr[i];
    }

    T & at(size_t i)
    {
        if(i >= sz)
            throw std::out_of_range("Index out of range");
        return arr[i];
    }
};
#endif //ALLOCATOR_TVECTOR_H