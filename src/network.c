#include "network.h"
#include "lists.h"
#include "lists.h"
#include <stdio.h>


void NetworkPrint(const Network* network)
{
    printf("Network with %zu layers:\n", network->layerCount);

    // Iterate over each layer
    for (size_t i = 0; i < network->layerCount; i++) {
        const Layer* layer = &network->layers[i];
        printf("\nLayer %zu:\n", i);
        printf("  Inputs: %zu\n", layer->neuronCount);
        printf("  Outputs: %zu\n", layer->neurons[0].weights.count);

        for(size_t j = 0; j < layer->neuronCount; j++){
            const Neuron neuron = layer->neurons[j];
            printf("    Value: %lf\n", neuron.value);
            printf("    Weights: ");
            for(size_t k = 0; k < neuron.weights.count; k++){
                printf("%lf ", neuron.weights.items[k]);
            }
            printf("\n");
        }
    }
}

void NetworkInit(Network* network, Sizes sizes) {
    // Set the number of layers in the network
    network->layerCount = sizes.count;

    // Loop through each layer to initialize it
    for (size_t i = 0; i < network->layerCount; i++) {
        size_t inputCount = sizes.items[i];
        size_t outputCount = (i == network->layerCount-1) ? 0 : sizes.items[i + 1];

        LayerInit(&network->layers[i], inputCount, outputCount);
    }

    SizesFree(&sizes);
}

void NetworkFree(Network* network)
{
    for(size_t i = 0; i < network->layerCount; i++){
        LayerFree(&network->layers[i]);
    }
}

