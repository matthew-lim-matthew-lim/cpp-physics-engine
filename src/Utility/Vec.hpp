#ifndef VEC_HPP
#define VEC_HPP

#include <iostream>

class Vec {
public:
  double x, y;

  Vec(double x_val = 0, double y_val = 0);

  void display() const;
};

// Overloaded operators
Vec operator-(Vec a, Vec b);

Vec operator+(Vec a, Vec b);

Vec operator*(float scalar, Vec a);

Vec operator*(Vec a, float scalar);

#endif // VEC_HPP
