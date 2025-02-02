#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

#include "../Utility/Vec.hpp"

class Rectangle {
public:
  Vec min;
  Vec max;
  Vec velocity;
  float mass;
  float elasticity;

  // Constructor
  Rectangle(Vec a, Vec b, Vec c, float d, float e);
};

#endif // RECTANGLE_HPP