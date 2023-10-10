#include <gtest/gtest.h>
#include "TAllocator.h"
#include "TFactorial.h"
#include "TVector.h"
#include "TList.h"
#include <iostream>
/**
 * Google tests
 */

std::vector<std::pair<int,int>> ExpectedResult={
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
    using StdMap = std::map<int, int>;
    StdMap StdFactorialMap;
}

TEST(allocator, list) {
    using StdMap = std::map<int, int>;
    StdMap StdFactorialMap;
}