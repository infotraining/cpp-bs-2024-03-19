#include "shape.hpp"

#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <string>
#include <vector>

using namespace std::literals;

TEST_CASE("polymorphism")
{
    Drawing::Shape shp{10, 20};
    shp.draw();
    shp.move(12, 55);
    shp.draw();
}