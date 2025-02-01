#ifndef SHAPE_HPP
#define SHAPE_HPP

#include "Circle.hpp"
#include "Rectangle.hpp"
#include <variant>

// Define a Shape type using std::variant
using Shape = std::variant<Rectangle, Circle>;

bool collidesWith(const Shape &a, const Shape &b);

#endif // SHAPES_HPP
