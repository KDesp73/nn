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
#define TARGET_LOSS 0.001

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
    double targetLoss;
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
    config.biasEngage = BIAS_ENGAGE; \
    config.targetLoss = TARGET_LOSS

void ConfigLoad(const char* file);
void ConfigPrint();

void ConfigSet(Config* config, const char* key, const char* value);
static inline void ConfigSetLearningRate(Config* config, double lr){ config->learningRate = lr; }
static inline void ConfigSetMinLearningRate(Config* config, double mlr){ config->minLearningRate = mlr; }
static inline void ConfigSetLearningRateDecay(Config* config, double lrd){ config->learningRateDecay = lrd; }
static inline void ConfigSetTheta(Config* config, double t){ config->theta = t; }
static inline void ConfigSetMaxGradient(Config* config, double mg){ config->maxGradient = mg; }
static inline void ConfigSetActivation(Config* config, int a){ config->activation = a; }
static inline void ConfigSetEpochs(Config* config, size_t e){ config->epochs = e; }
static inline void ConfigSetBiasInit(Config* config, double bi){ config->biasInit = bi; }
static inline void ConfigSetBiasEngage(Config* config, bool be){ config->biasEngage = be; }
static inline void ConfigSetTargetLoss(Config* config, double tl){ config->targetLoss = tl; }

#endif // CONFIG_H
