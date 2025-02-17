#include "Rectangle.hpp"
#include "Circle.hpp"
#include "Shape.hpp"

// Constructor
Rectangle::Rectangle(Vec tlPoint, Vec brPoint, Vec velocity, float mass,
                     float elasticity)
    : Shape(velocity, mass, elasticity), tlPoint(tlPoint), brPoint(brPoint) {}

bool Rectangle::collidesWith(Shape &other) {
  return other.collidesWithRectangle(*this);
}

bool Rectangle::collidesWithCircle(Circle &circle) {
  if (RectangleVsCircle(*this, circle)) {
    Vec normal = GetNormalRectangleVsCircle(*this, circle);
    processMovement(*this, circle, normal);
    return true;
  }
  return false;
}

bool Rectangle::collidesWithRectangle(Rectangle &rectangle) {
  if (RectangleVsRectangle(*this, rectangle)) {
    Vec normal = GetNormalRectangleVsRectangle(*this, rectangle);
    processMovement(*this, rectangle, normal);
    return true;
  }
  return false;
}

void Rectangle::move() {
  this->tlPoint.y += this->velocity.y;
  this->tlPoint.x += this->velocity.x;
  this->brPoint.y += this->velocity.y;
  this->brPoint.x += this->velocity.x;
}