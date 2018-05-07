#include "Point.h"
Point::Point() {
  x = random(-10, 10);
  y = random(b-10, b+10);
  bias = 1;
  // label = (x > y ? 1 : -1);
  float y_ = (m * x) + b;
  label = (y_ > y ? 1 : -1);
}
