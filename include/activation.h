#ifndef ACTIVATION_H
#define ACTIVATION_H

#include <math.h>
#include "config.h"

/**
 * @enum Activation
 * Represents the supported activation functions for the neural network.
 * 
 * @var ACT_SIGMOID Sigmoid activation function.
 * @var ACT_RELU Rectified Linear Unit (ReLU) activation function.
 * @var ACT_TANH Hyperbolic tangent (tanh) activation function.
 */
typedef enum {
    ACT_SIGMOID,
    ACT_RELU,
    ACT_TANH
} Activation;

/**
 * Converts an Activation enum value to its string representation.
 * 
 * @param activation The Activation enum value.
 * @return A string representing the activation function.
 */
const char* ActivationToString(Activation activation);

/**
 * Converts a string to its corresponding Activation enum value.
 * 
 * @param str The string representing an activation function.
 * @return The corresponding Activation enum value.
 */
Activation StringToActivation(const char* str);

/**
 * Computes the sigmoid activation function.
 * 
 * @param z The input value.
 * @return The result of the sigmoid function applied to the input.
 */
static inline double sigmoid(double z)
{
    double sigma = 1.0 / (1.0 + exp(-z / THETA));
    return sigma;
}

/**
 * Computes the derivative of the sigmoid activation function.
 * 
 * @param z The result of the sigmoid function (not the raw input).
 * @return The derivative of the sigmoid function.
 */
static inline double sigmoidPrime(double z)
{
    return (z) * (1 - (z));
}

/**
 * Computes the ReLU (Rectified Linear Unit) activation function.
 * 
 * @param z The input value.
 * @return The result of the ReLU function applied to the input.
 */
static inline double relu(double z)
{
    return (z > 0) ? z : 0;
}

/**
 * Computes the derivative of the ReLU activation function.
 * 
 * @param z The input value.
 * @return The derivative of the ReLU function.
 */
static inline double reluPrime(double z)
{
    return (z > 0) ? 1 : 0;
}

/**
 * Computes the derivative of the tanh activation function.
 * 
 * @param z The input value.
 * @return The derivative of the tanh function.
 */
static inline double tanhPrime(double z)
{
    double t = tanh(z);
    return 1.0 - t * t;
}

/**
 * Computes the activation function specified by config.activation
 * 
 * @param z The input value.
 * @return The result of the chosen activation function applied to the input.
 */
static inline double activation(double z)
{
    switch (config.activation) {
    case ACT_SIGMOID:
        return sigmoid(z);
    case ACT_RELU:
        return relu(z);
    case ACT_TANH:
        return tanh(z);
    }
    return -1;
}

/**
 * Computes the derivative of the activation function specified by config.activation
 * 
 * @param z The input value.
 * @return The derivative of the chosen activation function.
 */
static inline double activationPrime(double z)
{
    switch (config.activation) {
    case ACT_SIGMOID:
        return sigmoidPrime(z);
    case ACT_RELU:
        return reluPrime(z);
    case ACT_TANH:
        return tanhPrime(z);
    }
    return -1;
}

#endif // ACTIVATION_H
