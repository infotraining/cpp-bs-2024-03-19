#include "catch.hpp"

#include <algorithm>
#include <iostream>
#include <memory>
#include <numeric>
#include <string>
#include <vector>

using namespace std;

#ifdef _MSC_VER
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

void foo(int)
{ }

template <typename T>
void deduce1(T arg)
{
    puts(__PRETTY_FUNCTION__);
}

namespace Cpp20
{
    void deduce1(auto arg)
    {
        puts(__PRETTY_FUNCTION__);
    }
}

template <typename T>
void deduce2(T& arg)
{
    puts(__PRETTY_FUNCTION__);
}

template <typename T>
void deduce3(T&& arg)
{
    puts(__PRETTY_FUNCTION__);
}

TEST_CASE("type deduction rules")
{
    int x = 10;
    const int cx = 10;
    int& ref_x = x;
    const int& cref_x = x;
    int tab[10];

    SECTION("Case 1")
    {
        deduce1(x);      // int
        deduce1(cx);     // int - const is removed
        deduce1(ref_x);  // int - ref is removed
        deduce1(cref_x); // int - ref & const are removed
        deduce1(tab);    // int* - array decays to pointer
        deduce1(foo);    // void (*)(int)

        auto a1 = x;      // int
        auto a2 = cx;     // int
        auto a3 = ref_x;  // int
        auto a4 = cref_x; // int
        auto a5 = tab;    // int*
        auto a6 = foo;    // void (*)(int)
    }

    SECTION("Case 2")
    {
        deduce2(x);      // ref is added
        deduce2(cx);     // const int&
        deduce2(ref_x);  // ref is preserved
        deduce2(cref_x); // ref & const are preserved
        deduce2(tab);    // tab size is preserved
        deduce2(foo);    // void (&)(int)

        auto& a1 = x;      // int&
        auto& a2 = cx;     // const int&
        auto& a3 = ref_x;  // int&
        auto& a4 = cref_x; // const int&
        auto& a5 = tab;    // int(&a4)[10]
        auto& a6 = foo;    // void (&)(int)
    }

    SECTION("Case 3")
    {
        deduce3(x);              // deduce3<int&>(int&)
        deduce3(ref_x);          // deduce3<int&>(int&)
        deduce3(cref_x);         // deduce<const int&>(const int&)
        deduce3(tab);            // deduce3<int(&)[10]>(int(&)[10])
        deduce3(foo);            // deduce3<void (&)(int)>(void (&)(int))
        deduce3(cx);             // deduce3<const int&>(const int&)
        deduce3(string("text")); // deduce4<string>(string&&)

        auto&& a1 = x;      // int&
        auto&& a2 = ref_x;  // int&
        auto&& a3 = cref_x; // const int&
        auto&& a4 = tab;    // int(&a4)[10]
        auto&& a5 = foo;    // void (&)(int)
        auto&& a6 = cx;
        auto&& a7 = string("text");
    }
}

TEST_CASE("decltype(auto)")
{ 
    int x = 10;
    decltype(x) ax;

    const int cx = 665;
    decltype(cx) acx = 43;

    std::vector<decltype(x)> vec;

    int& ref_x = x;
    decltype(ref_x) another_ref = x;
}