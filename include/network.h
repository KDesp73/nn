#ifndef NETWORK_H
#define NETWORK_H

#include "lists.h"
#include <math.h>
#include <stdlib.h>

#define MAX_LAYERS 128
typedef struct {
    size_t inputCount;   // Number of inputs to this layer
    size_t outputCount;  // Number of outputs (neurons) in this layer
    Doubles weights;     // Array of weights (inputCount * outputCount)
    Doubles biases;      // Array of biases (outputCount)
    Doubles outputs;     // Array of outputs (outputCount)
    Doubles deltas;      // Array of deltas (outputCount)
} Layer;

void LayerInit(Layer* layer, size_t inputCount, size_t outputCount);
void LayerFree(Layer* layer);


typedef struct {
    size_t layerCount;
    Layer layers[MAX_LAYERS];
} Network;

void NetworkInit(Network* network, const Sizes sizes);
void NetworkFree(Network* network);
void NetworkPrint(const Network* network);

static inline double sigmoid(double z)
{
    return 1.0 / (1.0 + exp(-z));
}

static inline double sigmoidPrime(double z)
{
    return sigmoid(z)*(1-sigmoid(z));
}

double FeedForward(const Network *net, double a);
void UpdateMiniBatch(Network *net, double **miniBatch, int miniBatchSize, double eta);
double CostDerivative(double outputActivations, double y);


#endif // NETWORK_H
