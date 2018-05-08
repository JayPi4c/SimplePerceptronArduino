#include "Arduino.h"
#include "Vars.h"

#ifndef Point_h
#define Point_h

class Point {
  public:

    float x;
    float y;
    float bias;
    int label;

    Point(float m, float b);
};

#endif



