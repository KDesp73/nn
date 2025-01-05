#ifndef DATA_H
#define DATA_H

#include <stdbool.h>
#include <stddef.h>
typedef struct {
    size_t inputCount;
    size_t outputCount;
    size_t exampleCount;
    double** inputs;
    double** outputs;
} TrainingData;

void TrainingDataLoad(TrainingData *data, const char* file);
void TrainingDataFree(TrainingData* data);
void TrainingDataPrint(const TrainingData* data);
bool TrainingDataValid(const TrainingData* data);

#endif // DATA_H
