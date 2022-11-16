/**
 * allocator main
 * @return
 */

#include <gtest/gtest.h>
#include "TVector.h"
#include "TAllocator.h"
#include "TFactorial.h"

#define TEST_IN_MAIN

#ifdef TEST_IN_MAIN
TEST(allocator, std_map) {
    using StdMap = std::map<int, int>;
    // create std::map<int, int> and fill map with int key 0 ... 9 and factorial(key) values
    auto StdFactorialMap = InitFactorialMap<StdMap>(10);
    // print std map
    PrintMap("std::map", StdFactorialMap);
}

TEST(allocator, std_map_allocated)
{
    using debug_tag = TNoDebugTag;
    using StdMapAllocated = std::map<int, int, std::less<int>,
            TAllocator<std::pair<int, int>, debug_tag>>;
    // create std::map<int, int, TAllocator> with custom Allocator and fill Factorial 0 ... 9
    auto StdFactorialMapAllocated = InitFactorialMap<StdMapAllocated>(10);
    //print std map allocated
    PrintMap("std::map allocated", StdFactorialMapAllocated);
}

TEST(allocator,custom_container) {
    using CustomVector = TVector<int>;
    auto Vector = InitFactorialVector<CustomVector>(10);
    PrintVector("custom TVector", Vector);
}

TEST(allocator,custom_container_allocated){
    using debug_tag = TNoDebugTag;
    using CustomVectorAllocated = TVector<int, TAllocator<int, debug_tag>>;
    auto AllocatedVector = InitFactorialVector<CustomVectorAllocated>(10);
    PrintVector("custom TVector allocated", AllocatedVector);
}
#endif

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}