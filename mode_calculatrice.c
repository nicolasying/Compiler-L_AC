//
//  Mode Calculatrice
//  Compiler Lac_reorg
//
//  Created by Nicolas YING on 06/12/2016.
//  Copyright © 2016 Nicolas YING. All rights reserved.
//
//  Error code: 0**

#include "mode_calculatrice.h"
#include "analyse_lexical.h"
#include "common_component.h"
#include "processor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>


void initLacInterprete(int * symbolTable, int * VM, int * posSymbol, int * posVM) {
    // Cleaning the tables
    memset(symbolTable, 0, SYMBOL_TABLE_SIZE);
    memset(VM, 0, VM_SIZE);

    addBaseFunction (symbolTable, VM, posSymbol, posVM, 3, ".", 1, (int[]){ENTIER}, 0, (int[]){});
    addBaseFunction (symbolTable, VM, posSymbol, posVM, 4, "+", 2, (int[]){ENTIER, ENTIER}, 1, (int[]){ENTIER}); 
    addBaseFunction (symbolTable, VM, posSymbol, posVM, 5, "-", 2, (int[]){ENTIER, ENTIER}, 1, (int[]){ENTIER}); 
    addBaseFunction (symbolTable, VM, posSymbol, posVM, 6, "*", 2, (int[]){ENTIER, ENTIER}, 1, (int[]){ENTIER}); 
    addBaseFunction (symbolTable, VM, posSymbol, posVM, 7, "=", 2, (int[]){ENTIER, ENTIER}, 1, (int[]){BOOLEAN}); 
    addBaseFunction (symbolTable, VM, posSymbol, posVM, 8, "dup", 1, (int[]){ANY}, 2, (int[]){ANY, ANY}); 
    addBaseFunction (symbolTable, VM, posSymbol, posVM, 9, "drop", 1, (int[]){ANY}, 0, (int[]){}); 
    addBaseFunction (symbolTable, VM, posSymbol, posVM, 10, "swap", 2, (int[]){ANY, ANY}, 0, (int[]){}); 
    addBaseFunction (symbolTable, VM, posSymbol, posVM, 11, "count", 1, (int[]){CHAINECHAR}, 2, (int[]){CHAINECHARNOHEADER, ENTIER}); 
    addBaseFunction (symbolTable, VM, posSymbol, posVM, 12, "type", 2, (int[]){CHAINECHARNOHEADER, ENTIER}, 0, (int[]){}); 
    addBaseFunction (symbolTable, VM, posSymbol, posVM, 28, "calculate", 1, (int[]){CHAINECHAR}, 0, (int[]){}); 

    #ifdef DEBUG
    printf("mode_calculatrice:\nSymbol table and VM constructed.\n");
    #endif // DEBUG
}

int main(int argc, char * argv[]) { // it should not take any input
    #ifdef DEBUG
    printf("Running in DEBUG mode.\n");
    #endif // DEBUG

    // Initialisation of environment
    char lineBuffer[MAX_LINE_LENGTH] = {0};
    lexeme_Element lexemeList[MAX_LEXEME_NUMBER];
    basicStack *data, *type;
    int symbolTable[SYMBOL_TABLE_SIZE] = {0}, VM[VM_SIZE] = {0};
    char stringMem[MAX_STRING_SIZE] = {0};
    int posMemory = 0, posSymbol = 0, posVM = 0;
    basicFunc processor[PROCESS_FUN_QUAN];

    // Initialisation of L_ac components
    data = malloc(sizeof(basicStack));
    type = malloc(sizeof(basicStack));
    data->data = 0;
    data->precedent = NULL;
    type->data = 0;
    type->precedent = NULL;
    initProcessor(processor);
    linkProcessor(&data, &type, NULL, &stringMem[0], VM);
    initLacInterprete(symbolTable, VM, &posSymbol, &posVM);

    // Regular Expressions Definition
    const char expr[] = "((^| )\" ([^()\"]*[\\].*|[^()\\]*[\\(][^()\\]*[\\)][^()\\]*)\")|((^|[ \t\n])[\\]( |\t)[^\n]*($|\n))|((^|[ \t\n])[(] [^)]*[)])|((^|[ \t\n])[\"] [^\"]*[\"])|([-+/:;.\\=\\*0-9a-zA-Z[:punct:]]+)";
    // ordre:
    // char chaine containing " and \, "(^|[ \t\n])[\"]((^|[ \t\n])[\\] [^()\n]*($|\n))*((^|[ \t\n])[(] [^)]*[)])*[\"]"
    // single line comment, "(^|[ \t\n])[\\] [^()\n]*($|\n)"
    // multiline comment, "(^|[ \t\n])[(] [^)]*[)]"
    // char chaine, "(^|[ \t\n])[\"] [^\"]*[\"]"
    // punctuations and remainders, "[:=;+-*/.,1-9a-zA-Z]+"

    // Regular Expression Compilation
    regex_t regExp;
    if (regcomp(&regExp, expr, REG_EXTENDED)) {
        printf("Analyse lexical: \nMauvaise exp rationnelle : %s\n", expr);
        regfree(&regExp);
        exit(101);
    }
    #ifdef DEBUG
    printf("analyse_lexical: \nRegular Expression compiled.\n");
    #endif // DEBUG

    printf("Interpreter is running.\nWritten by Nicolas YING, this interpreter supports dup, drop, swap, ., count, type, =, and calculate.\nType in QUIT to exit.\n\nNotice: if the result calculation process went wrong, it could return a false result with a message, or crash :)\nEnjoy.\n");
    char quit[] = "QUIT";

    while(1) {
        // Take input from the shell
        printf("L_AC:> ");
        gets(lineBuffer);

        // Check if it is QUIT
        if (strcmp(lineBuffer, quit) == 0) break;

        // Pass to lexcial analysis, which is a common component
        int lexemeNumber = analyseLexical(lineBuffer, lexemeList, &regExp);
        int posLexeme = 0;
        // Interpret the lexemes
        do {
            if(lexemeList[posLexeme].type == C) {
                // Put the string into stringMem
                int length = lexemeList[posLexeme].end - lexemeList[posLexeme].begin + 1;
                pushStack(posMemory, &data);
                pushStack(CHAINECHAR, &type);
                stringMem[posMemory++ % MAX_STRING_SIZE] = length;
                strncpy(&stringMem[posMemory % MAX_STRING_SIZE], &lineBuffer[lexemeList[posLexeme].begin], length);
                posMemory += length;
            } else {
                // It is an identifier
                int posSymbolC = findFunction(posSymbol, symbolTable, &lexemeList[posLexeme], lineBuffer);
                if (posSymbolC > 0){
                    // a function is found, find it's VM position
                    int lenName = symbolTable[posSymbolC];
                    int paraIn = symbolTable[posSymbolC + lenName + 1];
                    int paraOut = symbolTable[posSymbolC + lenName + paraIn + 2];
                    int posVMC = symbolTable[posSymbolC + lenName + paraIn + paraOut + 3];

                    if (VM[posVMC] != 0) {
                        printf("Found a non valid function. Abort.\n");
                        break;
                    } else {
                        // run the function
                        processor[VM[posVMC + 1]]();
                    }
                } else {
                    // then it must be a number
                    int number;
                    if (convertLexeme2Number(lineBuffer, &lexemeList[posLexeme], &number) == 0) {
                        pushStack(number, &data);
                        pushStack(ENTIER, &type);
                    } else break;
                }
            }
            posLexeme++;
        } while (posLexeme < lexemeNumber);

        // One line is executed, display the stack
        displayStack(&data, &type,  &stringMem[0]);
        memset(lineBuffer, 0, MAX_LINE_LENGTH);
    }

    clearStack(&data); clearStack(&type); 
    // free(&regExp);
    return 0;
}


