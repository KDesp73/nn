#include "methods.h"
#include "activation.h"
#include "config.h"
#include "network.h"
#include <stdio.h>
#include <assert.h>

double CostDerivative(double predicted, double target) {
    return -target / predicted + (1 - target) / (1 - predicted);
}

void Forward(Layer *layer, double *inputs)
{
    assert(layer != NULL);
    assert(inputs != NULL);

    for (int i = 0; i < layer->outputCount; i++) {
        double sum = layer->biases.items[i];

        for (int j = 0; j < layer->inputCount; j++) {
            sum += inputs[j] * layer->weights.items[i * layer->inputCount + j];
        }

        layer->outputs.items[i] = activation(sum);
    }

    for (int i = 0; i < layer->outputCount; i++) {
        layer->outputs.items[i] = fmax(0.0, layer->outputs.items[i]);
        layer->outputs.items[i] = fmin(fmax(layer->outputs.items[i], -1.0), 1.0);
    }
}

void Backward(Layer *layer, double *inputs, double *nextDeltas, double *nextWeights, int nextOutputCount, double *targets, double learningRate)
{
    // Compute deltas for this layer
    for (int i = 0; i < layer->outputCount; i++) {
        double error = 0.0f;
        
        // For the output layer (layer index = last layer), use the cost derivative
        if (nextDeltas == NULL) {  // Output layer (last layer)
            error = CostDerivative(layer->outputs.items[i], targets[i]);
        } else {
            // For hidden layers, propagate the error backwards
            for (int j = 0; j < nextOutputCount; j++) {
                error += nextDeltas[j] * nextWeights[j * layer->outputCount + i];
            }
        }
        
        layer->deltas.items[i] = error * activationPrime(layer->outputs.items[i]);

        // Apply gradient clipping
        if (layer->deltas.items[i] > MAX_GRADIENT) {
            layer->deltas.items[i] = MAX_GRADIENT;
        } else if (layer->deltas.items[i] < -MAX_GRADIENT) {
            layer->deltas.items[i] = -MAX_GRADIENT;
        }
    }

    // Update weights and biases
    for (int i = 0; i < layer->outputCount; i++) {
        for (int j = 0; j < layer->inputCount; j++) {
            // Update weights using the deltas and the inputs from the previous layer
            layer->weights.items[i * layer->inputCount + j] += learningRate * layer->deltas.items[i] * inputs[j];
        }
        // Update biases using the deltas
        layer->biases.items[i] += learningRate * layer->deltas.items[i];
    }
}


// Train the network
void TrainNetwork(Network* nn, double** inputs, double** targets, int sampleCount, int epochCount)
{
    for (int epoch = 0; epoch < epochCount; epoch++) {
        double total_loss = 0.0;
        double learningRate = LEARNING_RATE * pow(LEARNING_RATE_DECAY, epoch);
        if (learningRate < MIN_LEARNING_RATE) {
            learningRate = MIN_LEARNING_RATE;
        }
        
        for (int sample = 0; sample < sampleCount; sample++) {
            double* current_inputs = inputs[sample];

            // Forward pass
            for (int i = 0; i < nn->layerCount; i++) {
                Forward(&nn->layers[i], current_inputs);
                current_inputs = nn->layers[i].outputs.items;
            }

            // Compute loss (mean squared error)
            double* outputs = nn->layers[nn->layerCount - 1].outputs.items;
            double* errors = malloc(nn->layers[nn->layerCount - 1].outputCount * sizeof(double));
            for (int i = 0; i < nn->layers[nn->layerCount - 1].outputCount; i++) {
                errors[i] = targets[sample][i] - outputs[i];
                total_loss += errors[i] * errors[i];
            }

            // Backward pass
            double* current_deltas = errors;
            for (int i = nn->layerCount - 1; i >= 0; i--) {
                Backward(&nn->layers[i],
                         (i == 0) ? inputs[sample] : nn->layers[i - 1].outputs.items,
                         current_deltas,
                         nn->layers[i].weights.items,
                         nn->layers[i].outputCount,
                         targets[sample],
                         learningRate);
                current_deltas = nn->layers[i].deltas.items;
            }

            free(errors);
        }

        // Average the loss across all samples
        // printf("Epoch %d, Loss: %f\n", epoch, total_loss / sampleCount);
    }
}
