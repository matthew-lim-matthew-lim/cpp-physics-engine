#include "Circle.hpp"

Circle::Circle(Vec a, float b, Vec c) : c(a), radius(b), velocity(c) {}

bool CircleVsRectangle(Circle a, Rectangle b) {
  // No collision in x
  if (a.c.y + a.radius < b.min.y || a.c.y - a.radius > b.max.y)
    return false;

  // No collision in y
  if (a.c.x + a.radius < b.min.x || a.c.x - a.radius > b.max.x)
    return false;

  return true;
}