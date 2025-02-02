#include "Rectangle.hpp"

// Constructor
Rectangle::Rectangle(Vec a, Vec b, Vec c, float d, float e)
    : min(a), max(b), velocity(c), mass(d), elasticity(e) {}