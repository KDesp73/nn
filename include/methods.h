#ifndef METHODS_H
#define METHODS_H

#include "data.h"
#include "network.h"

/**
 * Calculates the cost (loss) between the predicted output and the target value.
 * 
 * @param predicted The predicted output from the network.
 * @param target The true target value.
 * @return The cost value as a double.
 */
double Cost(double predicted, double target);

/**
 * Calculates the derivative of the cost function with respect to the output activations.
 * 
 * @param outputActivations The output activation value from the network.
 * @param y The true target value.
 * @return The derivative of the cost function as a double.
 */
double CostDerivative(double outputActivations, double y);

/**
 * Performs the backward pass (backpropagation) on the network to update weights and biases.
 * 
 * @param network Pointer to the neural network to train.
 * @param targets Array of target values for the training samples.
 * @param sampleCount The number of training samples.
 * @param learningRate The learning rate for gradient descent.
 */
void Backward(
    Network *network,
    double *targets,
    size_t sampleCount,
    double learningRate
);

/**
 * Performs the forward pass on the network to compute activations for all layers.
 * 
 * @param network Pointer to the neural network.
 */
void Forward(Network* network);

/**
 * Trains the network using the provided training data for a specified number of epochs.
 * 
 * @param nn Pointer to the neural network to train.
 * @param data Pointer to the training data structure containing input-output pairs.
 * @param epochCount The number of epochs to train the network.
 */
void TrainNetwork(Network *nn, const TrainingData* data, int epochCount);

#endif // METHODS_H
