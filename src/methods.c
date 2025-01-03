#include "methods.h"
#include "activation.h"
#include "config.h"
#include "network.h"
#include <stdio.h>
#include <stdbool.h>
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
    if(config.biasEngage)
        s += network->layers[layerIndex].neurons[nodeIndex].bias;
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

void Backward(
    Network *network,
    double *targets,
    size_t sampleCount,
    double learningRate
)
{
    Layer* lastLayer = &network->layers[network->layerCount-1];
    Layer* secondLastLayer = &network->layers[network->layerCount-2];
    for(size_t i = 0; i < lastLayer->neuronCount; i++){
        Neuron* neuron = &lastLayer->neurons[i];
        neuron->delta = (targets[i] - neuron->value) 
            * activationPrime(neuron->value);

        for(size_t j = 0; j < secondLastLayer->neuronCount; j++){
            Neuron* neuronBefore = &secondLastLayer->neurons[j];
            neuronBefore->weights.items[i] += learningRate 
                * neuron->delta
                * neuronBefore->value;
        }
        if(config.biasEngage)
            neuron->bias += learningRate * neuron->delta;
    }

    for(int i = network->layerCount - 3; i >= 0; i--){
        Layer *currentLayer = &network->layers[i];
        Layer *nextLayer = &network->layers[i + 1];
        Layer *twoAfterLayer = &network->layers[i + 2];

        for(size_t j = 0; j < currentLayer->neuronCount; j++){
            for(size_t k = 0; k < nextLayer->neuronCount; k++){
                double sum = 0;
                for(size_t l = 0; l < twoAfterLayer->neuronCount; l++){
                    sum += nextLayer->neurons[k].weights.items[l] 
                        * twoAfterLayer->neurons[l].delta;
                }
                
                double err = nextLayer->neurons[k].delta 
                    = sum * activationPrime(nextLayer->neurons[k].value);
                currentLayer->neurons[j].weights.items[k] += learningRate 
                    * currentLayer->neurons[j].value 
                    * err;
            }
            if(config.biasEngage)
                currentLayer->neurons[j].bias += learningRate * currentLayer->neurons[j].delta;
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
        double learningRate = config.learningRate * pow(config.learningRateDecay, epoch);
        if (learningRate < config.minLearningRate) {
            learningRate = config.minLearningRate;
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
