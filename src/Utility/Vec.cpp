#include "Vec.hpp"

// Constructor
Vec::Vec(double x_val, double y_val) : x(x_val), y(y_val) {}

// Display function
void Vec::display() const {
  std::cout << "(" << x << "," << y << ")" << std::endl;
}

// Overloaded operators
Vec operator-(Vec a, Vec b) { return Vec(a.x - b.x, a.y - b.y); }

Vec operator+(Vec a, Vec b) { return Vec(a.x + b.x, a.y + b.y); }

Vec operator*(float scalar, Vec a) { return Vec(a.x * scalar, a.y * scalar); }

Vec operator*(Vec a, float scalar) { return scalar * a; }