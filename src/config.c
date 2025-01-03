#include "config.h"
#include "activation.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Config config = {0};

void ConfigPrint()
{
    printf("Neural Network Configuration\n");
    printf("Learning Rate: %.2lf\n", config.learningRate);
    printf("Learning Rate Decay: %.2lf\n", config.learningRateDecay);
    printf("Minimum Learning Rate: %.2lf\n", config.minLearningRate);
    printf("Epochs: %zu\n", config.epochs);
    printf("Bias Init: %.2lf\n", config.biasInit);
    printf("Bias Engage: %s\n", (config.biasEngage) ? "true" : "false");
    printf("Max Gradient: %.2lf\n", config.maxGradient);
    printf("Theta: %.2lf\n", config.theta);
    printf("Activation: %s\n", ActivationToString(config.activation));
}

#include <string.h>

void ConfigLoad(const char* file)
{
    FILE* f = fopen(file, "r");
    if (f == NULL) {
        printf("Error: Could not open config file '%s'\n", file);
        return;
    }

    char line[1024];
    while (fgets(line, sizeof(line), f) != NULL) {
        line[strcspn(line, "\n")] = '\0';
        if (line[0] == '\0' || line[0] == '#') {
            continue;
        }

        char* delimiter = strchr(line, '=');
        if (delimiter == NULL) {
            printf("Warning: Invalid config line: '%s'\n", line);
            continue;
        }

        *delimiter = '\0';
        char* key = line;
        char* value = delimiter + 1;

        // Trim leading and trailing spaces
        while (*key == ' ') key++;
        while (*value == ' ') value++;
        char* end = value + strlen(value) - 1;
        while (end > value && (*end == ' ' || *end == '\t')) {
            *end = '\0';
            end--;
        }

        if (strcmp(key, "learningRate") == 0) {
            config.learningRate = atof(value);
        } else if (strcmp(key, "minLearningRate") == 0) {
            config.minLearningRate = atof(value);
        } else if (strcmp(key, "learningRateDecay") == 0) {
            config.learningRateDecay = atof(value);
        } else if (strcmp(key, "theta") == 0) {
            config.theta = atof(value);
        } else if (strcmp(key, "maxGradient") == 0) {
            config.maxGradient = atof(value);
        } else if (strcmp(key, "activation") == 0) {
            if (strcmp(value, "SIGMOID") == 0) {
                config.activation = ACT_SIGMOID;
            } else if (strcmp(value, "RELU") == 0) {
                config.activation = ACT_RELU;
            } else if (strcmp(value, "TANH") == 0) {
                config.activation = ACT_TANH;
            } else {
                printf("Warning: Unknown activation type: '%s'\n", value);
            }
        } else if (strcmp(key, "epochs") == 0) {
            config.epochs = (size_t)atoi(value);
        } else if (strcmp(key, "biasInit") == 0) {
            config.biasInit = atof(value);
        } else if (strcmp(key, "biasEngage") == 0) {
            config.biasEngage = (strcmp(value, "true") == 0 || strcmp(value, "1") == 0);
        } else {
            printf("Warning: Unknown config key: '%s'\n", key);
        }
    }

    fclose(f);
}
