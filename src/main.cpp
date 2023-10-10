/**
 * allocator main
 * @return
 */

#include <gtest/gtest.h>
#include <list>
#include "TVector.h"
#include "TAllocator.h"
#include "TFactorial.h"
#include "TList.h"
#include <chrono>
#include "TRange.h"

int main()
{
    const int SizeOfContainer = 10;
    using debug_tag = TNoDebugTag;
    using StdMap = std::map<const int, int>;
    using StdMapAllocated = std::map<const int, int, std::less<>,
        TAllocator<std::pair<const int, int>, SizeOfContainer, debug_tag>>;

    using CustomList = TList<std::pair<const int, int>>;

    using CustomListAllocated = TList<std::pair<const int, int>, TAllocator<std::pair<const int, int>, SizeOfContainer, debug_tag>>;

    StdMap StdFactorialMap;
    std::cout << " init StdMap with factorial " << std::endl;
    for(int i: TRange(SizeOfContainer))
        StdFactorialMap.insert({i, Factorial(i)});

    std::cout << " print StdMap " << std::endl;
    for(const auto [key, value]: StdFactorialMap)
        std::cout << key << " " << value << std::endl;
    std::cout << "----------------------" << std::endl;

    StdMapAllocated StdFactorialMapAllocated;
    std::cout << "init StdMap Custom Allocated with factorial" << std::endl;
    for(int i: TRange(SizeOfContainer))
        StdFactorialMapAllocated.insert({i, Factorial(i)});

    std::cout << "print StdMap Custom Allocated" << std::endl;
    for(const auto [key, value]: StdFactorialMapAllocated)
        std::cout << key << " " << value << std::endl;
    std::cout << "----------------------" << std::endl;

    std::cout << "init CustomList with factorial" << std::endl;
    CustomList List;
    for(int i: TRange(SizeOfContainer))
        List.push_back({i, Factorial(i)});

    std::cout << "print CustomList with factorial" << std::endl;
    for (const auto [i, item]:List)
        std::cout << i << " " << item << std::endl;
    std::cout << "----------------------" << std::endl;

    std::cout << "init CustomListAllocated with factorial" << std::endl;
    CustomListAllocated ListAllocated;
    for(int i: TRange(SizeOfContainer))
        ListAllocated.push_back({i, Factorial(i)});

    std::cout << "print CustomListAllocated with factorial" << std::endl;
    for (const auto [i, item]:List)
        std::cout << i << " " << item << std::endl;
    std::cout << "----------------------" << std::endl;
}