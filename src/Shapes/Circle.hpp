#ifndef CIRCLE_HPP
#define CIRCLE_HPP

#include "../Utility/Vec.hpp"
#include "Shape.hpp"

class Circle : public Shape {
public:
  Vec center;
  float radius;

  // Constructor
  Circle(Vec a, float b, Vec velocity, float d, float e);

  bool collidesWith(Shape &other) override;

  bool collidesWithCircle(Circle &circle) override;
  bool collidesWithRectangle(Rectangle &rect) override;

  void move() override;
  void move(Vec &delta) override;
};

#endif // CIRCLE_HPP