#ifndef CIRCLE_HPP
#define CIRCLE_HPP

#include "../Utility/Vec.hpp"
#include "Rectangle.hpp"

class Circle {
public:
  Vec center;
  float radius;
  Vec velocity;

  // Constructor
  Circle(Vec a, float b, Vec velocity);
};

#endif // CIRCLE_HPP