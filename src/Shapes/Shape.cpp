#include "Shape.hpp"
#include "Circle.hpp"
#include "Rectangle.hpp"
#include <cmath>
#include <iostream>

// Determine if Rectangle vs Rectangle collision
bool RectangleVsRectangle(const Rectangle &r1, const Rectangle &r2) {
  return (r1.brPoint.x > r2.tlPoint.x && r1.tlPoint.x < r2.brPoint.x &&
          r1.brPoint.y > r2.tlPoint.y && r1.tlPoint.y < r2.brPoint.y);
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
  float closestX =
      std::max(rect.tlPoint.x, std::min(circle.center.x, rect.brPoint.x));
  float closestY =
      std::max(rect.tlPoint.y, std::min(circle.center.y, rect.brPoint.y));

  float dx = circle.center.x - closestX;
  float dy = circle.center.y - closestY;

  return (dx * dx + dy * dy) <= (circle.radius * circle.radius);
}

// Get normal vector of Rectangle vs Rectangle collision
Vec GetNormalRectangleVsRectangle(const Rectangle &r1, const Rectangle &r2) {
  Vec centerA =
      Vec((r1.tlPoint.x + r1.brPoint.x) / 2, (r1.tlPoint.y + r1.brPoint.y) / 2);
  Vec centerB =
      Vec((r2.tlPoint.x + r2.brPoint.x) / 2, (r2.tlPoint.y + r2.brPoint.y) / 2);

  Vec difference = centerB - centerA;
  // dx and dy are the penetration depth in the x and y axis.
  double dx = (r1.brPoint.x - r1.tlPoint.x) / 2 +
              (r2.brPoint.x - r2.tlPoint.x) / 2 - std::abs(difference.x);
  double dy = (r1.brPoint.y - r1.tlPoint.y) / 2 +
              (r2.brPoint.y - r2.tlPoint.y) / 2 - std::abs(difference.y);

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
  Vec centerRect((rect.tlPoint.x + rect.brPoint.x) / 2,
                 (rect.tlPoint.y + rect.brPoint.y) / 2);

  // Find closest point on rectangle to the circle center
  double closestX =
      std::max(rect.tlPoint.x, std::min(circle.center.x, rect.brPoint.x));
  double closestY =
      std::max(rect.tlPoint.y, std::min(circle.center.y, rect.brPoint.y));

  Vec normal(circle.center.x - closestX, circle.center.y - closestY);

  double magnitude = std::sqrt(normal.x * normal.x + normal.y * normal.y);
  return (magnitude > 0) ? Vec(normal.x / magnitude, normal.y / magnitude)
                         : Vec(0, 0);
}

float DotProduct(Vec &a, Vec &b) { return a.x * b.x + a.y * b.y; }

const float INFINITE_MASS = 0;

void processMovement(Shape &a, Shape &b, Vec normal) {
  float aInverseMass = a.mass == INFINITE_MASS ? 0 : 1 / a.mass;
  float bInverseMass = b.mass == INFINITE_MASS ? 0 : 1 / b.mass;

  Vec rv = b.velocity - a.velocity;
  float velAlongNormal = DotProduct(rv, normal);

  if (velAlongNormal > 0)
    return; // Objects are separating

  float e = std::min(a.elasticity, b.elasticity);
  float j = -(1 + e) * velAlongNormal;
  j /= aInverseMass + bInverseMass;

  std::cout << "a.velocity: " << (a.velocity).y << std::endl;
  std::cout << "b.velocity: " << (b.velocity).y << std::endl;

  const Vec impulse = j * normal;
  a.velocity -= aInverseMass * impulse;
  b.velocity += bInverseMass * impulse;

  std::cout << "aInverseMass * impulse: " << (aInverseMass * impulse).y
            << std::endl;
  std::cout << "bInverseMass * impulse: " << (bInverseMass * impulse).y
            << std::endl;

  std::cout << "a.velocity: " << (a.velocity).y << std::endl;
  std::cout << "b.velocity: " << (b.velocity).y << std::endl;
}