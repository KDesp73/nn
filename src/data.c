#include "data.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool TrainingDataValid(const TrainingData* data)
{
    return (
        data != NULL &&
        data->inputs != NULL &&
        data->outputs != NULL &&
        data->inputCount > 0 &&
        data->outputCount > 0 &&
        data->exampleCount > 0
    );
}

void TrainingDataPrint(const TrainingData* data)
{
    printf("Required Inputs: %zu\n", data->inputCount);
    printf("Required Outputs: %zu\n", data->outputCount);
    for(size_t i = 0; i < data->exampleCount; i++){
        for(size_t j = 0; j < data->inputCount; j++){
            printf("inputs[%zu][%zu] = %lf\n", i, j, data->inputs[i][j]);
        }
        for(size_t k = 0; k < data->outputCount; k++){
            printf("outputs[%zu][%zu] = %lf\n", i, k, data->outputs[i][k]);
        }
        printf("\n");
    }
}

void TrainingDataLoad(TrainingData *data, const char* file)
{
    FILE* f = fopen(file, "r");
    if (!file) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    // Read input and output counts
    if (fscanf(f, "%zu %zu", &data->inputCount, &data->outputCount) != 2) {
        fprintf(stderr, "Invalid file format.\n");
        fclose(f);
        exit(EXIT_FAILURE);
    }

    // Count the number of training examples
    size_t exampleCount = 0;
    char line[1024];
    while (fgets(line, sizeof(line), f)) {
        if (line[0] != '\n' && line[0] != '#') {
            exampleCount++;
        }
    }
    data->exampleCount = exampleCount;

    // Allocate memory for inputs and outputs
    data->inputs = (double**)malloc(exampleCount * sizeof(double*));
    data->outputs = (double**)malloc(exampleCount * sizeof(double*));
    for (size_t i = 0; i < exampleCount; i++) {
        data->inputs[i] = (double*)malloc(data->inputCount * sizeof(double));
        data->outputs[i] = (double*)malloc(data->outputCount * sizeof(double));
    }

    // Reset file pointer to the beginning (after the first line)
    rewind(f);
    fscanf(f, "%*zu %*zu"); // Skip the first line

    // Read the input-output pairs
    size_t index = 0;
    while (fgets(line, sizeof(line), f)) {
        if (line[0] == '\n' || line[0] == '#') {
            continue; // Skip empty lines and comments
        }

        double* inputs = data->inputs[index];
        double* outputs = data->outputs[index];

        char* token = strtok(line, " |"); // Split the line into tokens using space and pipe as delimiters

        // Parse inputs
        for (size_t i = 0; i < data->inputCount; i++) {
            if (token != NULL) {
                inputs[i] = strtod(token, NULL); // Convert token to double
                token = strtok(NULL, " |");
            } else {
                fprintf(stderr, "Error: Insufficient input values in line %zu\n", index + 1);
                exit(EXIT_FAILURE);
            }
        }

        // Skip the pipe delimiter ("|") if present
        if (token != NULL && strcmp(token, "|") == 0) {
            token = strtok(NULL, " |");
        }

        // Parse outputs
        for (size_t i = 0; i < data->outputCount; i++) {
            if (token != NULL) {
                outputs[i] = strtod(token, NULL); // Convert token to double
                token = strtok(NULL, " |");
            } else {
                fprintf(stderr, "Error: Insufficient output values in line %zu\n", index + 1);
                exit(EXIT_FAILURE);
            }
        }

        index++;
    }

    fclose(f);
}

void TrainingDataFree(TrainingData* data)
{
    for (size_t i = 0; i < data->exampleCount; i++) {
        free(data->inputs[i]);
        free(data->outputs[i]);
    }
    free(data->inputs);
    free(data->outputs);
}
