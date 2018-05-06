#include "Perceptron.h"


Perceptron::Perceptron() {
  for (int i = 0; i < (sizeof(weights) / sizeof(float)); i++) {
    weights[i] = random(-1, 1);
  }
}

int Perceptron::guess(float inputs[]) {
  float sum = 0;
  for (int i = 0; i < (sizeof(weights) / sizeof(float)); i++) {
    sum += inputs[i] * weights[i];
  }
  int output = sign(sum);
  return output;
}



void Perceptron::train(float inputs[], int target) {
  int guess_ = guess(inputs);
  int error = target - guess_;
  // Tune all the weights
  for (int i = 0; i < (sizeof(weights) / sizeof(float)); i++) {
    weights[i] += error * inputs[i] * lr;
  }
}

int Perceptron::sign(float n) {
  return (n >= 0 ? 1 : -1);
}

float* Perceptron::getWeights(void) {
  return weights;
}



