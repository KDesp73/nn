#ifndef ACTIVATION_H
#define ACTIVATION_H

#include <math.h>
#include "config.h"

typedef enum {
    ACT_SIGMOID,
    ACT_RELU,
    ACT_TANH
} Activation;


static inline double sigmoid(double z)
{
    return 1.0 / (1.0 + exp(-z));
}

static inline double sigmoidPrime(double z)
{
    return sigmoid(z)*(1-sigmoid(z));
}

static inline double relu(double z)
{
    return (z > 0) ? z : 0;
}

static inline double reluPrime(double z)
{
    return (z > 0) ? 1 : 0;
}

static inline double tanhPrime(double z)
{
    double t = tanh(z);
    return 1.0 - t*t;
}

static inline double activation(double z)
{
    switch (ACTIVATION) {
    case ACT_SIGMOID:
        return sigmoid(z);
    case ACT_RELU:
        return relu(z);
    case ACT_TANH:
        return tanh(z);
    }
}

static inline double activationPrime(double z)
{
    switch (ACTIVATION) {
    case ACT_SIGMOID:
        return sigmoidPrime(z);
    case ACT_RELU:
        return reluPrime(z);
    case ACT_TANH:
        return tanhPrime(z);
    }
}

#endif // ACTIVATION_H
