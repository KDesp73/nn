#include <stddef.h>
#include <stdio.h>
#include "misc.h"
#include "network.h"
#include "lists.h"
#include <time.h>

int main()
{
    srand((unsigned int)time(NULL));

    Network n;
    NetworkInit(&n, SizesInit(2, 5, 3, 1, SIZESEND));
    NetworkPrint(&n);
    NetworkFree(&n);

    return 0;
}
