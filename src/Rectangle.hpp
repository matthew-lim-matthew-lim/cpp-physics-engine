#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

#include "Vec.hpp"

class Rectangle {
public:
  Vec min;
  Vec max;
  Vec velocity;

  // Constructor
  Rectangle(Vec a, Vec b, Vec c);
};

bool RectangleVsRectangle(Rectangle a, Rectangle b);

#endif // RECTANGLE_HPP