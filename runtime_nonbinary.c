//
//  Runtime non binary
//  Compiler Lac_reorg
//
//  Created by Nicolas YING on 19/12/2016.
//  Copyright © 2016 Nicolas YING. All rights reserved.
//
//  Error code 6**
//  generate a VM array complying to the VM_LAC_hugolonicolasien standard 1.4

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "common_component.h"
#include "processor.h"

static int * VMp; // VM array pointer
static int result;
basicStack *data, *type, *call;

int main(int argc, char * argv[]) { // argv[1] = fileURL
    #ifdef DEBUG
    printf("Running in DEBUG mode.\n");
    #endif // DEBUG

    // argument check
    if (argc != 2) {
        printf("Indication: pass in the compiled file, e.g. runtime ./factoriel.lacc\n");
        return 3;
    }

    printf("Welcome. Executor is being prepared to run your program.\nIt is written by Nicolas YING following the course of TPLA presented by Alain Chillès.\n \
\nThis executor follows the standard Hugolonicolasien 1.4, supporting .lacc files with standard prior to version 1.2.\n \
INPUT_FILE is %s,\nEnjoy.\n", argv[1]);

    // get file size
    FILE *fp;

    fp = fopen (argv[1], "r+" );
    if (fp==NULL) {fputs ("File error", stderr); exit (601);}

    // obtain file size:
    fseek (fp , 0 , SEEK_END);
    long lSize = ftell (fp) / 4;
    rewind (fp);

    // allocate memory to contain the whole file:
    VMp = (int*) malloc (lSize);
    if (VMp == NULL) {fputs ("Memory error", stderr); exit (602);}

    // copy the file into the buffer:
    int i = 0;
    while (fscanf (fp, "%d", &result) == 1) {
        VMp[i++] = result;
        #ifdef DEBUG
        printf("Reading Interation reads: %d\n", result);
        #endif // DEBUG
    }
    VMp[i] = -9000; // to mark the end
    result = i;
    /* the whole file is now loaded in the memory buffer. */

    #ifdef DEBUG
    printf("lSize is %d.\n", result);
    int j = 0;
    while (j < result) {
        printf("%d, ", VMp[j++]);
    }
    fputs ("\n", stdin);
    #endif // DEBUG
    
    fclose (fp);

    // Compatibility check
    // if (VMp[0] < 1002) {
    //     printf("VM version is not supported by this machine.\nAbort execution.\n");
    //     return 665;
    // }

    // Initialisation of environment
    int stringMem[MAX_STRING_SIZE] = {0};
    int posMemory = 0, posVM = VMp[1]; // set to entry point
    basicFunc processor[PROCESS_FUN_QUAN];

    // Initialisation of L_ac components
    data = malloc(sizeof(basicStack));
    type = malloc(sizeof(basicStack));
    call = malloc(sizeof(basicStack));
    data->data = 0;
    data->precedent = NULL;
    type->data = 0;
    type->precedent = NULL;
    call->data = 0;
    call->precedent = NULL;
    initProcessor(processor);
    linkProcessor(&data, &type, &call, &stringMem[0], VMp, &posMemory);

    // begin executing main function 
    int functionType = 1; // by default it is running main function
    int functionTypeOld = 1;

    if ( posVM >= result ) { // violation check
        free(VMp);
        printf("Entry point error.\n");
        return 666;
    }

    printf("Begin execution.\n\n\n\n");

    while (1) { 
        if (functionType != 0) { // check function type
            // it's a LAC function, or main function
            functionTypeOld = functionType;
            pushStack(++posVM, &call);
            if (posVM >= result) {
                break;
            }
            posVM = VMp[posVM];
            functionType = VMp[posVM];
            //posVM = popStack(&call) + 1;
        } else {
            // it's a base function
            processor[VMp[++posVM]]();
            posVM = popStack(&call);
            functionType = functionTypeOld;
            // pushStack(posVM, &call);
        }
    }
    
    popStack(&call);
    
    printf("And the final stack state is like:\n");
    displayStack(&data, &type, &stringMem[0]);
    clearStack(&data); clearStack(&type); 

    #ifdef DEBUG
    printf("End of execution.\n");
    #endif // DEBUG

    free (VMp);
    return 0;
}