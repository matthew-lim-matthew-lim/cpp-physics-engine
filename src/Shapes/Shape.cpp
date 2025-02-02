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

// Get normal vector of Rectangle vs Rectangle collision
Vec GetNormalRectangleVsRectangle(const Rectangle &r1, const Rectangle &r2) {
  Vec centerA = Vec((r1.min.x + r1.max.x) / 2, (r1.min.y + r1.max.y) / 2);
  Vec centerB = Vec((r2.min.x + r2.max.x) / 2, (r2.min.y + r2.max.y) / 2);

  Vec difference = centerB - centerA;
  // dx and dy are the penetration depth in the x and y axis.
  double dx = (r1.max.x - r1.min.x) / 2 + (r2.max.x - r2.min.x) / 2 -
              std::abs(difference.x);
  double dy = (r1.max.y - r1.min.y) / 2 + (r2.max.y - r2.min.y) / 2 -
              std::abs(difference.y);

  // The normal is determined by the axis with the smallest penetration
  if (dx < dy) {
    // Collision on left/right
    return Vec(difference.x > 0 ? 1 : -1, 0);
  } else {
    // Collision on top/bottom
    return Vec(0, difference.y > 0 ? 1 : -1);
  }
}

// Get normal vector of Circle vs Circle collision
Vec GetNormalCircleVsCircle(const Circle &c1, const Circle &c2) {
  float dx = c1.center.x - c2.center.x;
  float dy = c1.center.y - c2.center.y;
  float magnitude = std::sqrt(dx * dx + dy * dy);
  return (magnitude > 0) ? Vec(dx / magnitude, dy / magnitude) : Vec(0, 0);
}

// Get normal vector of Rectangle vs Circle collision
Vec GetNormalRectangleVsCircle(const Rectangle &rect, const Circle &circle) {
  Vec centerRect((rect.min.x + rect.max.x) / 2, (rect.min.y + rect.max.y) / 2);

  // Find closest point on rectangle to the circle center
  double closestX = std::max(rect.min.x, std::min(circle.center.x, rect.max.x));
  double closestY = std::max(rect.min.y, std::min(circle.center.y, rect.max.y));

  Vec normal(circle.center.x - closestX, circle.center.y - closestY);

  double magnitude = std::sqrt(normal.x * normal.x + normal.y * normal.y);
  return (magnitude > 0) ? Vec(normal.x / magnitude, normal.y / magnitude)
                         : Vec(0, 0);
}

Vec getCollisionNormal(const Shape &a, const Shape &b) {
  auto visitor = [](auto &&shapeA, auto &&shapeB) -> Vec {
    using T1 = std::decay_t<decltype(shapeA)>;
    using T2 = std::decay_t<decltype(shapeB)>;

    if constexpr (std::is_same_v<T1, Rectangle> &&
                  std::is_same_v<T2, Rectangle>) {
      return GetNormalRectangleVsRectangle(shapeA, shapeB);
    } else if constexpr (std::is_same_v<T1, Circle> &&
                         std::is_same_v<T2, Circle>) {
      return GetNormalCircleVsCircle(shapeA, shapeB);
    } else if constexpr (std::is_same_v<T1, Rectangle> &&
                         std::is_same_v<T2, Circle>) {
      return GetNormalRectangleVsCircle(shapeA, shapeB);
    } else if constexpr (std::is_same_v<T1, Circle> &&
                         std::is_same_v<T2, Rectangle>) {
      return GetNormalRectangleVsCircle(shapeB, shapeA);
    } else {
      std::cerr << "Error: getCollisionNormal() called with unrecognised shape "
                   "type (Shape.cpp)"
                << std::endl;
      return Vec(0, 0);
    }
  };
  return std::visit(visitor, a, b);
}