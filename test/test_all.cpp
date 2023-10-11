#include <gtest/gtest.h>
#include "TAllocator.h"
#include "TFactorial.h"
#include "TList.h"
#include <iostream>
#include "TRange.h"
/**
 * Google tests
 */

std::vector<std::pair<const int,int>> ExpectedResult={
    {0, 1},
    {1, 1},
    {2, 2},
    {3, 6},
    {4, 24},
    {5, 120},
    {6, 720},
    {7, 5040},
    {8, 40320},
    {9, 362880}
};

TEST(allocator, map) {
    const int SizeOfContainer = 10;
    using debug_tag = TNoDebugTag;
    using StdMap = std::map<const int, int>;
    using StdMapAllocated = std::map<const int, int, std::less<>,
            TAllocator<std::pair<const int, int>, SizeOfContainer, debug_tag>>;
    StdMap StdFactorialMap;
    std::cout << " init StdMap with factorial " << std::endl;
    for(int i: TRange(SizeOfContainer))
        StdFactorialMap.insert({i, Factorial(i)});

    std::cout << " print StdMap " << std::endl;
    for(const auto [key, value]: StdFactorialMap)
        std::cout << key << " " << value << std::endl;
    for(auto item: StdFactorialMap)
        EXPECT_TRUE(ExpectedResult[item.first]==item);
    std::cout << "----------------------" << std::endl;


    StdMapAllocated StdFactorialMapAllocated;
    std::cout << "init StdMap Custom Allocated with factorial" << std::endl;
    for(int i: TRange(SizeOfContainer))
        StdFactorialMapAllocated.insert({i, Factorial(i)});

    std::cout << "print StdMap Custom Allocated" << std::endl;
    for(const auto [key, value]: StdFactorialMapAllocated)
        std::cout << key << " " << value << std::endl;

    for(auto item: StdFactorialMapAllocated)
        EXPECT_TRUE(ExpectedResult[item.first]==item);
    std::cout << "----------------------" << std::endl;

}

TEST(allocator, list) {
    const int SizeOfContainer = 10;
    using debug_tag = TNoDebugTag;
    using CustomList = TList<std::pair<const int, int>>;
    using CustomListAllocated = TList<std::pair<const int, int>, TAllocator<std::pair<const int, int>, SizeOfContainer, debug_tag>>;
    std::cout << "init CustomList with factorial" << std::endl;
    CustomList List;
    for(int i: TRange(SizeOfContainer))
        List.push_back({i, Factorial(i)});

    std::cout << "print CustomList with factorial" << std::endl;
    for (const auto [i, item]:List)
        std::cout << i << " " << item << std::endl;

    for(auto item: List)
        EXPECT_TRUE(ExpectedResult[item.first]==item);
    std::cout << "----------------------" << std::endl;

    std::cout << "init CustomListAllocated with factorial" << std::endl;
    CustomListAllocated ListAllocated;
    for(int i: TRange(SizeOfContainer))
        ListAllocated.push_back({i, Factorial(i)});

    std::cout << "print CustomListAllocated with factorial" << std::endl;
    for (const auto [i, item]:List)
        std::cout << i << " " << item << std::endl;

    for(auto item: ListAllocated)
        EXPECT_TRUE(ExpectedResult[item.first]==item);
    std::cout << "----------------------" << std::endl;
}