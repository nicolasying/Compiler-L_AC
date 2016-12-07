//
//  Compiler
//  Compiler Lac_reorg
//
//  Created by Nicolas YING on 07/12/2016.
//  Copyright © 2016 Nicolas YING. All rights reserved.
//
//  Error code 7**
//  generate a VM array complying to the VM_LAC_hugolonicolasien standard

#include "common_component.h"
#include "analyse_lexical.h"
#include "compiler.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void initLacCompile(int * symbolTable, int * VM, int * posSymbol, int * posVM) {
    // Cleaning the tables
    memset(symbolTable, 0, SYMBOL_TABLE_SIZE);
    memset(VM, 0, VM_SIZE);

    // Adding funtions allowed in compile mode
    addBaseFunction (symbolTable, VM, posSymbol, posVM, 0, "lit", 0, (int[]){}, 0, (int[]){});
    addBaseFunction (symbolTable, VM, posSymbol, posVM, 1, "str", 0, (int[]){}, 0, (int[]){});
    addBaseFunction (symbolTable, VM, posSymbol, posVM, 2, "fin", 0, (int[]){}, 0, (int[]){});
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
    addBaseFunction (symbolTable, VM, posSymbol, posVM, 13, "if", 1, (int[]){BOOLEAN}, 0, (int[]){});
    addBaseFunction (symbolTable, VM, posSymbol, posVM, 14, "else", 0, (int[]){}, 0, (int[]){});
    addBaseFunction (symbolTable, VM, posSymbol, posVM, 15, "then", 0, (int[]){}, 0, (int[]){});
    addBaseFunction (symbolTable, VM, posSymbol, posVM, 20, "&&", 2, (int[]){BOOLEAN, BOOLEAN}, 1, (int[]){BOOLEAN});
    addBaseFunction (symbolTable, VM, posSymbol, posVM, 21, "||", 2, (int[]){BOOLEAN, BOOLEAN}, 1, (int[]){BOOLEAN});
    addBaseFunction (symbolTable, VM, posSymbol, posVM, 22, "!", 1, (int[]){BOOLEAN}, 1, (int[]){BOOLEAN});
    addBaseFunction (symbolTable, VM, posSymbol, posVM, 23, "<=", 2, (int[]){ENTIER, ENTIER}, 1, (int[]){BOOLEAN});
    addBaseFunction (symbolTable, VM, posSymbol, posVM, 25, "recurse", 0, (int[]){}, 0, (int[]){}); 
    addBaseFunction (symbolTable, VM, posSymbol, posVM, 28, "calculate", 1, (int[]){CHAINECHAR}, 0, (int[]){}); 
    addBaseFunction (symbolTable, VM, posSymbol, posVM, 29, "catenate", 1, (int[]){CHAINECHAR}, 0, (int[]){}); 

    #ifdef DEBUG
    printf("mode_compilé:\nSymbol table and VM constructed.\n");
    #endif // DEBUG
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

    printf("Compiler is running.\nWritten by Nicolas YING.\n \
\nThis compiler is very sensitive to code error. \
\nPlease review the code before compiling. \
\nEnjoy.\n \
\nINPUT_FILE is %s, \
\nOUTPUT_FILE will be %s.lacc\n", argv[1], argv[1]);

    return 0;
}