#include "lists.h"
#include "misc.h"

void DoublesZero(Doubles* doubles, size_t count)
{
    doubles->items = (double*)malloc(count * sizeof(double));
    doubles->count = count;

    for (size_t i = 0; i < count; i++) {
        doubles->items[i] = 0.0;
    }
}

void DoublesRand(Doubles* doubles, size_t count)
{
    doubles->items = (double*)malloc(count * sizeof(double));
    doubles->count = count;

    for (size_t i = 0; i < count; i++) {
        doubles->items[i] = randomFloat();
    }
}

