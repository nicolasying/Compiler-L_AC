//
//  Mode Calculatrice
//  Compiler Lac_reorg
//
//  Created by Nicolas YING on 06/12/2016.
//  Copyright © 2016 Nicolas YING. All rights reserved.
//

#include "mode_calculatrice.h"
#include "analyse_lexical.h"

int main(int argc, char * argv[]) { // it should not take any input
    
    // Initialisation of environment
    char lineBuffer[MAX_LINE_LENGTH] = {0};
    lexeme_Element lexemeList[MAX_LEXEME_NUMBER];
    int memorySpace[MAX_STRING_SIZE] = {0};
    
    while(1) {
        //scanf("%d[^\n]", lineBuffer);
        analyseLexical(lineBuffer, lexemeList);
    }
}