#include <stddef.h>
#include <stdio.h>
#include "methods.h"
#include "misc.h"
#include "network.h"
#include "lists.h"
#include <stdlib.h>
#include <time.h>

int main()
{
    srand((unsigned int)time(NULL));

    Doubles2D inputs = Doubles2DInit(
        DoublesInit(0.0, 0.0, DOUBLESEND).items,
        DoublesInit(0.0, 1.0, DOUBLESEND).items,
        DoublesInit(1.0, 0.0, DOUBLESEND).items,
        DoublesInit(1.0, 1.0, DOUBLESEND).items,
        NULL
    );

    Doubles2D targets = Doubles2DInit(
        DoublesInit(0.0, DOUBLESEND).items,
        DoublesInit(1.0, DOUBLESEND).items,
        DoublesInit(1.0, DOUBLESEND).items,
        DoublesInit(0.0, DOUBLESEND).items,
        NULL
    );

    Network n;
    NetworkInit(&n, SizesInit(2, 2, 1, SIZESEND));

    // n.layers[0].neurons[0].value = 1.0;
    // n.layers[0].neurons[0].weights = DoublesInit(1.450530, 0.196067, DOUBLESEND);
    // n.layers[0].neurons[1].value = 0.0;
    // n.layers[0].neurons[1].weights = DoublesInit(1.329281, 0.191530, DOUBLESEND);
    //
    // n.layers[1].neurons[0].value = 0.999292;
    // n.layers[1].neurons[0].weights = DoublesInit(5.749740, DOUBLESEND);
    // n.layers[1].neurons[1].value = 0.727175;
    // n.layers[1].neurons[1].weights = DoublesInit(-7.069996, DOUBLESEND);

    TrainNetwork(&n, inputs.items, targets.items, 4, EPOCH_COUNT);
    NetworkPrint(&n);

    printf("\nTesting the trained network:\n");
    for (int i = 0; i < 4; i++) {
        double *input = inputs.items[i];
        for(size_t k = 0; k < inputs.count; k++){
            n.layers[0].neurons[k].value = input[k];
        }

        Forward(&n);

        // Output result
        double output = n.layers[n.layerCount - 1].neurons[0].value;
        printf("Input: [%0.1f, %0.1f] -> Predicted: %0.4f, Target: %0.1f\n",
                input[0], input[1], output, targets.items[i][0]);
    }

    NetworkFree(&n);
    Doubles2DFree(&inputs);
    Doubles2DFree(&targets);

    return 0;
}
