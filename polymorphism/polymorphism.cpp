#include "shape.hpp"

#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <memory>

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

    SECTION("pointers & reference to base class")
    {
        Shape* ptr_shape{};

        Circle crc{10, 20, 50};
        Rectangle rect{77, 99, 100, 200};

        ptr_shape = &crc;
        ptr_shape->draw();

        ptr_shape = &rect;
        ptr_shape->draw();

        Shape& ref_shape = rect;
        ref_shape.draw();
    }
}

TEST_CASE("polymorphic objects")
{
    using namespace Drawing;

    std::vector<std::unique_ptr<Shape>> shapes;

    shapes.push_back(std::make_unique<Rectangle>(10, 20, 60, 55));
    shapes.push_back(std::make_unique<Circle>(17, 87, 60));
    shapes.push_back(std::make_unique<Rectangle>(7, 78, 66, 33));
    shapes.push_back(std::make_unique<Circle>(78, 80, 30));

    for(const auto& shp : shapes)
        shp->draw();   
}