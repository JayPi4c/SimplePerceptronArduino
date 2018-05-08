#include "Arduino.h"


#ifndef Perceptron_h
#define Perceptron_h

class Perceptron {
  public:
    Perceptron();
    void train(float inputs[], int target);
    int guess(float inputs[]);
    float* getWeights(void);


  private:
    float weights[3] = {};
    float lr = 0.02;
    int sign(float n);
};

#endif
