#include "lists.h"

void DoublesZero(Doubles* doubles, size_t count)
{
    doubles->items = (double*)malloc(count * sizeof(double));
    doubles->count = count;

    // Initialize all elements to 0 (or random values if needed)
    for (size_t i = 0; i < count; i++) {
        doubles->items[i] = 0.0;
    }
}


