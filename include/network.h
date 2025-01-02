#ifndef NETWORK_H
#define NETWORK_H

#include "lists.h"
#include <stdlib.h>
#include "config.h"


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


#endif // NETWORK_H
