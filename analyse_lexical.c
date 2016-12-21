//
//  Lexical Analysis
//  Compiler Lac_reorg
//
//  Created by Nicolas YING on 06/12/2016.
//  Copyright © 2016 Nicolas YING. All rights reserved.
//
//  Error code: 1**

#include "common_component.h"
#include "analyse_lexical.h"
#include <stdio.h>
#include <regex.h>
#include <stdlib.h>
#include <string.h>

int analyseLexical(char * textToAnalyse, lexeme_Element * lexeme_List, regex_t * regExp) {

    // Matching Expressions against the input text
    regmatch_t pmatch[3];
    int res = 0, offset = 0, t_c = 0;
    char *p = textToAnalyse;

    do {
        if (t_c > MAX_LEXEME_NUMBER) {
            // lexeme_List can no longer contain more lexemes
            printf("Lexemes overload.\n");
            exit(199);
        }
        res = regexec(regExp, p, 1, pmatch, 0);
        if (res) break;

        // Put matched result into lexeme_List
        int begin = (int)pmatch[0].rm_so;
        int end = (int)pmatch[0].rm_eo;
        int beginGlobal = begin + offset;
        int endGlobal = end + offset - 1;
        #ifdef DEBUG
        // printf("analyse_lexical: \n%.*s (%d, %d)\n", end - begin, p + begin, beginGlobal, endGlobal);
        #endif // DEBUG
        while (textToAnalyse[beginGlobal] == '\n') beginGlobal++;
        // Now check current lexeme type, discard the result if it is a comment
        if (textToAnalyse[endGlobal] == ')' && ( (textToAnalyse[beginGlobal] == '(' && ((beginGlobal > 0) ? (textToAnalyse[beginGlobal-1] == '\n' || textToAnalyse[beginGlobal-1] == ' ') : 1) && textToAnalyse[beginGlobal+1] == ' ') || ( (textToAnalyse[beginGlobal] == ' ' || textToAnalyse[beginGlobal] == '\t') && textToAnalyse[beginGlobal+1] == '(' && textToAnalyse[beginGlobal+2] == ' '))){
            // it is a multiline comment
            #ifdef DEBUG
            printf("analyse_lexical: \nMultiline comment detected.\n%.*s (%d, %d)\n", end - begin, p + begin,beginGlobal, endGlobal);
            #endif // DEBUG
        } else if ((textToAnalyse[beginGlobal] == '\\' && ((beginGlobal > 0) ? (textToAnalyse[beginGlobal-1] == '\n' || textToAnalyse[beginGlobal-1] == ' ') : 1) && textToAnalyse[beginGlobal+1] == ' ') || ((textToAnalyse[beginGlobal] == ' ' || textToAnalyse[beginGlobal] == '\t') && textToAnalyse[beginGlobal+1] == '\\' && textToAnalyse[beginGlobal+2] == ' ')) {
            // it is a single line comment
            #ifdef DEBUG
            printf("analyse_lexical: \nSingle line comment detected.\n%.*s (%d, %d)\n", end - begin, p + begin,beginGlobal, endGlobal);
            #endif // DEBUG
        } else if (textToAnalyse[endGlobal] == '\"' && ((textToAnalyse[beginGlobal] == '\"' && ((beginGlobal > 0) ? (textToAnalyse[beginGlobal-1] == '\n' || textToAnalyse[beginGlobal-1] == ' ') : 1) && textToAnalyse[beginGlobal+1] == ' ') || ((textToAnalyse[beginGlobal] == ' ' || textToAnalyse[beginGlobal] == '\t') && textToAnalyse[beginGlobal+1] == '\"' && textToAnalyse[beginGlobal+2] == ' '))) {
            // it is a chain of characters
            #ifdef DEBUG
            printf("analyse_lexical: \nString detected.\n%.*s (%d, %d)\n", end - begin, p + begin,beginGlobal, endGlobal);
            #endif // DEBUG
            
            // Register this lexeme into lexeme_List without _"_ and  "
            if (textToAnalyse[beginGlobal] == '\"') lexeme_List[t_c].begin = beginGlobal + 2;
            else lexeme_List[t_c].begin = beginGlobal + 3;
            lexeme_List[t_c].end = end + offset - 2;
            lexeme_List[t_c].type = C;

            #ifdef DEBUG
            printf("analyse_lexical: \nString registered: ");
            int i = 0;
            while (i < lexeme_List[t_c].end - lexeme_List[t_c].begin + 1) {
                printf("%c", textToAnalyse[lexeme_List[t_c].begin + i++]);
            }
            printf("\n\n");
            #endif // DEBUG

            t_c++;
        } else {
            // it is an identifier
            #ifdef DEBUG
            printf("analyse_lexical: \nIdentifier detected.\n%.*s (%d, %d)\n", end - begin, p + begin,beginGlobal, endGlobal);
            #endif // DEBUG

            // Register this lexeme into lexeme_List
            lexeme_List[t_c].begin = begin + offset;
            lexeme_List[t_c].end = end + offset - 1;
            lexeme_List[t_c].type = I;
            t_c++;
        }
        p += end;
        offset += end;
    } while (res == 0);

    return t_c;
}