#include <algorithm>
#include <array>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <span>

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

            for(const auto& item : data)
            {
                std::cout << item << " ";
            }
            std::cout << "\n";
        }
    }
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
    //cref_x = 665;    
}

TEST_CASE("pointer types")
{
    int x = 10;
    int y = 20;

    int* ptr = &x;
    CHECK(*ptr == 10);
    ptr = &y;
    CHECK(*ptr == 20);

    const int* ptr_to_const = &x;
    // *ptr_to_const = 76;

    int* const const_ptr = &x;
    *const_ptr = 665;
    // const_ptr = &y;
}
