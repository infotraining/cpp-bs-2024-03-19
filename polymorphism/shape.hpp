#ifndef SHAPE_HPP
#define SHAPE_HPP

#include "point.hpp"

namespace Drawing
{
    // abstract class - draw() is pure virtual
    class Shape
    {
    private:
        Point coord_;

    public:
        Shape(int x = 0, int y = 0)
            : coord_{x, y}
        {
        }

        Shape(Point pt)
            : coord_{pt}
        {
        }

        virtual void move(int dx, int dy)
        {
            coord_.translate(dx, dy);
        }

        virtual void draw() const = 0; // pure-virtual method - abstract method

        virtual ~Shape() = default;

    protected:
        Point coord() const
        {
            return coord_;
        }
    };

    class Circle : public Shape
    {
        uint16_t radius_;

    public:
        Circle(int x = 0, int y = 0, uint16_t r = 0)
            : Shape{x, y}
            , radius_{r}
        { }

        void draw() const override
        {
            std::cout << "Drawing Circle at " << coord() << " with radius " << radius_ << "\n";
        }
    };

    class Rectangle : public Shape
    {
    public:
        Rectangle(int x = 0, int y = 0, uint16_t w = 0, uint16_t h = 0)
            : Shape{x, y}
            , w_{w}
            , h_{h}
        {
        }

        void draw() const override
        {
            std::cout << "Drawing Rectangle at " << coord() << " with dimensions (width: " << w_ << ", height: " << h_ << ")\n";
        }

    private:
        uint16_t w_;
        uint16_t h_;
    };

    class Line : public Shape
    {
        Point end_coord_;
    public:
        Line(int x = 0, int y = 0, int end_x = 0, int end_y = 0)
            : Shape{x, y}, end_coord_{end_x, end_y}
        {
        }

        void draw() const override
        {
            std::cout << "Drawing Line from " << coord() << " to " << end_coord_ << "\n";
        }

        void move(int dx, int dy) override
        {
            Shape::move(dx, dy); // call move from base class
            end_coord_.translate(dx, dy);
        }
    };
} // namespace Drawing

#endif // SHAPE_HPP