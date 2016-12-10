//
//  Compiler
//  Compiler Lac_reorg
//
//  Created by Nicolas YING on 07/12/2016.
//  Copyright © 2016 Nicolas YING. All rights reserved.
//
//  Error code 7**
//  generate a VM array complying to the VM_LAC_hugolonicolasien standard 1.3

#include "common_component.h"
#include "analyse_lexical.h"
#include "compiler.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define NOT_COMPILING 7833
#define COMPILING_FUN 7943
#define COMPILING_MAIN 7317
#define COMPILING_COND 7704
#define MAX_IN_OUT_PUT_NUMBER 100
#define MAX_COND_BRANCH_LEVEL 10

int functionCompilingState = NOT_COMPILING; // a flag used to avoid nested definition
int cCFBegin = 0; // a container for the beginning VM position for this function, for recurse.
int cCFBeginLAC = 0; // a container for the beginning symbol table position for this function
int cCFNameLexPos = 0, cCFParaInCnt = 0, cCFParaOutCnt = 0, cCFParaCnt = 0, cCFParaPos = MAX_IN_OUT_PUT_NUMBER; // to simulate stack change
int cCFParaInArray[MAX_IN_OUT_PUT_NUMBER] = {0}, cCFParaOutArray[MAX_IN_OUT_PUT_NUMBER] = {0}, cCFParaArray[MAX_IN_OUT_PUT_NUMBER * 2] = {0};
static int litposVM = 0, strposVM = 0, finposVM = 0, recurseposSymbol = 0, mainPosVM = 0, ifposSymbol = 0, thenposSymbol = 0, elseposSymbol = 0;
static int condBranchLevel = -1;

struct condBranchSavedStateStruct {
    int bTBegin = 0, bTParaInCnt = 0, bTParaOutCnt = 0, bFBegin = 0, bFParaInCnt = 0, bFParaOutCnt = 0;
    int cBParaCnt = 0, cBParaPos = MAX_IN_OUT_PUT_NUMBER;
    int bTParaInArray[MAX_IN_OUT_PUT_NUMBER] = {0}, bTParaOutArray[MAX_IN_OUT_PUT_NUMBER] = {0}, bFParaInArray[MAX_IN_OUT_PUT_NUMBER] = {0}, bFParaOutArray[MAX_IN_OUT_PUT_NUMBER] = {0}, 
    int cBParaArray[MAX_IN_OUT_PUT_NUMBER * 2] = {0};
}

typedef struct condBranchSavedStateStruct condBchSState;
static condBchState[MAX_COND_BRANCH_LEVEL];

void initLacCompile(int * symbolTable, int * VM, int * posSymbol, int * posVM) {
    // Cleaning the tables
    memset(symbolTable, 0, SYMBOL_TABLE_SIZE);
    memset(VM, 0, VM_SIZE);

    // Adding funtions allowed in compile mode
    // It should be exactly the same in executer
    VM[(*posVM)++] = VM_VERSION; // VM version 1004, correspongding to VM_LAC_hugolonicolasien 1.4
    VM[(*posVM)++] = 0; // For point d'entree
    litposVM = *posVM;
    addBaseFunction (symbolTable, VM, posSymbol, posVM, 0, "lit", 0, (int[]){}, 0, (int[]){});
    strposVM = *posVM;
    addBaseFunction (symbolTable, VM, posSymbol, posVM, 1, "str", 0, (int[]){}, 0, (int[]){});
    finposVM = *posVM;
    addBaseFunction (symbolTable, VM, posSymbol, posVM, 2, "fin", 0, (int[]){}, 0, (int[]){});
    addBaseFunction (symbolTable, VM, posSymbol, posVM, 3, ".", 1, (int[]){ANY}, 0, (int[]){});
    addBaseFunction (symbolTable, VM, posSymbol, posVM, 4, "+", 2, (int[]){ENTIER, ENTIER}, 1, (int[]){ENTIER}); 
    addBaseFunction (symbolTable, VM, posSymbol, posVM, 5, "-", 2, (int[]){ENTIER, ENTIER}, 1, (int[]){ENTIER}); 
    addBaseFunction (symbolTable, VM, posSymbol, posVM, 6, "*", 2, (int[]){ENTIER, ENTIER}, 1, (int[]){ENTIER}); 
    addBaseFunction (symbolTable, VM, posSymbol, posVM, 7, "=", 2, (int[]){ENTIER, ENTIER}, 1, (int[]){BOOLEAN}); 
    addBaseFunction (symbolTable, VM, posSymbol, posVM, 8, "dup", 1, (int[]){ANY}, 2, (int[]){ANY, ANY}); 
    addBaseFunction (symbolTable, VM, posSymbol, posVM, 9, "drop", 1, (int[]){ANY}, 0, (int[]){}); 
    addBaseFunction (symbolTable, VM, posSymbol, posVM, 10, "swap", 2, (int[]){ANY, ANY}, 2, (int[]){ANY, ANY}); 
    addBaseFunction (symbolTable, VM, posSymbol, posVM, 11, "count", 1, (int[]){CHAINECHAR}, 2, (int[]){CHAINECHARNOHEADER, ENTIER}); 
    addBaseFunction (symbolTable, VM, posSymbol, posVM, 12, "type", 2, (int[]){ENTIER, CHAINECHARNOHEADER}, 0, (int[]){}); 
    ifposSymbol = *posSymbol + 1;
    addBaseFunction (symbolTable, VM, posSymbol, posVM, 13, "if", 1, (int[]){BOOLEAN}, 0, (int[]){});
    elseposSymbol = *posSymbol + 1;
    addBaseFunction (symbolTable, VM, posSymbol, posVM, 14, "else", 0, (int[]){}, 0, (int[]){});
    thenposSymbol = *posSymbol + 1;
    addBaseFunction (symbolTable, VM, posSymbol, posVM, 15, "then", 0, (int[]){}, 0, (int[]){});
    addBaseFunction (symbolTable, VM, posSymbol, posVM, 16, "/", 2, (int[]){ENTIER, ENTIER}, 1, (int[]){ENTIER}); 
    addBaseFunction (symbolTable, VM, posSymbol, posVM, 20, "&&", 2, (int[]){BOOLEAN, BOOLEAN}, 1, (int[]){BOOLEAN});
    addBaseFunction (symbolTable, VM, posSymbol, posVM, 21, "||", 2, (int[]){BOOLEAN, BOOLEAN}, 1, (int[]){BOOLEAN});
    addBaseFunction (symbolTable, VM, posSymbol, posVM, 22, "!", 1, (int[]){BOOLEAN}, 1, (int[]){BOOLEAN});
    addBaseFunction (symbolTable, VM, posSymbol, posVM, 23, "<=", 2, (int[]){ENTIER, ENTIER}, 1, (int[]){BOOLEAN});
    recurseposSymbol = *posSymbol + 1;
    addBaseFunction (symbolTable, VM, posSymbol, posVM, 25, "recurse", -1, (int[]){}, -1, (int[]){}); 
    addBaseFunction (symbolTable, VM, posSymbol, posVM, 28, "calculate", 1, (int[]){CHAINECHAR}, 0, (int[]){}); 
    addBaseFunction (symbolTable, VM, posSymbol, posVM, 29, "catenate", 1, (int[]){CHAINECHAR}, 0, (int[]){}); 
    addBaseFunction (symbolTable, VM, posSymbol, posVM, 30, "cr", 0, (int[]){}, 1, (int[]){CHAINECHAR}); 

    #ifdef DEBUG
    printf("mode_compilé:\nSymbol table and VM constructed.\n");
    int i = 0;
    while (i <= *posSymbol) {
        printf("%d: %d\n", i, symbolTable[i]);
        i++;
    }
    printf("---------\n");
    i = 0;
    while (i <= *posVM) {
        printf("%d: %d\n", i, VM[i]);
        i++;
    }
    #endif // DEBUG
}

int typeConversion(int type1, int type2) { // Evaluate ANY type
    if (type1 > type2){
        type1 ^= type2;
        type2 ^= type1;
        type1 ^= type2;
    } // so type1 <= type2
    if (type1 == ANY || type1 == type2) return type2;
    if (type1 != type2) return 10; // only ANY can be resolved to other types
    return 11; // to resolve warning message
}

int main(int argc, char * argv[]) { // argv[1] = fileURL
    #ifdef DEBUG
    printf("Running in DEBUG mode.\n");
    #endif // DEBUG

    // argument check
    if (argc != 2) {
        printf("Indication: pass in the source code, e.g. compiler_l_ac ./factoriel.lac\n");
        return 3;
    }

    // get file size
    FILE *fp;
    long size_file;
    fp = fopen(argv[1], "r");
    fseek(fp, 0L, SEEK_END);
    size_file = ftell(fp);
    rewind(fp);

    // get file chars into one single string
    char texte[size_file];
    char tmp[100];
    memset(texte, 0, size_file);
    memset(tmp, 0, 100);
    
    while (fgets(tmp, 100, fp)) {
        strcat(texte, tmp);
    }
    fclose(fp);
    
    #ifdef DEBUG
    printf("---main---\nRead Buffer: \n%s\n", texte);
    #endif // DEBUG

    // initialisation of environment
    int symbolTable[SYMBOL_TABLE_SIZE] = {0}, VM[VM_SIZE] = {0};
    lexeme_Element lexemeList[MAX_LEXEME_NUMBER];
    int posSymbol = 0, posVM = 0; 

    // initialisation of L_ac components
    initLacCompile(symbolTable, VM, &posSymbol, &posVM);

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

    printf("Welcome. Compiler is running.\nIt is written by Nicolas YING following the course of TPLA presented by Alain Chillès.\n \
\nThis compiler follows the standard Hugolonicolasien 1.4.\nIt is very sensitive to code error. \
\nPlease review the code before compiling.\n\nThis compiler supports up to 10 levels of nesting conditional branch, \
and if recursive procedures are involed, it will neglect input and output constraints. So good luck with that. \
\n\nINPUT_FILE is %s, \
\nOUTPUT_FILE will be %s.lacc\nEnjoy.\n", argv[1], argv[1]);

    // Pass to lexcial analysis, which is a common component
    int lexemeNumber = analyseLexical(texte, lexemeList, &regExp);
    int posLexeme = 0;
    // Interpret the lexemes
    do {
        #ifdef DEBUG
        printf("functionCompilingState: %d\n", functionCompilingState);
        #endif // DEBUG
        if(lexemeList[posLexeme].type == C) {
            #ifdef DEBUG
            printf("Adding a string to VM");
            #endif // DEBUG
            // Check the state, if it's not compiling a function, it's compiling main (the last block of .lac file).
            if (functionCompilingState == NOT_COMPILING ) {
                #ifdef DEBUG
                printf("Entering main function compilation beginning with an identifier.\n");
                #endif // DEBUG
                functionCompilingState = COMPILING_MAIN;
                mainPosVM = posVM;
                VM[posVM++] = 2; // lac main function
            }
            // Update parameter statistics
            if (cCFParaOutCnt >= 0 && cCFParaInCnt >= 0) {
                cCFParaArray[cCFParaPos++] = CHAINECHAR;
                cCFParaOutArray[cCFParaOutCnt++] = CHAINECHAR;
                cCFParaCnt++;
            } else { // a function which inputs and outpus cannot be prevaluated
                cCFParaOutCnt = -1;
                cCFParaInCnt = -1;
            }
            // Put the string into VM using str
            int length = lexemeList[posLexeme].end - lexemeList[posLexeme].begin + 1;
            // VM position for str
            VM[posVM++] = strposVM;
            // Copy the string into VM
            VM[posVM++] = length;
            int i = 0;
            while (i < length) {
                VM[posVM++] = texte[lexemeList[posLexeme].begin + i++];
            }
        } else if (lexemeList[posLexeme].end - lexemeList[posLexeme].begin == 0 && texte[lexemeList[posLexeme].begin] == ':') {
            #ifdef DEBUG
            printf("Defining a new function.\n");
            #endif // DEBUG
            // Define a new function, check if definition is nested
            if (functionCompilingState == COMPILING_FUN) {
                printf("Function definition cannot be nested.\n");
                printf("%.*s... (pos: %d)\n", 10, texte + lexemeList[posLexeme].begin, lexemeList[posLexeme].begin);
                exit(700);
            }
            if (functionCompilingState == COMPILING_MAIN) {
                printf("Error: Function definition after code execution. Move your function definition above main codes.\n");
                printf("%.*s... (pos: %d)\n", 10, texte + lexemeList[posLexeme].begin, lexemeList[posLexeme].begin);
                exit(702);
            }
            // Check if a function with the same name exists
            if (findFunction(posSymbol, symbolTable, &lexemeList[posLexeme + 1], texte) > 0) {
                printf("A function with the same name exists.\n");
                printf("%.*s... (pos: %d)\n", 10, texte + lexemeList[posLexeme + 1].begin, lexemeList[posLexeme + 1].begin);
                exit(701);
            }
            // Initialise flags
            cCFParaInCnt = 0;
            cCFParaOutCnt = 0; 
            cCFParaCnt = 0;
            cCFBegin = posVM;
            cCFBeginLAC = posSymbol + 1;
            memset(cCFParaInArray, 0, MAX_IN_OUT_PUT_NUMBER);
            memset(cCFParaOutArray, 0, MAX_IN_OUT_PUT_NUMBER);
            memset(cCFParaArray, 0, 2 * MAX_IN_OUT_PUT_NUMBER);
            functionCompilingState = COMPILING_FUN;
            cCFParaPos = MAX_IN_OUT_PUT_NUMBER;
            // Writing into VM
            VM[posVM++] = 1; // A user-defined function is marked by 1
            // Take note of the function name, store as a LAC string
            cCFNameLexPos = ++posLexeme;

        } else if (lexemeList[posLexeme].end - lexemeList[posLexeme].begin == 0 && texte[lexemeList[posLexeme].begin] == ';') {
            // End of the defnition
            #ifdef DEBUG
            printf("End defining a new function.\n");
            #endif // DEBUG
            if (functionCompilingState != COMPILING_FUN) {
                printf("Error: There's no function being defined.\n");
                printf("%.*s (pos: %d)\n", 10, texte + lexemeList[posLexeme].begin, lexemeList[posLexeme].begin);
                exit(701);
            }
            VM[posVM++] = finposVM; // VM position for fin
            
            // Add info to symbol table
            // Name
            posSymbol++;
            int nameLength = lexemeList[cCFNameLexPos].end - lexemeList[cCFNameLexPos].begin + 1;
            symbolTable[posSymbol++] = nameLength;
            int i = 0;
            while (i < nameLength) {
                symbolTable[posSymbol++] = texte[lexemeList[cCFNameLexPos].begin + i++];
            }
            // Inputs Outputs
            symbolTable[posSymbol++] = cCFParaInCnt;
            i = 0;
            while (i < cCFParaInCnt) {
                symbolTable[posSymbol++] = cCFParaInArray[i++];
            }
            symbolTable[posSymbol++] = cCFParaOutCnt;
            i = 0;
            while (i < cCFParaOutCnt) {
                symbolTable[posSymbol++] = cCFParaInArray[i++];
            }
            symbolTable[posSymbol++] = cCFBegin; // VM association
            symbolTable[posSymbol] = cCFBeginLAC; // symbol table pos_begin
            // Mark the end of definition
            functionCompilingState = NOT_COMPILING;

            #ifdef DEBUG // dumping out table and VM
            printf("mode_compilé:\n\nNEW Symbol table and VM.\n");
            i = 0;
            while (i <= posSymbol) {
                printf("%d: %d\n", i, symbolTable[i]);
                i++;
            }
            printf("---------\n");
            i = 0;
            while (i <= posVM) {
                printf("%d: %d\n", i, VM[i]);
                i++;
            }
            #endif // DEBUG

            // Initialise flags
            cCFParaInCnt = 0;
            cCFParaOutCnt = 0; 
            cCFParaCnt = 0;
            memset(cCFParaInArray, 0, MAX_IN_OUT_PUT_NUMBER);
            memset(cCFParaOutArray, 0, MAX_IN_OUT_PUT_NUMBER);
            memset(cCFParaArray, 0, 2 * MAX_IN_OUT_PUT_NUMBER);
            cCFParaPos = MAX_IN_OUT_PUT_NUMBER;

        } else {
            // It is an normal identifier
            // Check the state, if it's not compiling a function, it's compiling main (the last block of .lac file).
            if (functionCompilingState == NOT_COMPILING ) {
                #ifdef DEBUG
                printf("Entering main function compilation beginning with an identifier.\n");
                #endif // DEBUG
                functionCompilingState = COMPILING_MAIN;
                mainPosVM = posVM;
                VM[posVM++] = 2; // lac main function
            }
            #ifdef DEBUG
            printf("symbolTable: %d, %d\n", posSymbol, symbolTable[posSymbol]);
            #endif // DEBUG
            int posSymbolC = findFunction(posSymbol, symbolTable, &lexemeList[posLexeme], texte);
            if (posSymbolC == ifposSymbol) {
                
            } else if (posSymbolC == elseposSymbol) {
                
            } else if (posSymbolC == thenposSymbol) {
                
            } else if (posSymbolC == recurseposSymbol) {
                // update parameter statistics
                cCFParaOutCnt = -1;
                cCFParaInCnt = -1;
                printf("Recurse is used in the function definition.\nNicolas is lazy, so the compiler now will not evaluate the function inputs and outputs.\nErrors may occur in execution.\nBonne chance.:)\n");
                // put the pos_begin VM of current compiled function into VM
                if (functionCompilingState == COMPILING_FUN) VM[posVM++] = cCFBegin;
                else if (functionCompilingState == COMPILING_MAIN) VM[posVM++] = mainPosVM;
                else {
                    printf("It should not be possible to display this message, it must be the C compiler that's wrong.\n");
                    return(744);
                }
            } else if (posSymbolC > 0){ // a function is found, find it's VM position
                if (cCFParaOutCnt >= 0 && cCFParaInCnt >= 0) { // if the state is still valid, update parameter statistics
                    int lenName = symbolTable[posSymbolC];
                    int paraInCnt = symbolTable[posSymbolC + lenName + 1];
                    int paraOutCnt = symbolTable[posSymbolC + lenName + paraInCnt + 2];
                    int posVMC = symbolTable[posSymbolC + lenName + paraInCnt + paraOutCnt + 3];
                    if (paraInCnt >= 0 && paraOutCnt >= 0) { // a normal function
                        // simulate executing the function
                        int i = 0;
                        while (i < paraInCnt) { // inputs
                            // for cCFParaArray that is not yet used, it is initialised to 0 - ANY
                            int typeCheck = typeConversion(cCFParaArray[cCFParaPos-1], symbolTable[posSymbolC + lenName + i + 2]);
                            if (typeCheck < 6 ) { // type is compatible
                                if(cCFParaOutCnt > 0) cCFParaOutCnt--; // reduce from outputs
                                cCFParaArray[cCFParaPos--] = ANY; // moving parameter stack
                                cCFParaCnt--;
                                if(cCFParaCnt < 0) {
                                    // an additional input is needed
                                    cCFParaInArray[cCFParaInCnt++] = typeCheck;
                                    cCFParaCnt++;  
                                }
                                i++;
                            } else { // type is incompatible
                                printf("Error code 711: type incompatible.\n");
                                printf("%.*s (pos: %d)\n", 10, texte + lexemeList[posLexeme].begin, lexemeList[posLexeme].begin);
                                return(711);
                            }
                        }
                        i = 0;
                        while (i < paraOutCnt) { // generate outputs
                            cCFParaOutArray[cCFParaOutCnt++] = symbolTable[posSymbolC + lenName + paraInCnt + i + 3]; 
                            cCFParaArray[cCFParaPos++] = symbolTable[posSymbolC + lenName + paraInCnt + i + 3]; 
                            cCFParaCnt++;
                            i++;
                        }
                    } else { // a function which inputs and outpus cannot be prevaluated
                        cCFParaOutCnt = -1;
                        cCFParaInCnt = -1;
                    }
                } else {
                    cCFParaOutCnt = -1;
                    cCFParaInCnt = -1;
                }
                // add function VM address into VM
                VM[posVM++] = posVMC;
            } else { // then it must be a number
                int number;
                if (convertLexeme2Number(texte, &lexemeList[posLexeme], &number) == 0) {
                    // Update parameter statistics
                    if (cCFParaOutCnt >= 0 && cCFParaInCnt >= 0) {
                        cCFParaArray[cCFParaPos++] = ENTIER;
                        cCFParaOutArray[cCFParaOutCnt++] = ENTIER;
                        cCFParaCnt++;
                    } else { // a function which inputs and outpus cannot be prevaluated
                        cCFParaOutCnt = -1;
                        cCFParaInCnt = -1;
                    }
                    // Put the number into VM using lit
                    // VM position for lit
                    VM[posVM++] = litposVM;
                    // Copy the string into VM
                    VM[posVM++] = number;
                } else { // not a number
                    printf("Error code 712: Convert number failed.\n");
                    printf("%.*s (pos: %d)\n", 10, texte + lexemeList[posLexeme].begin, lexemeList[posLexeme].begin);
                    return(712);
                }
            }
        }
        posLexeme++;
    } while (posLexeme < lexemeNumber);
    if (cCFParaInCnt != 0) {
        printf("Error code 713: input not sufficient.\n"); 
        return 713;
    } else printf("\nCompilation logic finished.\n"); 
    posVM--;

    // End of the compilation
    #ifdef DEBUG
    printf("Finishing up.\n");
    #endif // DEBUG

    if (functionCompilingState != COMPILING_MAIN) {
        printf("Error: We're not compiling main.\n");
        printf("%.*s (pos: %d)\n", 10, texte + lexemeList[posLexeme].begin, lexemeList[posLexeme].begin);
        exit(701);
    }

    // VM point d'entree
    VM[1] = mainPosVM; 
    int i = 0;
    // generate compiled file
    #ifdef DEBUG
    printf("mode_compilé:\n\nVM point d'entrée: %d.\n", mainPosVM);
    i = 0;
    while (i <= posVM) {
        printf("%d: %d\n", i, VM[i]);
        i++;
    }
    #endif // DEBUG
    
    i = strlen(argv[1]);
    argv[1][i] = 'c'; argv[1][i+1] = '\0';
    fp = fopen(argv[1], "wb");
    fwrite (&VM[0], sizeof(int32_t), posVM + 1, fp);
    printf("Compiled file is generated.\nCheckout %s.\n", argv[1]);
    fclose (fp);

    return 0;
}