#include "shape.hpp"

#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <string>
#include <vector>

using namespace std::literals;

TEST_CASE("polymorphism")
{
    using namespace Drawing;

    SECTION("Shape")
    {
        Shape shp{10, 20};
        shp.draw();
        shp.move(12, 55);
        shp.draw();
    }

    SECTION("Circle")
    {
        Circle crc{10, 20, 50};
        crc.draw();
        crc.move(33, 66);
        crc.draw();
    }

    SECTION("Rectangle")
    {
        Rectangle rect{10, 20, 100, 200};
        rect.draw();
        rect.move(44, 55);
        rect.draw();
    }
}