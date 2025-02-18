#include "Circle.hpp"
#include "Rectangle.hpp"
#include "Shape.hpp"

Circle::Circle(Vec center, float radius, Vec velocity, float mass,
               float elasticity)
    : Shape(velocity, mass, elasticity), center(center), radius(radius) {}

// Delegate to the appropriate overload in the other shape class
// (First Dispatch)
bool Circle::collidesWith(Shape &other) {
  return other.collidesWithCircle(*this);
}

bool Circle::collidesWithCircle(Circle &circle) {
  if (CircleVsCircle(*this, circle)) {
    Vec normal = GetNormalCircleVsCircle(*this, circle);
    processMovement(*this, circle, normal);
    return true;
  }
  return false;
}

bool Circle::collidesWithRectangle(Rectangle &rectangle) {
  if (RectangleVsCircle(rectangle, *this)) {
    Vec normal = GetNormalRectangleVsCircle(rectangle, *this);
    processMovement(*this, rectangle, normal);
    return true;
  }
  return false;
}

void Circle::move() { this->move(this->velocity); }

void Circle::move(Vec delta) {
  this->center.y += delta.y;
  this->center.x += delta.x;
}