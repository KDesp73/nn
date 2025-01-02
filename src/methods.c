#include "methods.h"
#include "activation.h"
#include "config.h"
#include "misc.h"
#include "network.h"
#include <stdio.h>
#include <assert.h>

double Cost(double predicted, double target) {
    double diff = target - predicted;
    return diff;
}

double CostDerivative(double predicted, double target)
{
    double diff = target - predicted;
    return diff* (target)*(1-target);
}

static double sum(Network* network, size_t layerIndex, size_t nodeIndex)
{
    double s = 0;
    const Layer* prevLayer = layerIndex == 0 ? NULL : &network->layers[layerIndex - 1];
    if(prevLayer == NULL) return s;
    for(size_t i = 0; i < prevLayer->neuronCount; i++){
        s += prevLayer->neurons[i].value * prevLayer->neurons[i].weights.items[nodeIndex];
    }
    return s;
}

void Forward(Network* network)
{
    for (int i = 1; i < network->layerCount; i++) {
        Layer layer = network->layers[i];
        for (int j = 0; j < layer.neuronCount; j++) {
            layer.neurons[j].value = activation(sum(network, i, j));
        }
    }
}

static void backprop(Network* n, size_t layerIndex, double* targets, size_t sampleCount, double learningRate)
{
    Layer* currentLayer = &n->layers[layerIndex];
    Layer* prevLayer = (layerIndex == 0) ? NULL : &n->layers[layerIndex-1];
    Layer* nextLayer = (layerIndex == n->layerCount-1) ? NULL : &n->layers[layerIndex+1];

    for (size_t i = 0; i < currentLayer->neuronCount; i++) {
        Neuron* neuron = &currentLayer->neurons[i];
        double error = 0.0;

        // Compute the errors
        if (layerIndex == n->layerCount-1) {
            // Last layer
            error = targets[i] - neuron->value;
            neuron->delta = error * neuron->value * (1 - neuron->value);
        } else {
            // Hidden layer: compute error from the next layer
            for (size_t j = 0; j < nextLayer->neuronCount; j++) {
                Neuron* nextNeuron = &nextLayer->neurons[j];
                error += nextNeuron->delta * nextNeuron->weights.items[i];
            }
            neuron->delta = error * neuron->value * (1 - neuron->value);
        }

        if(prevLayer != NULL){
            for (size_t j = 0; j < prevLayer->neuronCount; j++) {
                Neuron* prevNeuron = &prevLayer->neurons[j];
                // Update weights using the delta and the inputs from the previous layer
                double input = (prevLayer == NULL) ? n->layers[0].neurons[j].value : prevLayer->neurons[j].value;
                prevNeuron->weights.items[i] += learningRate * neuron->delta * input;
            }
        }
        // Update the bias
        neuron->bias += learningRate * neuron->delta;
    }
}

void Backward(
    Network *network,
    double *targets,
    size_t sampleCount,
    double learningRate
)
{
    // for (int i = (int)network->layerCount - 1; i >= 0; i--) {
    //     backprop(
    //         network,
    //         i,
    //         targets,
    //         sampleCount,
    //         learningRate
    //     );
    // }

    Layer* lastLayer = &network->layers[network->layerCount-1];
    for(size_t i = 0; i < lastLayer->neuronCount; i++){
        Neuron* neuron = &lastLayer->neurons[i];
        neuron->delta = (targets[i] - neuron->value) * neuron->value * (1 - neuron->value);

        for(size_t j = 0; j < network->layers[network->layerCount-2].neuronCount; j++){
            Neuron* neuronBefore = &network->layers[network->layerCount-2].neurons[j];
            neuronBefore->weights.items[i] += learningRate * neuron->delta * neuronBefore->value;
        }
    }

    for(int r = network->layerCount - 3; r >= 0; r--){
        for(size_t t = 0; t < network->layers[r].neuronCount; t++){
            for(size_t i = 0; i < network->layers[r+1].neuronCount; i++){
                double sum = 0;
                for(size_t yy = 0; yy < network->layers[r+2].neuronCount; yy++){
                    sum += network->layers[r+1].neurons[i].weights.items[yy] * network->layers[r+2].neurons[yy].delta;
                }
                
                double err = network->layers[r+1].neurons[i].delta = sum * network->layers[r+1].neurons[i].value * (1 - network->layers[r+1].neurons[i].value);
                network->layers[r].neurons[t].weights.items[i] += learningRate * network->layers[r].neurons[t].value * err;
            }
        }
    }
}

void TrainNetwork(Network* nn, double** inputs, double** targets, int sampleCount, int epochCount)
{
    double* errors = malloc(nn->layers[nn->layerCount - 1].neuronCount * sizeof(double));
    if (errors == NULL) {
        printf("Error: Failed to allocate memory for errors.\n");
        return;
    }

    for (int epoch = 0; epoch < epochCount; epoch++) {
        double total_loss = 0.0;
        double learningRate = LEARNING_RATE * pow(LEARNING_RATE_DECAY, epoch);
        if (learningRate < MIN_LEARNING_RATE) {
            learningRate = MIN_LEARNING_RATE;
        }

        for (int sample = 0; sample < sampleCount; sample++) {
            for(size_t i = 0; i < nn->layers[0].neuronCount; i++){
                nn->layers[0].neurons[i].value = inputs[sample][i];
            }

            // Forward pass
            Forward(nn);

            // Backward pass
            Backward(nn, targets[sample], sampleCount, learningRate);

            // Compute loss and errors
            Layer* outputLayer = &nn->layers[nn->layerCount - 1];
            for (int i = 0; i < outputLayer->neuronCount; i++) {
                double output = outputLayer->neurons[i].value;
                errors[i] = Cost(output, targets[sample][i]);
                total_loss += (targets[sample][i] - output) * (targets[sample][i] - output);
            }

        }

        if (epoch % 2000 == 0) {
            printf("Epoch %d, Loss: %f\n", epoch, total_loss / sampleCount);
        }
    }

    free(errors);
}
