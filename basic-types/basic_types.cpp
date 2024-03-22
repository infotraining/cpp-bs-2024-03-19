#include <algorithm>
#include <array>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <numeric>
#include <ranges>
#include <span>
#include <string>
#include <tuple>
#include <vector>

using namespace std::literals;

TEST_CASE("integers")
{
    SECTION("raw types")
    {
        int a = 1;
        unsigned int ua = 5345;
        char n = 34;
        n = -45;
        unsigned char un = 33;
        unsigned char un_2 = -45;
        long l = 6576576;
        unsigned long ul = 4234;
    }

    SECTION("aliases")
    {
        int32_t a = -1;
        uint32_t b = 42;
        int8_t c = 'H';
        uint8_t uc = 128;
        int64_t l = 534534;

        intmax_t sum = 423423;
    }

    SECTION("type suffix")
    {
        auto x = 42;
        auto lx = 42L;
        auto ux = 42U;
        auto ulx = 42UL;
    }
}

TEST_CASE("floating points")
{
    double dx = 0.53453;
    float fx = 0.534534;

    auto adx = 0.534534;
    auto afx = 0.53453F;

    decltype(afx) new_variable = 43;

    static_assert(std::is_same_v<decltype(afx), float>, "afx must be float"); // decltype(afx) == float
}

TEST_CASE("boolean values")
{
    bool flag = true;
    flag = false;

    CHECK(!flag);
}

TEST_CASE("strings")
{
    SECTION("c-string")
    {
        const char* c_txt = "Text";
        const char* other_c_txt = "Text";

        CHECK(std::strcmp(c_txt, other_c_txt) == 0);
        CHECK(std::strlen(c_txt) == 4);

        SECTION("literal")
        {
            auto c_str = "abc"; // const char*
        }
    }

    SECTION("std::string")
    {
        std::string txt = "Text";
        std::string other_txt = "Text";
        CHECK(txt == other_txt);

        other_txt += "!!!";
        CHECK(other_txt == "Text!!!");

        other_txt = other_txt + txt;
        other_txt += txt;

        std::cout << "other_txt: " << other_txt << "\n";

        SECTION("literal")
        {
            auto str = "abc"s;
        }
    }

    SECTION("string_view")
    {
        std::string_view sv_1 = "text";

        std::string str = "text";
        std::string_view sv_2 = str;

        CHECK(sv_1 == sv_2);
    }

    SECTION("raw-string")
    {
        std::string path_1 = "c:\\nasz\\tekst\\";
        std::string path_2 = R"(c:\nasz\tekst\)";

        CHECK(path_1 == path_2);

        auto lines = R"(one
two
three
)";

        CHECK(lines == "one\ntwo\nthree\n"s);
    }
}

namespace API
{
    namespace CStyle
    {
        void consume_tab(int* tab, size_t size, const char* desc = "tab")
        {
            std::cout << desc << ": ";

            for (int* it = tab; it != tab + size; ++it)
                std::cout << *it << " ";
            std::cout << "\n";
        }
    } // namespace CStyle

    namespace ModernCpp
    {
        void consume_tab(std::span<const int> data, std::string_view desc = "data")
        {
            std::cout << desc << ": ";

            for (const auto& item : data)
            {
                std::cout << item << " ";
            }
            std::cout << "\n";
        }
    } // namespace ModernCpp
} // namespace API

TEST_CASE("vector")
{
    using namespace API;

    int c_tab[10] = {1, 2, 3, 4, 5};
    CStyle::consume_tab(c_tab, 10);

    std::array<int, 10> arr = {1, 2, 3, 4, 5};
    CHECK(arr[1] == 2);
    CHECK(arr.size() == 10);
    CHECK_THROWS_AS(arr.at(100), std::out_of_range);
    CStyle::consume_tab(arr.data(), arr.size());

    int size = 20; // runtime value
    int* dynamic_tab = new int[size];
    std::fill_n(dynamic_tab, size, 0); // algorithm
    CStyle::consume_tab(dynamic_tab, size);

    std::vector<int> vec = {1, 2, 3, 4, 5};
    vec.resize(10);
    CStyle::consume_tab(vec.data(), vec.size());

    ModernCpp::consume_tab(std::span{c_tab, 10});
    ModernCpp::consume_tab(arr);
    ModernCpp::consume_tab(std::span{dynamic_tab, static_cast<size_t>(size)});
    ModernCpp::consume_tab(vec, "vec");

    std::string desc_vec = "vector<int>";
    ModernCpp::consume_tab(vec, desc_vec);

    delete[] dynamic_tab;
}

TEST_CASE("span")
{
    std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    std::span<int> vec_fragment{vec.begin() + 3, 4};

    API::ModernCpp::consume_tab(vec_fragment);

    std::fill(vec_fragment.begin(), vec_fragment.end(), 0);

    API::ModernCpp::consume_tab(vec);
}

TEST_CASE("reference types")
{
    int x = 10;
    int& ref_x = x;
    ref_x = 20;
    CHECK(x == 20);

    const int& cref_x = x;
    // cref_x = 665;
}

TEST_CASE("pointer types")
{
    int x = 10;
    int y = 20;

    // int* ptr = nullptr;
    int* ptr{};
    ptr = &x;
    CHECK(*ptr == 10);
    ptr = &y;
    CHECK(*ptr == 20);

    const int* ptr_to_const = &x;
    // *ptr_to_const = 76;

    int* const const_ptr = &x;
    *const_ptr = 665;
    // const_ptr = &y;
}

TEST_CASE("universal init syntax")
{
    SECTION("primitive types")
    {
        int x1;
        int x2(10);
        // int x3(); // most vexing parse
        int x4 = 10;

        int ux2{10};
        int ux3{};

        // uint32_t n{ux2}; // narrowing conversion
    }

    SECTION("aggregates")
    {
        struct X
        {
            int a;
            double dx;
        };

        X x_1{10, 0.1};
        X x_2(10, 0.1);

        int tab[10] = {1, 2, 3, 4};
    }

    SECTION("classes")
    {
        struct X
        {
            int a;
            double dx;

            X(int a, double dx)
                : a{a}
                , dx{dx}
            {
            }
        };

        X x_2(10, 0.1);
        X ux_2{10, 0.1};

        std::vector<int> vec = {1, 2, 3};
    }
}

struct TupleLike
{
    int m1;
    double m2;
    std::string m3;
    std::vector<int> m4;
};

std::tuple<int, int, double> calc_stats(std::ranges::range auto const& data)
{
    // std::pair<std::vector<int>::const_iterator, std::vector<int>::const_iterator> minmax_pos = std::minmax_element(data.begin(), data.end());

    // int min = *minmax_pos.first;
    // int max = *minmax_pos.second;

    const auto [min_pos, max_pos] = std::minmax_element(std::ranges::begin(data), std::ranges::end(data)); // Structured Bindings auto [x, y, z] = tuple{1, 2, 3}

    double avg = std::accumulate(std::ranges::begin(data), std::ranges::end(data), 0.0) / std::ranges::size(data);

    return std::tuple{*min_pos, *max_pos, avg};
}

TEST_CASE("tuples")
{
    std::tuple<int, double, std::string, std::vector<int>> tpl_1{42, 3.14, "abc", std::vector{1, 2, 3}};

    // since C++17 - CTAD
    std::tuple tpl_2{1, 3.14, "abc"s}; // std::tuple<int, double, std::string>

    int tab[8] = {1, 2, 3, 4, 5, 6, 7, 8};

    auto [min, max, avg] = calc_stats(tab);

    std::cout << std::format("min:{}; max:{}, avg:{}\n", min, max, avg);
}

template <typename TContainer>
void print(TContainer&& container, std::string_view desc = "data")
{
    std::cout << desc << ": ";
    for (const auto& item : container)
        std::cout << item << " ";
    std::cout << "\n";
}

TEST_CASE("ranges")
{
    int tab[8] = {1, 2, 3, 4, 5, 6, 7, 8};

    auto evens = tab | std::views::filter([](int x) { return x % 2 == 0; });
    print(evens, "data");

    for (auto& item : evens)
    {
        item = 0;
    }

    print(tab, "tab");

    auto squares = std::views::iota(1)
        | std::views::filter([](int x) { return x > 3; })
        | std::views::transform([](int x) { return x * x; })
        | std::views::take(10);

    print(squares, "squares");

    std::string str = "abc def ghi";

    std::string_view delim = " "sv;

    for (const auto& token : str | std::views::split(delim))
    {
        std::cout << "size: " << token.size() << " - ";
        print(token, "token");
    }
}

// namespace Cpp23
// {
//     std::generator<int> get_squares(auto&& rng data)
//     {
//         for (const auto& item : data)
//             co_yield item* item;
//     }

//     TEST_CASE("generator")
//     {
//         for (int x : get_squares(std::vector{1, 2, 3}))
//         {
//             std::cout << x << "\n";
//         }
//     }
// } // namespace Cpp23

///////////////////////////////////////////////////////////////////////
// comparisons - pre C++20

namespace Comparisons
{
    namespace BeforeCpp20
    {
        struct Data
        {
            int x;
            double dx;
            std::string str;

            auto tied() const
            {
                return std::tie(x, dx, str);
            }

            bool operator==(const Data& other) const
            {
                // return x == other.x && dx == other.dx && str == other.str;
                return tied() == other.tied();
            }

            bool operator!=(const Data& other) const
            {
                return !(*this == other);
            }

            bool operator<(const Data& other) const
            {
                // if (x == other.x)
                // {
                //     if (dx == other.dx)
                //     {
                //         return str < other.str;
                //     }

                //     return dx < other.dx;
                // }

                // return x < other.x;

                // return std::tie(x, dx, str) < std::tie(other.x, other.dx, other.str);

                return tied() < other.tied();
            }
        };
    } // namespace BeforeCpp20

    inline namespace Cpp20
    {
        struct Data
        {
            int x;
            double dx;
            std::string str;

            //bool operator(const Data&) const = default; 

            auto operator<=>(const Data&) const = default;
        };
    } // namespace BeforeCpp20
} // namespace Comparisons

TEST_CASE("tie")
{
    int x;
    double dx;

    std::tuple<int&, double&> ref_tpl{x, dx}; // tuple with refs

    ref_tpl = std::tuple{1, 3.14};
    CHECK(x == 1);
    CHECK(dx == 3.14);

    CHECK(std::tie(x, dx) == std::tuple{1, 3.14});
}

TEST_CASE("comparisons - < C++20")
{
    using namespace Comparisons;

    Data d1{1, 3.14, "abc"};
    Data d2{1, 3.14, "abc"};
    Data d3{1, 3.14, "def"};

    CHECK(d1 == d2);
    CHECK(d1 != d2);
    CHECK(d1 < d3);
}
