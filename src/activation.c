#include "activation.h"
#include <string.h>


const char* ActivationToString(Activation activation)
{
    switch (activation) {
    case ACT_SIGMOID:
        return "SIGMOID";
    case ACT_RELU:
        return "RELU";
    case ACT_TANH:
        return "TANH";
    default:
        return NULL;
    }
}

Activation StringToActivation(const char* str)
{
    if(!strcmp(ActivationToString(ACT_SIGMOID), str)) return ACT_SIGMOID;
    else if(!strcmp(ActivationToString(ACT_RELU), str)) return ACT_RELU;
    else if(!strcmp(ActivationToString(ACT_TANH), str)) return ACT_TANH;
    else return -1;
}
