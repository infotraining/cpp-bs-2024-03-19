#include "square.hpp"

#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <list>
#include <numeric>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>
#include <functional>
#include <format>

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
        auto mutable_lambda = [x](int y) mutable {
            x += y;
            return x;
        };

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
    return [seed]() mutable {
        return seed++;
    };
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

///////////////////////////////////////////////////////////////////////////////////////////
// callables

bool is_even(uint64_t n)
{
    return n % 2 == 0;
}

struct IsEven
{
    bool operator()(uint64_t n) const
    {
        return n % 2 == 0;
    }
};

auto lambda_is_even = [](uint64_t n) {
    return n % 2 == 0;
};

namespace Explain
{
    namespace ver_1
    {
        template <typename InIter, typename Predicate>
        InIter find_if(InIter first, InIter last, Predicate f)
        {
            for (InIter it = first; it != last; ++it)
            {
                if (f(*it))
                {
                    return it;
                }
            }

            return last;
        }
    } // namespace ver_1

    namespace ver_2
    {
        template <std::input_iterator Iter, typename Predicate>
        Iter find_if(Iter first, Iter last, Predicate f)
        {
            for (Iter it = first; it != last; ++it)
            {
                if (f(*it))
                {
                    return it;
                }
            }

            return last;
        }
    } // namespace ver_1

	inline namespace ver_3
	{
		template <std::input_iterator Iter, typename F>
        Iter find_if(Iter first, Iter last, F f)
			requires std::predicate<F, decltype(*first)>
        {
            for (Iter it = first; it != last; ++it)
            {
                if (f(*it))
                {
                    return it;
                }
            }

            return last;
        }
	}

	namespace ver_4
	{
		template <std::input_iterator Iter, std::predicate<typename std::iterator_traits<Iter>::value_type> F>
        Iter find_if(Iter first, Iter last, F f)
        {
            for (Iter it = first; it != last; ++it)
            {
                if (f(*it))
                {
                    return it;
                }
            }

            return last;
        }
	}

} // namespace Explain

TEST_CASE("callable as func param")
{
    std::vector<int> vec = {1, 3, 5, 8, 9, 11};

    std::random_access_iterator auto it = vec.begin();
    ++(++it);
    --it;
    it = it + 2;

    std::ranges::sort(vec, std::greater{});
    print(vec, "vec");

    auto pos = Explain::ver_4::find_if(vec.begin(), vec.end(), is_even);
    CHECK(pos != vec.end());
    CHECK(*pos == 8);
}

///////////////////////////////////////////////////////////////////
// concepts

template <typename T>
concept Addable = requires(T a, T b) {
    a + b;
};

static_assert(Addable<int>);
static_assert(Addable<std::string>);
static_assert(!Addable<std::vector<int>>);
static_assert(std::integral<int>);
static_assert(!std::integral<double>);

namespace Unconstrained
{
    template <typename T>
    auto add(T a, T b)
    {
        return a + b;
    }
} // namespace Unconstrained

namespace Constrained
{
    inline namespace ver_1
    {
        template <typename T>
            requires Addable<T>
        auto add(T a, T b)
        {
            return a + b;
        }
    } // namespace ver_1

    namespace ver_2
    {
        template <Addable T>
        auto add(T a, T b)
        {
            return a + b;
        }
    } // namespace ver_2
} // namespace Constrained

TEST_CASE("using concepts")
{
    auto str = Unconstrained::add("abc"s, "def"s);
    CHECK(str == "abcdef");

    std::integral auto vec = Constrained::add(5, 665); // std::integral<decltype(vec)>
}

//////////////////////////////////////////////////////////////////////

void do_work(const std::string& desc)
{
	std::cout << "work: " << desc << "\n";
}

struct DoWork
{
	int counter{};

	void operator()(const std::string& desc)
	{
		++counter;
		std::cout << std::format("WORK: {}; counter: {};\n", desc, counter);
	}
};

TEST_CASE("callback")
{
	SECTION("C-style")
	{
		void (*callback)(const std::string&) = nullptr;

		callback = do_work;

		callback("START");
		callback("STOP");

		callback = [](const auto& desc) { std::cout << "Lambda: " << desc << "\n"; };
		callback("HELLO");
	}

	SECTION("modern C++")
	{
		std::function<void(const std::string&)> callback;
		CHECK_THROWS_AS(callback("abc"), std::bad_function_call);

		callback = do_work;
		callback("START");

		callback = DoWork();
		callback("A");
		callback("B");
		callback("C");

		uint64_t counter{};
		callback = [&counter](const auto& desc) { std::cout << std::format("Lambda: {}\n", desc); ++counter; };
		callback("D");
		callback("E");
		callback("F");
		CHECK(counter == 3);
	}
}

struct Robot
{
	int id;
	std::function<void()> on_start;
	std::function<void(std::span<std::byte>)> on_data_ready;

	// Robot(int id, std::function<void(std::span<std::byte>)> data_ready_callback) : id{id}, on_data_ready{data_ready_callback}
	// {
	// 	bootstrap();
	// }

	template <std::invocable<std::span<std::byte>> Callback>
	Robot(int id, Callback data_ready_callback) : id{id}, on_data_ready{data_ready_callback}
	{
		bootstrap();
	}

	void bootstrap()
	{
		on_start = [this](){ std::cout << std::format("Robot#{} has started...", id); };
	}

	void run()
	{
		on_start();

		std::vector<std::byte> data{std::byte{42}, std::byte{45}, std::byte{44}};
		
		if (on_data_ready)
			on_data_ready(data);
	}
};

TEST_CASE("using callbacks")
{
	std::vector<int> data;

	auto on_data_ready = [&data](std::span<std::byte> received) { 
		for(std::byte b : received)
		{
			data.push_back(std::to_integer<int>(b));
		}
	};

	Robot robot(665, on_data_ready);
	robot.run();

	CHECK(data == std::vector{42, 45, 44});
}