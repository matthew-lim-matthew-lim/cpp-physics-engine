#include "Rectangle.hpp"

class Circle {
public:
  Vec c;
  float radius;
  Vec velocity;

  // Constructor
  Circle(Vec a, float b, Vec velocity);
};

bool CircleVsRectangle(Circle a, Rectangle b);