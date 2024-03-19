#include "square.hpp"

#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <string>
#include <vector>

using namespace std::literals;

namespace MyMath
{
    template <typename T>
    T square(T x) // definition
    {
        return x * x;
    }

    template <typename T1, typename T2>
    auto multiply(T1 x, T2 y)
    {
        return x * y;
    }

    namespace Cpp20
    {
        auto multiply(auto x, auto y)
        {
            return x * y;
        }
    } // namespace Cpp20

	auto describe_even(auto n)
	{
		if (n % 2 == 0)
			return "even"s;
			
		return "odd"s;
	}
} // namespace MyMath

TEST_CASE("square")
{
    CHECK(MyMath::square(10) == 100);

    long n = 1024;

    auto result = MyMath::square(n);

    CHECK(result == 1048576);
}

TEST_CASE("multiply")
{
    CHECK(MyMath::multiply(4, 5) == 20);
    CHECK(MyMath::multiply(4L, 5L) == 20L);

    CHECK(MyMath::multiply(4, 5L) == 20L);
    CHECK(MyMath::multiply(4L, 5) == 20L);

    CHECK(MyMath::Cpp20::multiply(4, 2.1) == 8.4);

	CHECK(MyMath::describe_even(8) == "even"s);
}