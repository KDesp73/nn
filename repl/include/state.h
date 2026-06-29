#ifndef STATE_H
#define STATE_H

#include "data.h"
#include "network.h"
#include <stdbool.h>
typedef struct {
    Network nn;
    TrainingData data;
    bool nnLoaded;
    bool debug;
} State;

extern State state;

#endif // STATE_H
