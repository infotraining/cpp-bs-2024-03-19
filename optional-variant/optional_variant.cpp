#include <array>
#include <catch2/catch_test_macros.hpp>
#include <format>
#include <iostream>
#include <span>
#include <string>
#include <variant>
#include <vector>

using namespace std::literals;

TEST_CASE("optional")
{
    std::optional<int> opt_int;
    CHECK(opt_int.has_value() == false);

    opt_int = 42;
    CHECK(opt_int.has_value() == true);

    if (opt_int)
        std::cout << "value: " << *opt_int << "\n";

    CHECK(opt_int.value() == 42);

    // opt_int.reset();
    opt_int = std::nullopt;

    CHECK_THROWS_AS(opt_int.value(), std::bad_optional_access);
}

namespace WithOptional
{

    template <std::ranges::range TRng, typename F>
    std::optional<typename std::ranges::range_value_t<TRng>> find_if(TRng&& rng, F f)
        requires std::predicate<F, typename std::ranges::range_value_t<TRng>>
    {
        for (const auto& item : rng)
        {
            if (f(item))
            {
                return item;
            }
        }

        return std::nullopt;
    }

    template <std::ranges::range TRng>
    std::optional<typename std::ranges::range_value_t<TRng>> find(TRng&& rng, const auto& value)
    {
        for (const auto& item : rng)
        {
            if (item == value)
            {
                return item;
            }
        }

        return std::nullopt;
    }
} // namespace WithOptional

TEST_CASE("optional returned from function")
{
    std::vector<int> vec = {1, 2, 5, 645, 665, 33};

    std::optional<int> opt_value = WithOptional::find_if(vec, [](int i) { return i == 42; });
    CHECK(opt_value.has_value() == false);

    CHECK(WithOptional::find(vec, 42).value_or(-1) == -1);
}

///////////////////////////////////////////////////////////////////////////////////////////

struct Printer
{
    void operator()(int n) const
    {
        std::cout << "int: " << n << "\n";
    }

    void operator()(double d) const
    {
        std::cout << "double: " << d << "\n";
    }

    void operator()(const std::string& s) const
    {
        std::cout << "string: " << s << "\n";
    }

    void operator()(std::ranges::range auto const& rng) const
    {
        std::cout << "values: [ ";
        for (const auto& item : rng)
            std::cout << item << " ";
        std::cout << "]\n";
    }
};

template <typename... Ts>
struct Overloaded : Ts...
{
    using Ts::operator()...;
};

TEST_CASE("variant")
{
    SECTION("default constructor")
    {
        std::variant<int, double, std::string> v_1;

        CHECK(std::holds_alternative<int>(v_1) == true);
        CHECK(std::get<int>(v_1) == 0);
    }

    SECTION("can hold many different values")
    {
        std::variant<int, double, std::string> v_1 = 42;
        v_1 = 3.14;
        v_1 = "abc"s;

        SECTION("safe access")
        {
            CHECK(std::get<std::string>(v_1) == "abc"s);
            CHECK_THROWS_AS(std::get<int>(v_1), std::bad_variant_access);
        }

        SECTION("unsafe")
        {
            int* ptr_int = std::get_if<int>(&v_1);

            if (ptr_int)
                std::cout << "int: " << *ptr_int << "\n";
            else
                std::cout << "different type inside\n";
        }
    }

    SECTION("visiting variants")
    {
        std::variant<int, double, std::string, std::vector<int>> v_1 = 3.14;
        std::visit(Printer{}, v_1);

        v_1 = std::vector{1, 2, 3};
        std::visit(Printer{}, v_1);

        auto printer = Overloaded{
            [](int n) { std::cout << "int: " << n << "\n"; },
            [](double d) { std::cout << "double: " << d << "\n"; },
            [](const std::string& s) { std::cout << "string: " << s << "\n"; },
            [](std::ranges::range auto const& rng) {
                std::cout << "values: [ ";
                for (const auto& item : rng)
                    std::cout << item << " ";
                std::cout << "]\n";
            }};

        v_1 = 665;

        std::visit(printer, v_1);
    }
}

[[nodiscard]] std::variant<std::string, std::errc> load_file(const std::string& path)
{
    if (path != "")
    {
        return std::errc::bad_file_descriptor;
    }

    return std::format("File {} content...", path);
}

void process_result_with(auto&& result, auto&&... result_visitor)
{
    std::visit(Overloaded{std::forward<decltype(result_visitor)>(result_visitor)...}, result);
}

TEST_CASE("using variant to return value or error")
{
    process_result_with(
        load_file("abc.txb"),
        [](const std::string& result) { std::cout << "result: " << result << "\n"; },
        [](std::errc error_code) { std::cout << "Error: " << std::make_error_code(error_code) << "\n"; });
}

//////////////////////////////////////////////////////////////////////

class Circle
{
    std::tuple<int, int> coord_;
    uint16_t radius_;

public:
    explicit Circle(int x = 0, int y = 0, uint16_t r = 0)
        : coord_{x, y}
        , radius_{r}
    { }

    uint16_t radius() const
    {
        return radius_;
    }

    void set_radius(uint16_t r)
    {
        radius_ = r;
    }

    void draw() const
    {
        std::cout << std::format("Drawing Circle at ({}, {}) with radius {}\n", 
            std::get<0>(coord_), std::get<1>(coord_), radius_);
    }
};

class Square
{
    std::tuple<int, int> coord_;
    uint16_t size_;

public:
    explicit Square(int x = 0, int y = 0, uint16_t r = 0)
        : coord_{x, y}
        , size_{r}
    { }

    uint16_t size() const
    {
        return size_;
    }

    void set_size(uint16_t r)
    {
        size_ = r;
    }

    void draw() const
    {
        std::cout << std::format("Drawing Square at ({}, {}) with radius {}\n", 
            std::get<0>(coord_), std::get<1>(coord_), size_);
    }
};

using ShapeType = std::variant<Circle, Square>;

class Shape
{
    ShapeType shape_;
public:
    Shape(const auto& shp) : shape_{shp}
    {}

    Shape& operator=(const auto& shp) 
    {
        shape_ = shp;

        return *this;
    }

    void draw() const
    {
        std::visit([](auto&& shp) { shp.draw(); }, shape_);
    }
};

TEST_CASE("using variant as polymorphic type")
{
    Shape shp = Circle(10, 20, 50);
    shp.draw();

    shp = Square(99, 434, 54);
    shp.draw();


    std::vector<Shape> shapes;
    shapes.push_back(Circle{1, 2, 4});
    shapes.push_back(Square{7, 4, 6});

    for(const auto& s : shapes)
        s.draw();
}