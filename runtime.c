//
//  Runtime
//  Compiler Lac_reorg
//
//  Created by Nicolas YING on 07/12/2016.
//  Copyright © 2016 Nicolas YING. All rights reserved.
//
//  Error code 6**
//  generate a VM array complying to the VM_LAC_hugolonicolasien standard 1.3

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char * argv[]) { // argv[1] = fileURL
    #ifdef DEBUG
    printf("Running in DEBUG mode.\n");
    #endif // DEBUG

    // argument check
    if (argc != 2) {
        printf("Indication: pass in the compiled file, e.g. runtime ./factoriel.lacc\n");
        return 3;
    }

    // get file size
    FILE *fp;
    long size_file;
    fp = fopen(argv[1], "wb");
    
    int testArray[15] = {0, 1, 2, 5, 7, 8};
    fwrite (&testArray[0], sizeof(int), 15, fp);
    printf("file written. Size is %ld.\n", sizeof(testArray));
    fclose (fp);

    // reading part.
    int * buffer;
    size_t result;

    printf("Now reading.");

    fp = fopen (argv[1], "rb" );
    if (fp==NULL) {fputs ("File error",stderr); exit (1);}

    // obtain file size:
    fseek (fp , 0 , SEEK_END);
    long lSize = ftell (fp);
    rewind (fp);

    // allocate memory to contain the whole file:
    buffer = (int*) malloc (lSize);
    if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}

    // copy the file into the buffer:
    result = fread (buffer, sizeof(int), lSize/sizeof(int) , fp);
    if (result != lSize/sizeof(int)) {fputs ("Reading error",stderr); exit (3);}
    /* the whole file is now loaded in the memory buffer. */

    // terminate
    fclose (fp);
    free (buffer);
    return 0;
}