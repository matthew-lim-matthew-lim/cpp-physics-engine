#ifndef CIRCLE_HPP
#define CIRCLE_HPP

#include "../Utility/Vec.hpp"

class Circle {
public:
  Vec center;
  float radius;
  Vec velocity;
  float mass;
  float elasticity;

  // Constructor
  Circle(Vec a, float b, Vec velocity, float d, float e);
};

#endif // CIRCLE_HPP