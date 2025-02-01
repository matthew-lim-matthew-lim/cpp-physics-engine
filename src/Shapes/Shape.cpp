#include "Shape.hpp"
#include <cmath>
#include <iostream>

// Determine if Rectangle vs Rectangle collision
bool RectangleVsRectangle(const Rectangle &r1, const Rectangle &r2) {
  return (r1.max.x > r2.min.x && r1.min.x < r2.max.x && r1.max.y > r2.min.y &&
          r1.min.y < r2.max.y);
}

// Determine if Circle vs Circle collision
bool CircleVsCircle(const Circle &c1, const Circle &c2) {
  float dx = c1.center.x - c2.center.x;
  float dy = c1.center.y - c2.center.y;
  float distanceSquared = dx * dx + dy * dy;
  float radiusSum = c1.radius + c2.radius;
  return distanceSquared <= (radiusSum * radiusSum);
}

// Determine if Rectangle vs Circle collision
bool RectangleVsCircle(const Rectangle &rect, const Circle &circle) {
  float closestX = std::max(rect.min.x, std::min(circle.center.x, rect.max.x));
  float closestY = std::max(rect.min.y, std::min(circle.center.y, rect.max.y));

  float dx = circle.center.x - closestX;
  float dy = circle.center.y - closestY;

  return (dx * dx + dy * dy) <= (circle.radius * circle.radius);
}

// Determine if Figures out which collision function to call
bool collidesWith(const Shape &a, const Shape &b) {
  auto visitor = [](auto &&shapeA, auto &&shapeB) -> bool {
    using T1 = std::decay_t<decltype(shapeA)>;
    using T2 = std::decay_t<decltype(shapeB)>;

    if constexpr (std::is_same_v<T1, Rectangle> &&
                  std::is_same_v<T2, Rectangle>) {
      return RectangleVsRectangle(shapeA, shapeB);
    } else if constexpr (std::is_same_v<T1, Circle> &&
                         std::is_same_v<T2, Circle>) {
      return CircleVsCircle(shapeA, shapeB);
    } else if constexpr (std::is_same_v<T1, Rectangle> &&
                         std::is_same_v<T2, Circle>) {
      return RectangleVsCircle(shapeA, shapeB);
    } else if constexpr (std::is_same_v<T1, Circle> &&
                         std::is_same_v<T2, Rectangle>) {
      return RectangleVsCircle(shapeB, shapeA);
    } else {
      std::cerr << "Error: collidesWith() called with unrecognised shape "
                   "type (Shape.cpp)"
                << std::endl;
      return false;
    }
  };
  // Use std::visit to apply the visitor lambda function to std::variant objects
  // a and b, calling the correct collision function.
  return std::visit(visitor, a, b);
}
