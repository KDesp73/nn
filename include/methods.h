#ifndef METHODS_H
#define METHODS_H

#include "network.h"

double CostDerivative(double outputActivations, double y);
void Backward(Layer *layer, double *inputs, double *nextDeltas, double *nextWeights, int nextOutputCount, double *targets, double learningRate);
void Forward(Layer *layer, double *inputs);
void TrainNetwork(Network *nn, double **inputs, double **targets, int sampleCount, int epochCount);


#endif // METHODS_H
