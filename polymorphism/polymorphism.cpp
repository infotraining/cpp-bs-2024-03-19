#include "shape.hpp"

#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace std::literals;

TEST_CASE("polymorphism")
{
    using namespace Drawing;

    // SECTION("Shape")
    // {
    //     Shape shp{10, 20};
    //     shp.draw();
    //     shp.move(12, 55);
    //     shp.draw();
    // }

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

        ShapeBase& ref_shape = rect;
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
    shapes.push_back(std::make_unique<Line>(10, 20, 60, 70));

    SECTION("using base interface")
    {
        for (const auto& shp : shapes)
            shp->draw();

        std::cout << "----------------------\n";

        for (const auto& shp : shapes)
            shp->move(100, 200);

        for (const auto& shp : shapes)
            shp->draw();
    }

    SECTION("downcasting")
    {
        Shape* ptr_shape = shapes[1].get();
        ptr_shape->draw();

        SECTION("unsafe - static_cast")
        {
            Circle* ptr_c = static_cast<Circle*>(ptr_shape);
            ptr_c->set_radius(300);
            ptr_c->draw();
        }

        SECTION("safe - dynamic_cast")
        {
            Circle* ptr_c = dynamic_cast<Circle*>(ptr_shape);

            if (ptr_c)
                ptr_c->set_radius(500);
            else
                std::cout << "Dynamic cast returned nullptr\n";
            
            ptr_shape->draw();

            try
            {
                Rectangle& rect = dynamic_cast<Rectangle&>(*ptr_shape);
            }
            catch(const std::bad_cast& e)
            {
                std::cout << "Error: " << e.what() << "\n";
            }
        }
    }
}

void use_rect(Drawing::Rectangle& rect)
{
    rect.set_width(10);
    rect.set_height(20);
    CHECK(rect.width() * rect.height() == 200);
}

TEST_CASE("Square")
{
    using namespace Drawing;

    Square s{100, 200, 300};
    s.draw();
    s.set_size(600);
    s.draw();
}