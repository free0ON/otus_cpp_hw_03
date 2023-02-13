#include <gtest/gtest.h>
#include "TAllocator.h"
#include "TFactorial.h"
#include "TVector.h"
#include "TList.h"
#include "TIterator.h"
#include <iostream>
/**
 * Google tests
 */

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
            TAllocator<std::pair<int, int>, 10, debug_tag>>;
    // create std::map<int, int, TAllocator> with custom Allocator and fill Factorial 0 ... 9
    auto StdFactorialMapAllocated = InitFactorialMap<StdMapAllocated>(10);
    //print std map allocated
    PrintMap("std::map allocated", StdFactorialMapAllocated);
}

TEST(allocator,custom_container) {
    using CustomVector = TVector<int>;
    // create custom vector and fill Factorial 0 ... 9
    auto Vector = InitFactorialVector<CustomVector>(10);
    // print custom vector
    PrintVector("custom TVector", Vector);
}

TEST(allocator,custom_container_allocated){
    using debug_tag = TNoDebugTag;
    using CustomVectorAllocated = TVector<int, TAllocator<int, 10, debug_tag>>;
    // create custom vector TVector with custom allocator TAllocator and fill Factorial 0 ... 9
    auto AllocatedVector = InitFactorialVector<CustomVectorAllocated>(10);
    // print custom vector
    PrintVector("custom TVector allocated", AllocatedVector);
}

//TEST(allocator, bad_alloc_custom_container_allocated) {
//
//    using debug_tag = TNoDebugTag;
//    std::vector<uint64_t> MaxVectorTest;
//    uint64_t MaxSize = MaxVectorTest.max_size();
//    using BigCustomVectorAllocated = TVector<uint64_t, TAllocator<uint64_t, 10, debug_tag>>;
//
//
//    EXPECT_THROW({
//                     try {
//                         BigCustomVectorAllocated BigAllocatedVector(MaxSize, 0);
//                     }
//                     catch (const std::exception& e)
//                     {
//                         EXPECT_TRUE(typeid(e) == typeid(std::bad_alloc));
//                         std::cout << e.what();
//                         throw;
//                     }
//                 }, std::bad_alloc);
//}

TEST(allocator, custom_list) {
    using debug_tag = TNoDebugTag;
    using CustomList = TList<int, TAllocator<int, 10, debug_tag>, TIterator<int>>;
    auto List = InitFactorialList<CustomList>(10);
    // print custom vector
    PrintList("custom TList", List);

}