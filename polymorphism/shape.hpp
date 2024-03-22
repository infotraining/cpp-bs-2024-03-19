#ifndef SHAPE_HPP
#define SHAPE_HPP

#include "point.hpp"
#include <cassert>

namespace Drawing
{
    // inteface
    class Shape
    {
    public:
        virtual void move(int dx, int dy) = 0;
        virtual void draw() const = 0;
        virtual ~Shape() = default;
    };

    // abstract class - draw() is pure virtual
    class ShapeBase : public Shape
    {
    private:
        Point coord_;

    public:
        explicit ShapeBase(int x = 0, int y = 0)
            : coord_{x, y}
        {
        }

        ShapeBase(Point pt)
            : coord_{pt}
        {
        }

        void move(int dx, int dy) override
        {
            coord_.translate(dx, dy);
        }

    protected:
        Point coord() const
        {
            return coord_;
        }

        void set_coord(const Point& coord)
        {
            coord_ = coord;
        }
    };

    class Circle : public ShapeBase
    {
        uint16_t radius_;

    public:
        explicit Circle(int x = 0, int y = 0, uint16_t r = 0)
            : ShapeBase{x, y}
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

        void draw() const override
        {
            std::cout << "Drawing Circle at " << coord() << " with radius " << radius_ << "\n";
        }
    };

    class Rectangle : public ShapeBase
    {
    public:
        explicit Rectangle(int x = 0, int y = 0, uint16_t w = 0, uint16_t h = 0)
            : ShapeBase{x, y}
            , w_{w}
            , h_{h}
        {
        }

        uint16_t width() const
        {
            return w_;
        }

        void set_width(uint16_t w)
        {
            w_ = w;
        }

        uint16_t height() const
        {
            return w_;
        }

        void set_height(uint16_t h)
        {
            h_ = h;
        }

        void draw() const override
        {
            std::cout << "Drawing Rectangle at " << coord() << " with dimensions (width: " << w_ << ", height: " << h_ << ")\n";
        }

    private:
        uint16_t w_;
        uint16_t h_;
    };

    class Line : public ShapeBase
    {
        Point end_coord_;

    public:
        explicit Line(int x = 0, int y = 0, int end_x = 0, int end_y = 0)
            : ShapeBase{x, y}
            , end_coord_{end_x, end_y}
        {
        }

        void draw() const override
        {
            std::cout << "Drawing Line from " << coord() << " to " << end_coord_ << "\n";
        }

        void move(int dx, int dy) override
        {
            ShapeBase::move(dx, dy); // call move from base class
            end_coord_.translate(dx, dy);
        }
    };

    class Square : public Shape
    {
        Rectangle rect_;
    public:
        explicit Square(int x = 0, int y = 0, uint16_t s = 0)
            : rect_{x, y, s, s}
        {
            assert(rect_.width() == rect_.height());
        }

        int16_t size() const
        {
            return rect_.width();
        }

        void set_size(int16_t s)
        {
            rect_.set_width(s);
            rect_.set_height(s);
            assert(rect_.width() == rect_.height());
        }

        void move(int dx, int dy) override
        {
            rect_.move(dx, dy);
        }

        void draw() const override
        {
            rect_.draw();
        }
    };
} // namespace Drawing

#endif // SHAPE_HPP