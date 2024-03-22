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

        void draw() const;
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
        Circle(int x = 0, int y = 0, uint16_t r = 0) : Shape{x, y}, radius_{r}
        {}

        void draw() const
        {
            std::cout << "Drawing Circle at " << coord() << " with radius " << radius_ << "\n";
        }
    };
} // namespace Drawing

#endif // SHAPE_HPP