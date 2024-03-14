#include "shape.hpp"

void Drawing::Shape::draw() const
{
	std::cout << "Drawing Shape at " << coord_ << "\n";
}
