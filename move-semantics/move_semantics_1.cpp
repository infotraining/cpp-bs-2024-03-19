#include "helpers.hpp"

#include <catch2/catch_test_macros.hpp>
#include <iostream>

using namespace std::literals;

std::string full_name(const std::string& first_name, const std::string& last_name)
{
    return first_name + " " + last_name;
}

TEST_CASE("reference binding")
{
    std::string name = "jan";

    SECTION("C++98")
    {
        std::string& lref_name = name;
        const std::string& ref_full_name = full_name(name, "Kowalski");
        //ref_full_name[0] = 'J';
    }

    SECTION("C++11")
    {
        std::string&& ref_full_name = full_name(name, "Kowalski");
        ref_full_name[0] = 'J';
        CHECK(ref_full_name == "Jan Kowalski");

        // std::string&& rref_name = name; // LVALUE
    }
}

struct NoDefaultConstructor
{
    int x{};

    NoDefaultConstructor(int x) : x{x}
    {}

    NoDefaultConstructor(const NoDefaultConstructor&) = default; // user declared

    ~NoDefaultConstructor() // user declared
    {
        std::cout << "~NoDefaultConstructor(" << x << ")\n";
    }
};

TEST_CASE("default constructor not declared")
{
    //static_assert(std::is_default_constructible_v<NoDefaultConstructor>, "NoDefaultConstructor is not default constructible");

    NoDefaultConstructor  ndc_1{42};
    NoDefaultConstructor ndc_2 = ndc_1;
    CHECK(ndc_2.x == 42);
}