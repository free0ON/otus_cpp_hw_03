/**
 * TFactorial
 */

#ifndef ALLOCATOR_TFACTORIAL_H
#define ALLOCATOR_TFACTORIAL_H

#include <initializer_list>
#include <utility>
#include <iostream>
#include "TList.h"

constexpr int Factorial(int i)
{
    return (i == 0) || (i == 1)? 1: i * Factorial(i - 1);
}

template<int p>
class TFactorial {
public:
    static const int value = (p > 0)? p * TFactorial<(p-1)>::value:0;
public:

    static int get()
    {
        return value;
    }

    static std::pair<int, int> get_pair()
    {
        return {p, value};
    }
};

template<>
class TFactorial<0> {
public:
    static const int value = 1;
    static auto get()
    {
        return value;
    }
    static std::pair<int, int> get_pair()
    {
        return {0, value};
    }
};
#endif //ALLOCATOR_TFACTORIAL_H

