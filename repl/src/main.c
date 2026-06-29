#define ANSI_IMPLEMENTATION
#include <io/ansi.h>

#include "commands.h"
#include "utils.h"
#include <stddef.h>
#include <stdio.h>
#include "activation.h"
#include "config.h"
#include "data.h"
#include "methods.h"
#include "misc.h"
#include "network.h"
#include "lists.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

Network* nn;
extern Config config;

void loop(){
    char input[1024];

    for(;;) {
        printf("%s>%s ", ANSI_GREEN, ANSI_RESET);
        if(fgets(input, sizeof(input), stdin) == NULL) continue; 

        input[strcspn(input, "\n")] = 0;

        if(!isEmpty(input))
            HandleCommand(input);
    }

}

int main()
{
    srand((unsigned int)time(NULL));
    CONFIG_INIT();
    ConfigLoad("nn.conf");

    loop();

    return 0;
}
