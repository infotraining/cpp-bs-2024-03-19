#include <algorithm>
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <ranges>
#include <string>
#include <vector>
#include <string_view>
#include <array>

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
			print("Vector constructed");
        }

        Vector(std::initializer_list<T> items)
            : size_{items.size()}
            , items_{new T[size_]}
        {
            std::ranges::copy(items, items_);
			print("Vector constructed");
        }

        // copy constructor
        Vector(const Vector& source)
            : size_{source.size()}
            , items_{new T[size_]}
        {
            std::ranges::copy(source, items_);
			print("Vector copy constructor");
        }

        // copy assignment
        Vector& operator=(const Vector& source)
        {
            if (this != &source) // avoiding self assignment
            {
                delete[] items_;

                size_ = source.size_;
                items_ = new T[size_];
                std::ranges::copy(source, items_);
            }

			// Vector temp(source); // cc
			// swap(temp);
			print("Vector - copy assignment");

            return *this;
        }

        ~Vector()
        {
			print("Vector - destructor");
            delete[] items_;
        }

		void swap(Vector& that)
		{
			std::swap(this->size_, that.size_);
			std::swap(this->items_, that.items_);
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

		void print(std::string_view desc) const
		{
			std::cout << desc << ": [ ";
			for(const auto& item : *this)
			{
				std::cout << item << " ";
			}
			std::cout << "]\n";
		}
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

namespace ModernCpp
{
    Vector<int> create_vector(size_t size)
    {
		// Vector<int> vec(size);
		// //... push_backs
		// return vec; // lvalue - may be optimized by copy elision

        return Vector<int>(size); // rvalue - copy elision is guaranteed
    }
} // namespace ModernCpp

TEST_CASE("Vector - copy", "[Vector][copy]")
{
    using namespace ModernCpp;

    SECTION("copy constructor")
    {
        Vector<int> source = {1, 2, 3, 4, 5};
        Vector<int> backup = source; // copy
        // Vector<int> backup(source);  // copy

        CHECK(source == backup);
    }

    SECTION("copy assignment")
    {
        Vector<int> vec_1 = {1, 2, 3, 4};
        Vector<int> vec_2 = {5, 6, 7};

        vec_1 = vec_2; // copy assignment

        CHECK(vec_1 == vec_2);
    }

	SECTION("copy elision")
	{
		Vector<int> vec = create_vector(1'000'000);
	}
}

TEST_CASE("Vector - swap", "[Vector][swap]")
{
	using namespace ModernCpp;

	Vector<int> vec_1 = {1, 2, 3, 4};
    Vector<int> vec_2 = {5, 6, 7};

	vec_1.swap(vec_2);
	CHECK(vec_1 == Vector{5, 6, 7});
	CHECK(vec_2 == Vector{1, 2, 3, 4});
}

//////////////////////////////////////////////////////////////////////////////
// operator ==

// enum class BaudRate : uint16_t
// {	
// };

struct BaudRate
{
	int value;

	constexpr BaudRate(int value) : value(value)
	{
		constexpr std::array<uint16_t, 4> valid_values = {100, 200, 300, 400};
		
		if (std::ranges::find(valid_values, value) == valid_values.end())
			throw std::invalid_argument{"Invalid value"};
	}

	bool operator==(const BaudRate&) const = default;
};


consteval BaudRate operator ""_br(unsigned long long value)
{
	// constexpr std::array<uint16_t, 4> valid_values = {100, 200, 300, 400};
	// static_assert(std::ranges::find(valid_values, value) != valid_values.end());
	
	return BaudRate{static_cast<uint16_t>(value)};
}

struct Timeout
{
	uint16_t value;

	bool operator==(const Timeout&) const = default;
};

Timeout operator ""_ms(unsigned long long value)
{
	return Timeout{static_cast<uint16_t>(value)};
}

Timeout operator ""_s(unsigned long long value)
{
	return Timeout{static_cast<uint16_t>(value * 1000)};
}


// void connect(int rate, int timeout)
// {}

void connect(BaudRate rate, Timeout t)
{
	if (t == Timeout{2000})
	{
		std::cout << "Waiting 2s...\n";
	}
}

TEST_CASE("using strong typing")
{
	int br = 100;
	int timeout = 2000;

	connect(BaudRate{100}, Timeout{2000});
	connect(200_br, 2000_ms);
	connect(100_br, 2_s);
	//connect(1000_br, 2_s);
}