/**
 * TVector
 */

#ifndef ALLOCATOR_TVECTOR_H
#define ALLOCATOR_TVECTOR_H

#include <cstddef>
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <iterator>

template<typename T, typename Alloc = std::allocator<T>, typename Iter = std::iterator<T, std::random_access_iterator_tag>>
class TVector {
private:
    T *arr = nullptr;
    size_t sz = 0;
    size_t cap = 0;
    Alloc alloc;
    Iter iter;
public:

    TVector(const Alloc &alloc = Alloc(),
            const Iter &iter = Iter()) : alloc(alloc), iter(iter) {
    }

    size_t size() {
        return sz;
    }

    size_t capacity() {
        return cap;
    }

    const Iter& begin()
    {
        return iter;
    }

    const Iter& end()
    {
        return iter;
    }

    void reserve(size_t n) {
        if (n <= cap) return;
        //TODO: allocation problem with std::allocator
        T *new_arr = alloc.allocate(n);
        if (new_arr == nullptr) throw std::bad_alloc();
        else {
            if (new_arr != nullptr && arr != nullptr && new_arr != arr) {
                try {
                    std::uninitialized_copy(arr, arr + sz, new_arr);
                    for (size_t i = 0; i < sz; alloc.destroy(arr + i), ++i);
                    alloc.deallocate(arr, sz);
                }
                catch (...) {
                    alloc.deallocate(new_arr, n);
                    throw;
                }
            }
            arr = new_arr;
            cap = n;
        }
    }

    void resize(size_t n) {
        if (n > cap) {
            reserve(n);
        }
        if (n < sz) {
            for (size_t i = n; i < sz; alloc.destroy(arr + i), ++i);
            sz = n;
        }
    }

    void push_back(const T &value) {
        // TODO: wrong realisation of push_back
        if (sz == cap)
            resize(++sz);
        alloc.construct((arr + sz), value);
    }

    void pop_back() {
        if (sz > 0)
            alloc.destroy(arr + --sz);
    }

    T &operator[](size_t i) {
        return arr[i];
    }

    const T &operator[](size_t i) const {
        return arr[i];
    }

    T &at(size_t i) {
        if (i >= sz)
            throw std::out_of_range("Index out of range");
        return arr[i];
    }
};

#endif //ALLOCATOR_TVECTOR_H