/**
 * TFactorial
 */

#ifndef ALLOCATOR_TFACTORIAL_H
#define ALLOCATOR_TFACTORIAL_H

#include <initializer_list>
#include <utility>
#include <iostream>

int Factorial(int i);

template<typename T>
T InitFactorialMap(int n) {
    T InitList;
    for(int i = 0; i < n; ++i)
    {
        InitList.insert({i, Factorial(i)});
    }
    return std::move(InitList);
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
    return std::move(InitList);
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
    for(int i = 0; i < vec.size(); ++i)
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
    static auto get()
    {
        return value;
    }

    static std::pair<int, int> get_pair()
    {
        return {0, value};
    }

    static const int value = 1;
};
#endif //ALLOCATOR_TFACTORIAL_H

