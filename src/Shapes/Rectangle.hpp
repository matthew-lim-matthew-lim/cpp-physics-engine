#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

#include "../Utility/Vec.hpp"

class Rectangle {
public:
  Vec min;
  Vec max;
  Vec velocity;

  // Constructor
  Rectangle(Vec a, Vec b, Vec c);
};

#endif // RECTANGLE_HPP