#ifndef SHAPE_HPP
#define SHAPE_HPP

#include "point.hpp"

namespace Drawing
{
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

        void move(int dx, int dy)
        {
            coord_.translate(dx, dy);
        }

        virtual void draw() const;

        virtual ~Shape() 
        {
            std::cout << "~Shape()\n";
        }

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

        ~Circle() override
        {
            std::cout << "~Circle()\n";
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

        ~Rectangle() override
        {
            std::cout << "~Rectangle()\n";
        }

    private:
        uint16_t w_;
        uint16_t h_;
    };
} // namespace Drawing

#endif // SHAPE_HPP