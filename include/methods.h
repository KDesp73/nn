#ifndef METHODS_H
#define METHODS_H

#include "network.h"

double CostDerivative(double outputActivations, double y);
void Backward(
    Network *network,
    double *inputs,
    double *targets,
    size_t sampleCount,
    double learningRate
);
void Forward(Network* network);
void TrainNetwork(Network *nn, double **inputs, double **targets, int sampleCount, int epochCount);


#endif // METHODS_H
