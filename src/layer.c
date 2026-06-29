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

    for (size_t i = 0; i < layer->neuronCount; i++) {
        layer->neurons[i] = NeuronInit(randomFloat(), DoublesAlloc(outputCount));
        layer->neurons[i].bias = ((double)rand() / RAND_MAX) - 0.5;
        for (size_t j = 0; j < layer->neurons[i].weights.count; j++) {
            layer->neurons[i].weights.items[j] = (randomFloat() + 0.1);
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

