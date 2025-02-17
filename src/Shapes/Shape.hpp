#ifndef SHAPE_HPP
#define SHAPE_HPP

#include "../Utility/Vec.hpp"

// Use forward declarations instead of includes to avoid errors
class Circle;
class Rectangle;

class Shape {
public:
  Vec velocity;
  float mass;
  float elasticity;

  Shape(const Vec &velocity, float mass, float elasticity)
      : velocity(velocity), mass(mass), elasticity(elasticity) {}

  // Use double dispatch. First dispatch is collision detection, and second
  // dispatch is for the other shape to call the appropriate method.
  virtual bool collidesWith(Shape &other) = 0;

  virtual bool collidesWithCircle(Circle &circle) = 0;
  virtual bool collidesWithRectangle(Rectangle &rect) = 0;

  virtual void move() = 0;           // Move by 1 unit of current velocity.
  virtual void move(Vec &delta) = 0; // Move a specific displacement.
};

void processMovement(Shape &a, Shape &b, Vec normal);

bool RectangleVsRectangle(const Rectangle &r1, const Rectangle &r2);
bool CircleVsCircle(const Circle &c1, const Circle &c2);
bool RectangleVsCircle(const Rectangle &rect, const Circle &circle);

Vec GetNormalRectangleVsRectangle(const Rectangle &r1, const Rectangle &r2);
Vec GetNormalCircleVsCircle(const Circle &c1, const Circle &c2);
Vec GetNormalRectangleVsCircle(const Rectangle &rect, const Circle &circle);

#endif // SHAPES_HPP
