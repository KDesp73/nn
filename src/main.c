#include <stddef.h>
#include <stdio.h>
#include "methods.h"
#include "misc.h"
#include "network.h"
#include "lists.h"
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
    NetworkInit(&n, SizesInit(2, 5, 3, 1, SIZESEND));

    TrainNetwork(&n, inputs.items, targets.items, 4, EPOCH_COUNT);
    NetworkPrint(&n);

    printf("\nTesting the trained network:\n");
    for (int i = 0; i < 4; i++) {
        double *input = inputs.items[i];
        double *current_input = input;

        // Forward pass
        for (int j = 0; j < n.layerCount; j++) {
            Forward(&n.layers[j], current_input);
            current_input = n.layers[j].outputs.items;
        }

        // Output result
        double output = n.layers[n.layerCount - 1].outputs.items[0];
        printf("Input: [%0.1f, %0.1f] -> Predicted: %0.4f, Target: %0.1f\n",
                input[0], input[1], output, targets.items[i][0]);
    }

    NetworkFree(&n);
    Doubles2DFree(&inputs);
    Doubles2DFree(&targets);

    return 0;
}
