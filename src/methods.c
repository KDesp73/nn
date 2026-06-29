#include "methods.h"
#include "activation.h"
#include "config.h"
#include "data.h"
#include "network.h"
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

double Cost(double predicted, double target)
{
    return 0.5 * (target - predicted) * (target - predicted);
}

double CostDerivative(double predicted, double target)
{
    return predicted - target;
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
    for (size_t i = 1; i < network->layerCount; i++) {
        Layer* layer = &network->layers[i];
        for (size_t j = 0; j < layer->neuronCount; j++) {
            layer->neurons[j].value = activation(sum(network, i, j));
        }
    }
}

static double clampGradient(double grad, double maxGradient)
{
    if (grad > maxGradient) return maxGradient;
    if (grad < -maxGradient) return -maxGradient;
    return grad;
}

void Backward(
    Network *network,
    double *targets,
    size_t sampleCount,
    double learningRate
)
{
    if (network->layerCount < 2) return;

    Layer* lastLayer = &network->layers[network->layerCount-1];
    Layer* secondLastLayer = &network->layers[network->layerCount-2];
    double maxGrad = config.maxGradient;
    for (size_t i = 0; i < lastLayer->neuronCount; i++) {
        Neuron* neuron = &lastLayer->neurons[i];
        neuron->delta = (targets[i] - neuron->value)
            * activationPrime(neuron->value);

        for (size_t j = 0; j < secondLastLayer->neuronCount; j++) {
            Neuron* neuronBefore = &secondLastLayer->neurons[j];
            double grad = learningRate * neuron->delta * neuronBefore->value;
            grad = clampGradient(grad, maxGrad);
            neuronBefore->weights.items[i] += grad;
        }
        if (config.biasEngage) {
            double grad = learningRate * neuron->delta;
            grad = clampGradient(grad, maxGrad);
            neuron->bias += grad;
        }
    }

    if (network->layerCount < 3) return;

    for (size_t i = network->layerCount - 2; i > 0; i--) {
        size_t idx = i - 1;
        Layer *currentLayer = &network->layers[idx];
        Layer *nextLayer = &network->layers[idx + 1];
        Layer *twoAfterLayer = &network->layers[idx + 2];

        for (size_t j = 0; j < currentLayer->neuronCount; j++) {
            for (size_t k = 0; k < nextLayer->neuronCount; k++) {
                double sum = 0;
                for (size_t l = 0; l < twoAfterLayer->neuronCount; l++) {
                    sum += nextLayer->neurons[k].weights.items[l]
                        * twoAfterLayer->neurons[l].delta;
                }

                double delta = sum * activationPrime(nextLayer->neurons[k].value);
                nextLayer->neurons[k].delta = delta;
                double grad = learningRate * currentLayer->neurons[j].value * delta;
                grad = clampGradient(grad, maxGrad);
                currentLayer->neurons[j].weights.items[k] += grad;
            }
            if (config.biasEngage) {
                double grad = learningRate * currentLayer->neurons[j].delta;
                grad = clampGradient(grad, maxGrad);
                currentLayer->neurons[j].bias += grad;
            }
        }
    }
}

void TrainNetwork(Network* nn, const TrainingData *data, int epochCount)
{
    assert(epochCount > 0);
    assert(nn != NULL);
    assert(TrainingDataValid(data));

    size_t sampleCount = data->exampleCount;
    double** inputs = data->inputs;
    double** targets = data->outputs;

    for (int epoch = 0; epoch < epochCount; epoch++) {
        double total_loss = 0.0;
        double learningRate = config.learningRate * pow(config.learningRateDecay, epoch);
        if (learningRate < config.minLearningRate) {
            learningRate = config.minLearningRate;
        }

        for (int sample = 0; sample < sampleCount; sample++) {
            for (size_t i = 0; i < nn->layers[0].neuronCount; i++) {
                nn->layers[0].neurons[i].value = inputs[sample][i];
            }

            Forward(nn);
            Backward(nn, targets[sample], sampleCount, learningRate);

            Layer* outputLayer = &nn->layers[nn->layerCount - 1];
            for (size_t i = 0; i < outputLayer->neuronCount; i++) {
                double output = outputLayer->neurons[i].value;
                total_loss += (targets[sample][i] - output) * (targets[sample][i] - output);
            }
        }

        nn->totalLoss = total_loss / sampleCount;
        if (nn->totalLoss <= config.targetLoss) break;
    }
}
