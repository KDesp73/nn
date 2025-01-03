#include "network.h"
#include "lists.h"
#include "lists.h"
#include "misc.h"
#include <stdio.h>
#include <string.h>

#undef DEBUG
#include <io/logging.h>

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


void NetworkLoad(Network* network, const char* file) {
    FILE* f = fopen(file, "r");
    if (f == NULL) {
        perror("Failed to open file");
        return;
    }
    INFO("Loading network from %s", file);

    // Read the first line to get the number of layers and neurons in each layer
    char line[1024];
    if (fgets(line, sizeof(line), f) == NULL) {
        fprintf(stderr, "Error: Failed to read the first line of the file.\n");
        fclose(f);
        return;
    }

    // Parse the number of layers and neurons in each layer
    char* token = strtok(line, " ");
    size_t layerCount = (size_t)atoi(token);

    assert(layerCount <= MAX_LAYERS); // Ensure the number of layers does not exceed the limit
    network->layerCount = layerCount;
    INFO("Layer count: %zu", layerCount);

    // Parse neuron counts for each layer
    for (size_t i = 0; i < layerCount; i++) {
        token = strtok(NULL, " ");
        size_t neuronCount = (size_t)atoi(token);
        network->layers[i].neuronCount = neuronCount;

        // Allocate memory for neurons in the layer
        network->layers[i].neurons = malloc(neuronCount * sizeof(Neuron));
        if (network->layers[i].neurons == NULL) {
            perror("Failed to allocate memory for neurons");
            fclose(f);
            return;
        }

        // Initialize each neuron
        for (size_t j = 0; j < neuronCount; j++) {
            network->layers[i].neurons[j].weights.items = NULL;
            network->layers[i].neurons[j].value = 0.0;
            network->layers[i].neurons[j].delta = 0.0;
            network->layers[i].neurons[j].bias = 0.0;
        }
    }

    // Parse the rest of the file for neuron properties and weights
    Layer* currentLayer = NULL;
    Neuron* currentNeuron = NULL;
    size_t currentLayerIndex = 0;
    size_t currentNeuronIndex = 0;

    while (fgets(line, sizeof(line), f) != NULL) {
        line[strcspn(line, "\n")] = '\0'; // Remove the newline character

        if (strncmp(line, "layer", 5) == 0) {
            currentLayerIndex = (size_t)atoi(line + 6);
            DEBU("Current Layer Index: %zu", currentLayerIndex);
            currentLayer = &network->layers[currentLayerIndex];
            currentNeuronIndex = 0;
        } else if (strncmp(line, "neuron", 6) == 0) {
            currentNeuronIndex = (size_t)atoi(line + 7);
            DEBU("Current Neuron Index: %zu", currentNeuronIndex);
            currentNeuron = &currentLayer->neurons[currentNeuronIndex];
        } else if (strncmp(line, "value", 5) == 0) {
            currentNeuron->value = atof(line + 6);
            DEBU("Neuron %zu value: %lf", currentNeuronIndex, currentNeuron->value);
        } else if (strncmp(line, "delta", 5) == 0) {
            currentNeuron->delta = atof(line + 6);
            DEBU("Neuron %zu delta: %lf", currentNeuronIndex, currentNeuron->delta);
        } else if (strncmp(line, "bias", 4) == 0) {
            currentNeuron->bias = atof(line + 5);
            DEBU("Neuron %zu bias: %lf", currentNeuronIndex, currentNeuron->bias);
        } else if (strncmp(line, "weights", 7) == 0) {
            char* weightsStr = line + 8;
            size_t weightCount = currentLayerIndex + 1 < network->layerCount
                ? network->layers[currentLayerIndex + 1].neuronCount
                : 0;

            currentNeuron->weights = DoublesAlloc(weightCount);

            size_t weightIndex = 0;
            char* weightToken = strtok(weightsStr, " ");
            while (weightToken != NULL) {
                currentNeuron->weights.items[weightIndex++] = atof(weightToken);
                weightToken = strtok(NULL, " ");
            }
        }
    }

    fclose(f);
}

void NetworkdSave(const Network* network, const char* file)
{
    FILE* f = fopen(file, "w");

    fprintf(f, "%zu ", network->layerCount);
    for(size_t i = 0; i < network->layerCount; i++){
        fprintf(f, "%zu ", network->layers[i].neuronCount);
    }
    fprintf(f, "\n\n");

    for(size_t i = 0; i < network->layerCount; i++){
        fprintf(f, "layer %zu\n", i);
        for(size_t j = 0; j < network->layers[i].neuronCount; j++){
            Neuron n = network->layers[i].neurons[j];
            fprintf(f, "neuron %zu\n", j);
            fprintf(f, "value %lf\n", n.value);
            fprintf(f, "delta %lf\n", n.delta);
            fprintf(f, "bias %lf\n", n.bias);
            if(n.weights.count > 0){
                fprintf(f, "weights ");
                for(size_t k = 0; k < n.weights.count; k++){
                    fprintf(f, "%lf ", n.weights.items[k]);
                }
                fprintf(f, "\n");
            }
        }
        fprintf(f, "\n");
    }

    fclose(f);
}

