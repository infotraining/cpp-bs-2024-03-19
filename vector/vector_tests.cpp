#include <algorithm>
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>
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

        explicit Vector(size_t size)
            : size_{size}
        {
            items_ = new T[size_]{};
        }

        Vector(std::initializer_list<T> items)
            : size_{items.size()}
            , items_{new T[size_]}
        {
            std::ranges::copy(items, items_);
        }

        ~Vector()
        {
            delete[] items_;
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

        T& operator[](size_t index)
        {
            return items_[index];
        }

		const T& operator[](size_t index) const
        {
            return items_[index];
        }

		bool operator==(const Vector& that) const
		{
			return std::ranges::equal(*this, that);
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
    for (const auto& item : container)
        std::cout << item << " ";
}

void use(ModernCpp::Vector<int> v)
{
    print(v, "v");
}

TEMPLATE_TEST_CASE("Vector - constructor with size", "[Vector][constructors]", int, double, float, std::string)
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

    SECTION("explicit constructor")
    {
        Vector<int> vec(12);
        CHECK(vec.size() == 12);
    }
}

TEST_CASE("Vector - initializer list", "[Vector][constructors]")
{
    using ModernCpp::Vector;

    SECTION("initializer list")
    {
        Vector<int> vec = {1, 2, 3, 4, 5, 6}; // std::initializer_list<int>

        CHECK(vec.size() == 6);
        CHECK(rng::equal(vec, std::vector{1, 2, 3, 4, 5, 6}));
    }

    SECTION("beware")
    {
        Vector<int> vec1(10);
        Vector<int> vec2{10};

        CHECK(not rng::equal(vec1, vec2));
    }
}

TEST_CASE("Vector - indexing", "[Vector][index]")
{
    using ModernCpp::Vector;

    SECTION("reading")
    {
        const Vector<int> vec = {1, 2, 3, 4};
        CHECK(vec[0] == 1);
        CHECK(vec[2] == 3);
        CHECK(vec[3] == 4);
    }

	SECTION("writing")
	{
		Vector<int> vec = {1, 2, 3, 4};
		vec[0] = 665;

		CHECK(vec == Vector{665, 2, 3, 4});
		CHECK(vec != Vector{1, 2, 3, 4}); // !(vec == Vector{1, 2, 3, 4});
	}
}