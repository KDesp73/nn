#ifndef METHODS_H
#define METHODS_H

#include "data.h"
#include "network.h"

double CostDerivative(double outputActivations, double y);
void Backward(
    Network *network,
    double *targets,
    size_t sampleCount,
    double learningRate
);
void Forward(Network* network);
void TrainNetwork(Network *nn, const TrainingData* data, int epochCount);


#endif // METHODS_H
