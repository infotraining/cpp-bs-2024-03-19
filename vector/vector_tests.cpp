#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <ranges>
#include <string>
#include <vector>

using namespace std::literals;

namespace ModernCpp
{
    class Vector
    {
    public:
		using iterator = int*;

        Vector(size_t size)
            : size_{size}
        {
			items_ = new int[size_]{};
        }

        size_t size()
        {
            return size_;
        }

		iterator begin()
		{
			return items_;
		}

		iterator end()
		{
			return items_ + size_;
		}

    private:
        size_t size_;
		int* items_;
    };
} // namespace ModernCpp

namespace rng = std::ranges;

TEST_CASE("Vector")
{
    using ModernCpp::Vector;

    SECTION("constructed with size")
    {
        Vector vec(10);
        CHECK(vec.size() == 10);

		SECTION("all items set to zero")
		{
			CHECK(rng::all_of(vec, [](int x) { return x == 0; }));
		}
    }
}