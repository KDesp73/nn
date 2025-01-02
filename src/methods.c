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

static void backprop(Layer* currentLayer, Layer* prevLayer, double* inputs, double* targets, size_t sampleCount, double learningRate)
{
    for (size_t i = 0; i < currentLayer->neuronCount; i++) {
        Neuron* neuron = &currentLayer->neurons[i];
        double error = 0.0;
        if (prevLayer == NULL) {
            error = CostDerivative(neuron->value, targets[i]);
            neuron->delta = error * activationPrime(neuron->value);
        } else {
            // Hidden layer: compute error from the next layer
            for (size_t j = 0; j < prevLayer->neuronCount; j++) {
                Neuron* prevNeuron = &prevLayer->neurons[j];
                error += prevNeuron->delta * prevNeuron->weights.items[i];
            }
            neuron->delta = error * activationPrime(neuron->value);
        }
    }

    for (size_t i = 0; i < currentLayer->neuronCount; i++) {
        Neuron* neuron = &currentLayer->neurons[i];
        for (size_t j = 0; j < neuron->weights.count; j++) {
            // Update weights using the delta and the inputs from the previous layer
            double input = (inputs != NULL) ? inputs[j] : prevLayer->neurons[j].value;
            neuron->weights.items[j] += learningRate * neuron->delta * input;
        }
        // Update the bias
        neuron->bias += learningRate * neuron->delta;
    }
}

void Backward(
    Network *network,
    double *inputs,
    double *targets,
    size_t sampleCount,
    double learningRate
)
{
    for (int i = (int)network->layerCount - 1; i >= 0; i--) {
        backprop(
            &network->layers[i],
            (i == 0) ? NULL : &network->layers[i - 1],
            (i == 0) ? inputs : NULL, // Use inputs only for the first layer
            targets,
            sampleCount,
            learningRate
        );
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
            for(size_t i = 0; i < sampleCount; i++){
                nn->layers[0].neurons[i].value = inputs[sample][i];
            }

            // Forward pass
            Forward(nn);

            // Compute loss and errors
            Layer* outputLayer = &nn->layers[nn->layerCount - 1];
            for (int i = 0; i < outputLayer->neuronCount; i++) {
                double output = outputLayer->neurons[i].value;
                errors[i] = CostDerivative(output, targets[sample][i]);
                total_loss += (targets[sample][i] - output) * (targets[sample][i] - output);
            }

            // Backward pass
            Backward(nn, inputs[sample], targets[sample], sampleCount, learningRate);
        }

        if (epoch % 2000 == 0) {
            printf("Epoch %d, Loss: %f\n", epoch, total_loss / sampleCount);
        }
    }

    free(errors);
}
