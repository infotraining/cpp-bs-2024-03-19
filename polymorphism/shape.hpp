#ifndef SHAPE_HPP
#define SHAPE_HPP

#include "point.hpp"

namespace Drawing
{
    class Shape
    {
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
    };

} // namespace Drawing

#endif // SHAPE_HPP