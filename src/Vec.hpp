#ifndef VEC_HPP
#define VEC_HPP

#include <iostream>

class Vec {
public:
  double x, y;

  Vec(double x_val = 0, double y_val = 0);

  void display() const;
  Vec add(const Vec &other) const;
  Vec scale(double scalar) const;
};

// Function prototype
Vec add(Vec v1, Vec v2);

#endif // VEC_HPP
