#include "square.hpp"

#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <numeric>
#include <ranges>
#include <string>
#include <vector>
#include <string_view>

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

    template <typename TContainer, typename Func>
    void apply(TContainer& container, Func f)
    {
        for (auto& e : container)
        {
            e = f(e);
        }
    }

    template <typename InIter, typename OutIter, typename Func>
    void transform(InIter first, InIter last, OutIter out, Func f)
    {
        for (InIter it = first; it != last; ++it)
        {
            *(out++) = f(*it);
        }
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

    std::vector<int> data = {1, 2, 3, 4, 5};
    MyMath::apply(data, &MyMath::square<int>);
    CHECK(data == std::vector{1, 4, 9, 16, 25});

    MyMath::apply(data, [](int x) { return x * x; });

    std::vector<float> data_fl = {1.1, 2.3, 5.5};
    MyMath::apply(data_fl, &MyMath::square<float>);

    int tab[10] = {1, 2, 3, 4, 5};
    MyMath::apply(tab, [](int x) { return x * 3; });

    SECTION("STL - transform")
    {
        std::vector<int> data = {1, 2, 3, 4, 5};
        std::transform(data.begin(), data.end(), data.begin(), [](int x) { return x * x; });
        std::ranges::transform(data, data.begin(), [](int x) { return x * x; });
        CHECK(data == std::vector{1, 4, 9, 16, 25});
    }
}

struct By2
{
    auto operator()(int n) const
    {
        return n * 2;
    }
};

TEST_CASE("functor")
{
    By2 by_2;
    CHECK(by_2(4) == 8);
}

struct Lambda_423674523867452384 // no capture
{
    auto operator()(int x) const
    {
        return 2 * x;
    }
};

struct Lambda_7234527634582637534 // capture factor by value
{
    const int factor;

    auto operator()(int x) const { return x * factor; }
};

struct Lambda_7689578978476846786 // capture factor by ref
{
    int& factor;

    auto operator()(int x) const { return x * factor; }
};

TEST_CASE("lambda")
{
    auto multiply_by_2 = [](int x) {
        return 2 * x;
    };
    CHECK(multiply_by_2(8) == 16);

    SECTION("is interpreted as")
    {
        auto by_2 = Lambda_423674523867452384{};
        CHECK(multiply_by_2(10) == 20);
    }

    SECTION("lambda with empty [] is convertible to function pointer")
    {
        int (*f)(int) = multiply_by_2;
    }

    SECTION("capturing by value")
    {
        double factor = 2.5;

        auto multiply_by_factor = [factor](int x) {
            return x * factor;
        };

        SECTION("default capture mode")
        {
            auto by_value = [=](int x) {
                return x * factor;
            };
        }

        factor = 10.0; // no effect

        CHECK(multiply_by_factor(10) == 25.0);
    }

    SECTION("capturing by ref")
    {
        double factor = 2.5;

        auto mutiply_by_factor = [&factor](int x) {
            return x * factor++;
        };

		SECTION("default capture mode")
        {
            auto by_value = [&](int x) {
                return x * factor;
            };
        }

        factor = 10.0; // has effect

        CHECK(mutiply_by_factor(10) == 100.0);
        CHECK(factor == 11.0);
    }

    SECTION("capture list")
    {
        std::vector<double> vec = {1, 1.6, 1.8, 1.9, 1.3};

        auto calc_avg = [data = std::as_const(vec)]() {
            return std::accumulate(data.begin(), data.end(), 0.0) / data.size();
        };

        std::cout << "AVG: " << calc_avg() << "\n";
    }

	SECTION("mutable lambda")
	{
		int x = 10;
		auto mutable_lambda = [x](int y) mutable { x += y; return x; };

		CHECK(mutable_lambda(1) == 11);
		CHECK(mutable_lambda(5) == 16);
	}
}

TEST_CASE("sorting - STL & ranges")
{
    std::vector<int> vec = {283746, 5, 45, 345, 665, 2, 35, 6};

    SECTION("classic STL")
    {
        std::sort(vec.begin(), vec.end());
        CHECK(std::is_sorted(vec.begin(), vec.end()));
    }

    SECTION("ranges")
    {
        std::ranges::sort(vec);
        CHECK(std::ranges::is_sorted(vec));
    }
}

auto create_generator(int seed)
{
	return [seed]() mutable { return seed++; };
}

void print(const auto& v, std::string_view name)
{
    std::cout << name << ": ";

    for (const auto& e : v)
    {
        std::cout << e << " ";
    }

    std::cout << "\n";
}

TEST_CASE("generator")
{
	auto gen_1 = create_generator(1);
	CHECK(gen_1() == 1);
	CHECK(gen_1() == 2);
	CHECK(gen_1() == 3);
	CHECK(gen_1() == 4);

	auto gen_2 = create_generator(100);
	CHECK(gen_2() == 100);
	CHECK(gen_2() == 101);
	CHECK(gen_2() == 102);

	std::vector<int> vec(100);
	std::ranges::generate(vec, create_generator(665));

	print(vec, "vec");
}