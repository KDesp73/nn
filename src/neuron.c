#include "config.h"
#include "lists.h"
#include "network.h"

Neuron NeuronInit(double value, Doubles weights)
{
    return (Neuron) {
        .value = value,
        .weights = weights,
        .bias = BIAS_INIT,
        .delta = 0
    };
}

void NeuronFree(Neuron* neuron)
{
    DoublesFree(&neuron->weights);
}
