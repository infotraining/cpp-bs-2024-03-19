#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <string>
#include <vector>

#include "square.hpp"

using namespace std::literals;

TEST_CASE("square")
{
	CHECK(MyMath::square(10) == 100);
}