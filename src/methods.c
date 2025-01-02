#include "network.h"

double CostDerivative(double outputActivations, double y)
{
    return outputActivations - y;
}

