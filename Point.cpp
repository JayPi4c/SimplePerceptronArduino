#include "Point.h"
Point::Point(float m, float b) {
  x = random(-100, 100);
  y = random(b-100, b+100);
  bias = 1;
  // label = (x > y ? 1 : -1);
  float y_ = (m * x) + b;
  label = (y_ > y ? 1 : -1);
}
