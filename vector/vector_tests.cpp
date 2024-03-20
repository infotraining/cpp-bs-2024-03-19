#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <ranges>
#include <algorithm>

using namespace std::literals;

namespace ModernCpp
{
	class Vector
	{};
}

namespace rng = std::ranges;

TEST_CASE("Vector")
{	
	using ModernCpp::Vector;

	SECTION("constructed with size")
	{
		Vector vec(10);
		CHECK(vec.size() == 10)
		CHECK(rng::all_of(vec, [](int x) { return x == 0; }));
	}
}