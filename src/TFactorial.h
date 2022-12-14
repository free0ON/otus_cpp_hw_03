/**
 * TFactorial
 */

#ifndef ALLOCATOR_TFACTORIAL_H
#define ALLOCATOR_TFACTORIAL_H

#include <initializer_list>
#include <utility>
#include <iostream>

constexpr int Factorial(int i)
{
    return (i == 0) || (i == 1)? 1: i * Factorial(i - 1);
}

template<typename T>
T InitFactorialMap(int n) {
    T InitList;
    for(int i = 0; i < n; ++i)
    {
        InitList.insert({i, Factorial(i)});
    }
    return InitList; // moving a local object in a return statement prevents copy elision
}

template<typename T>
T InitFactorialVector(int n) {
    T InitList;
    try {
        for (int i = 0; i < n; ++i) {
            InitList.push_back(Factorial(i));
        }
    }
    catch(...) {
        std::cout << "error" << std::endl;
    }
    return InitList; // moving a local object in a return statement prevents copy elision
}
template <typename T>
static void PrintMap(std::string_view header, T& map)
{
    std::cout << header << std::endl;
    for(const auto [key, value]: map)
    {
        std::cout << key << " " << value << std::endl;
    }
}

template <typename T>
static void PrintVector(std::string_view header, T& vec)
{
    std::cout << header << std::endl;
    for(size_t i = 0; i < vec.size(); ++i)
    {
        std::cout << i << " " << vec[i] << std::endl;
    }
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

