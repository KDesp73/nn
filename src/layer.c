#include "lists.h"
#include "network.h"

void LayerInit(Layer* layer, size_t inputCount, size_t outputCount)
{
    layer->inputCount = inputCount;
    layer->outputCount = outputCount;

    // Initialize weights (inputCount * outputCount)
    DoublesZero(&layer->weights, inputCount * outputCount);

    // Initialize biases, outputs, and deltas (outputCount each)
    DoublesZero(&layer->biases, outputCount);
    DoublesZero(&layer->outputs, outputCount);
    DoublesZero(&layer->deltas, outputCount);

    for (size_t i = 0; i < layer->weights.count; i++) {
        layer->weights.items[i] = ((double)rand() / RAND_MAX) * 2 - 1; // Random between -1 and 1
    }
    for (size_t i = 0; i < layer->biases.count; i++) {
        layer->biases.items[i] = ((double)rand() / RAND_MAX) * 2 - 1; // Random between -1 and 1
    }
}

void LayerFree(Layer* layer)
{
    DoublesFree(&layer->weights);
    DoublesFree(&layer->biases);
    DoublesFree(&layer->outputs);
    DoublesFree(&layer->deltas);
}

