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
		using const_iterator = const int *;

        Vector(size_t size)
            : size_{size}
        {
			items_ = new int[size_]{};
        }

        size_t size() const
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

		const_iterator begin() const
		{
			return items_;
		}

		const_iterator end() const
		{
			return items_ + size_;
		}

		const_iterator cbegin() const
		{
			return items_;
		}

		const_iterator cend() const
		{
			return items_ + size_;
		}

    private:
        size_t size_;
		int* items_;
    };
} // namespace ModernCpp

namespace rng = std::ranges;

void print(const auto& container, std::string_view desc = "data")
{
	std::cout << desc << ": ";
	for(const auto& item : container)
		std::cout << item << " ";
}

TEST_CASE("Vector")
{
    using ModernCpp::Vector;

    SECTION("constructed with size")
    {
        Vector vec(10);
		print(vec, "vec");
        CHECK(vec.size() == 10);

		SECTION("all items set to zero")
		{
			CHECK(rng::all_of(vec, [](int x) { return x == 0; }));
		}
    }
}