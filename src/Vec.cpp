#include "Vec.hpp"

// Constructor
Vec::Vec(double x_val, double y_val) : x(x_val), y(y_val) {}

// Display function
void Vec::display() const {
  std::cout << "(" << x << "," << y << ")" << std::endl;
}

// Add method
Vec Vec::add(const Vec &other) const { return Vec(x + other.x, y + other.y); }

// Scale method
Vec Vec::scale(double scalar) const { return Vec(x * scalar, y * scalar); }

// Standalone add function
Vec add(Vec v1, Vec v2) { return v1.add(v2); }
