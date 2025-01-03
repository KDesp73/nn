#ifndef CONFIG_H
#define CONFIG_H

#include <stdbool.h>
#include <stddef.h>

#define LEARNING_RATE 2.0
#define MIN_LEARNING_RATE 0.0001
#define LEARNING_RATE_DECAY 0.999
#define THETA 0.2
#define MAX_GRADIENT 10.0
#define MAX_LAYERS 128
#define ACTIVATION ACT_SIGMOID
#define EPOCH_COUNT 40000
#define BIAS_INIT 0.1
#define BIAS_ENGAGE false

typedef struct {
    double learningRate;
    double minLearningRate;
    double learningRateDecay;
    double theta;
    double maxGradient;
    int activation;
    size_t epochs;
    double biasInit;
    bool biasEngage;
} Config;

extern Config config;

#define CONFIG_INIT() \
    config.learningRate = LEARNING_RATE; \
    config.minLearningRate = MIN_LEARNING_RATE; \
    config.learningRateDecay = LEARNING_RATE_DECAY; \
    config.theta = THETA; \
    config.maxGradient = MAX_GRADIENT; \
    config.activation = ACTIVATION; \
    config.epochs = EPOCH_COUNT; \
    config.biasInit = BIAS_INIT; \
    config.biasEngage = BIAS_ENGAGE

void ConfigLoad(const char* file);
void ConfigPrint();

#endif // CONFIG_H
