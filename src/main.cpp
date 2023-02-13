/**
 * allocator main
 * @return
 */

#include <gtest/gtest.h>
#include "TVector.h"
#include "TAllocator.h"
#include "TFactorial.h"
#include "TList.h"
#include "TIterator.h"

int main()
{
    using debug_tag = TDebugInfoTag;
    using StdMap = std::map<const int, int>;
    using StdMapAllocated = std::map<const int, int, std::less<>,
            TAllocator<std::pair<const int, int>, 10, debug_tag>>;
    //using CustomVector = TVector<int>;
//    using CustomVectorAllocated = TVector<int, TAllocator<int, 10, debug_tag>>;

//    using CustomList = TList<int>;


    // create std::map<int, int> and fill map with int key 0 ... 9 and factorial(key) values
    auto StdFactorialMap = InitFactorialMap<StdMap>(10);
    // print std map
    PrintMap("std::map", StdFactorialMap);
    std::destroy(StdFactorialMap.begin(), StdFactorialMap.end());
    // create std::map<int, int, TAllocator> with custom Allocator and fill Factorial 0 ... 9
    auto StdFactorialMapAllocated = InitFactorialMap<StdMapAllocated>(10);
    //print std map allocated
    PrintMap("std::map allocated", StdFactorialMapAllocated);
    //std::destroy(StdFactorialMapAllocated.begin(), StdFactorialMapAllocated.end());
    // create custom vector and fill Factorial 0 ... 9
    //auto Vector = InitFactorialVector<CustomVector>(10);
    // print custom vector
    //PrintVector("custom TVector", Vector);
    //std::destroy(Vector.begin(), Vector.end());
    // create custom vector TVector with custom allocator TAllocator and fill Factorial 0 ... 9
    // auto AllocatedVector = InitFactorialVector<CustomVectorAllocated>(30);
    // print custom vector
    // PrintVector("custom TVector allocated", AllocatedVector);

    //CustomList List = InitFactorialList<CustomList>(10);
    //PrintList<CustomList>("custom list" , List);
    return 0;
}