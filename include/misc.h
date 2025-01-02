#ifndef MISC_H
#define MISC_H

#include <stdlib.h>

static inline double randomFloat()
{
    return (double)rand() / (double)RAND_MAX;
}

#define DOUBLE_PRINT(d)\
    printf("%s: %lf\n", #d, d)

#endif // MISC_H
