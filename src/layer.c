#include "lists.h"
#include "misc.h"
#include "network.h"
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

void LayerInit(Layer* layer, size_t inputCount, size_t outputCount)
{
    layer->neuronCount = inputCount;

    layer->neurons = (Neuron*) malloc(sizeof(Neuron) * inputCount);
    if(!layer->neurons){
        fprintf(stderr, "Failed to allocate memory for layer->neurons");
        exit(1);
    }

    for (int i = 0; i < layer->neuronCount; i++) {
        layer->neurons[i] = NeuronInit(randomFloat(), DoublesAlloc(outputCount));
        for (int j = 0; j < layer->neurons[i].weights.count; j++) {
            layer->neurons[i].weights.items[j] = ((rand() % 1000) / 1000.0) * sqrt(2.0 / (inputCount + layer->neuronCount));
        }
    }
}

void LayerFree(Layer* layer)
{
    for(size_t i = 0; i < layer->neuronCount; i++) {
        NeuronFree(&layer->neurons[i]);
    }
    free(layer->neurons);
}

