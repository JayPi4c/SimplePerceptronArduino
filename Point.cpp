#include "Point.h"
Point::Point() {
  x = random(100);
  y = random(100);
  bias = 1;
  // label = (x > y ? 1 : -1);
  float y_ = (a * x) + b;
  label = (y_ > y ? 1 : -1);
}
