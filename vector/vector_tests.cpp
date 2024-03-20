#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <iostream>
#include <ranges>
#include <string>
#include <vector>

using namespace std::literals;

namespace ModernCpp
{
	template <typename T>
    class Vector
    {
    public:
		using iterator = T*;
		using const_iterator = const T*;

        Vector(size_t size)
            : size_{size}
        {
			items_ = new T[size_]{};
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
		T* items_;
    };
} // namespace ModernCpp

namespace rng = std::ranges;

void print(const auto& container, std::string_view desc = "data")
{
	std::cout << desc << ": ";
	for(const auto& item : container)
		std::cout << item << " ";
}

TEMPLATE_TEST_CASE("Vector", "[Vector][constructors]", int, double, float, std::string)
{
    using ModernCpp::Vector;

    SECTION("constructed with size")
    {
        Vector<TestType> vec(10);
		print(vec, "vec");
        CHECK(vec.size() == 10);

		SECTION("all items set to zero")
		{
			CHECK(rng::all_of(vec, [](auto x) { return x == TestType{}; }));
		}
    }
}