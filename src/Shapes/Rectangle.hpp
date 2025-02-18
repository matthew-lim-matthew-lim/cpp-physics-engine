#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

#include "../Utility/Vec.hpp"
#include "Shape.hpp"

class Rectangle : public Shape {
public:
  Vec tlPoint; // Top Left Point
  Vec brPoint; // Bottom Right Point

  // Constructor
  Rectangle(Vec tlPoint, Vec brPoint, Vec velocity, float mass,
            float elasticity);

  bool collidesWith(Shape &other) override;

  bool collidesWithCircle(Circle &circle) override;
  bool collidesWithRectangle(Rectangle &rect) override;

  void move() override;
  void move(Vec delta) override;
};

#endif // RECTANGLE_HPP