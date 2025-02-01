#include "Rectangle.hpp"

// Constructor
Rectangle::Rectangle(Vec a, Vec b, Vec c) : min(a), max(b), velocity(c) {}

// Collision check method
bool RectangleVsRectangle(Rectangle a, Rectangle b) {
  // No collision in x
  if (a.max.x < b.min.x || a.min.x > b.max.x)
    return false;
  // No collision in y
  if (a.max.y < b.min.y || a.min.y > b.max.y)
    return false;

  return true;
}