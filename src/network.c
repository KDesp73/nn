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
        printf("  Inputs: %zu\n", layer->inputCount);
        printf("  Outputs: %zu\n", layer->outputCount);

        // Print weights
        printf("  Weights:\n");
        for (size_t j = 0; j < layer->outputCount; j++) {
            printf("    Neuron %zu: ", j);
            for (size_t k = 0; k < layer->inputCount; k++) {
                printf("%f ", layer->weights.items[j * layer->inputCount + k]);
            }
            printf("\n");
        }

        // Print biases
        printf("  Biases:\n");
        for (size_t j = 0; j < layer->biases.count; j++) {
            printf("    Neuron %zu: %f\n", j, layer->biases.items[j]);
        }

        // Print outputs
        printf("  Outputs:\n");
        for (size_t j = 0; j < layer->outputs.count; j++) {
            printf("    Neuron %zu: %f\n", j, layer->outputs.items[j]);
        }
    }
}

void NetworkInit(Network* network, Sizes sizes) {
    // Set the number of layers in the network
    network->layerCount = sizes.count - 1;

    // Loop through each layer to initialize it
    for (size_t i = 0; i < network->layerCount; i++) {
        size_t inputCount = sizes.items[i];
        size_t outputCount = sizes.items[i + 1];

        // Initialize the layer with the specified input and output counts
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

