#include "lists.h"
#include "network.h"
#include <math.h>

#define FULL

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

    for (int i = 0; i < layer->outputCount; i++) {
        for (int j = 0; j < layer->inputCount; j++) {
            // Xavier initialization (based on layer sizes)
#if defined (XAVIER)
            layer->weights.items[i * layer->inputCount + j] = ((rand() % 1000) / 1000.0) * sqrt(2.0 / (layer->inputCount + layer->outputCount));
#elif defined (FULL)
    layer->weights.items[i * layer->inputCount + j] = rand() / (RAND_MAX / 2.0) - 1.0;  // [-1, 1] range
#endif
        }
    }

    for (int i = 0; i < layer->outputCount; i++) {
        layer->biases.items[i] = BIAS_INIT;  // Try zero initialization
    }
}

void LayerFree(Layer* layer)
{
    DoublesFree(&layer->weights);
    DoublesFree(&layer->biases);
    DoublesFree(&layer->outputs);
    DoublesFree(&layer->deltas);
}

