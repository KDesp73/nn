#ifndef NETWORK_H
#define NETWORK_H

#include "lists.h"
#include <stdlib.h>
#include "config.h"

typedef struct {
    double value;
    Doubles weights; // Forward
    double delta;
    double bias;
} Neuron;

Neuron NeuronInit(double value, Doubles weights);
void NeuronFree(Neuron* neuron);

typedef struct {
    size_t neuronCount;  // Number of outputs (neurons) in this layer
    Neuron* neurons;
} Layer;

void LayerInit(Layer* layer, size_t inputCount, size_t outputCount);
void LayerFree(Layer* layer);

typedef struct {
    size_t layerCount;
    Layer layers[MAX_LAYERS];
    double totalLoss;
} Network;

void NetworkInit(Network* network, const Sizes sizes);
void NetworkFree(Network* network);
void NetworkPrint(const Network* network);

void NetworkLoad(Network* network, const char* file);
void NetworkdSave(const Network* network, const char* file);

#endif // NETWORK_H
