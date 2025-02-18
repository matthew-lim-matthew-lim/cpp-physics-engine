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

    normal = GetNormalRectangleVsCircle(*this, circle);
    float penetrationDepth = PenetrationDepthRectCircle(*this, circle);
    positionalCorrection(*this, circle, normal, penetrationDepth);
    return true;
  }
  return false;
}

bool Rectangle::collidesWithRectangle(Rectangle &rectangle) {
  if (RectangleVsRectangle(*this, rectangle)) {

    Vec normal = GetNormalRectangleVsRectangle(*this, rectangle);
    processMovement(*this, rectangle, normal);

    normal = GetNormalRectangleVsRectangle(*this, rectangle);
    float penetrationDepth = PenetrationDepthRectRect(*this, rectangle);
    positionalCorrection(*this, rectangle, normal, penetrationDepth);

    return true;
  }
  return false;
}

void Rectangle::move() { this->move(this->velocity); }

void Rectangle::move(Vec delta) {
  this->tlPoint.y += delta.y;
  this->tlPoint.x += delta.x;
  this->brPoint.y += delta.y;
  this->brPoint.x += delta.x;
}