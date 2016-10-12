//
//  analex.c
//  Compiler Lac
//
//  Created by Nicolas YING on 19/09/2016.
//  Copyright © 2016 Nicolas YING. All rights reserved.
//

#include "analex.h"
#include <stdio.h>
#include <regex.h>
#include <stdlib.h>
#include <string.h>

int treatment (int debut, int fin, char* p, lexeme_t* lexeme) {
    int inner;
    
    if (p[debut] == ' ' || p[debut] == '\t' || p[debut] == '\n') {
        
        inner = treatment(debut, fin - 1, p + 1, lexeme); // jump one char ahead
        
    } else if ((p[debut] == '\\' || p[debut] == '(' ) && (p[debut + 1] == '\n' || p[debut + 1] == ' ' ) ) {
        
        //treatment_comment
        printf("---analex\\treatment---\nComment detected.\n");
        inner = 1;
        
    } else if (p[debut] == '"') {
        
        //treatment_char_chain
        lexeme->type = C;
        lexeme->lex = (char*) malloc((fin - debut + 1) * sizeof(char));
        strncpy(lexeme->lex, p + debut, fin - debut);
        (lexeme->lex)[fin - debut] = '\0';
        //printf("---analex\\treatment---\nChar_chain detected:\n%s\n", (lexeme->lex));
        inner = 0;
        
    } else {
        
        //treatment_identificateur
        lexeme->type = I;
        lexeme->lex = (char*) malloc((fin - debut + 1) * sizeof(char));
        strncpy(lexeme->lex, p + debut, fin - debut);
        (lexeme->lex)[fin - debut] = '\0';
        //printf("---analex\\treatment---\nIdentificateur detected:\n%s\n", (lexeme->lex));
        inner = 0;
        
    }
    
    return inner;
}


int analex(char * wholetext, lexeme_t * lexemes) {
    
    //const char expr[] = "";
    const char expr[] = "((^| )\" ([^()\"]*[\\].*|[^()\\]*[\\(][^()\\]*[\\)][^()\\]*)\")|((^|[ \t\n])[\\]( |\t)[^\n]*($|\n))|((^|[ \t\n])[(] [^)]*[)])|((^|[ \t\n])[\"] [^\"]*[\"])|([-+/:;.\\=\\*0-9a-zA-Z[:punct:]]+)"; // ^ et \n.
    
    // ordre:
    // char chaine containing " and \, "(^|[ \t\n])[\"]((^|[ \t\n])[\\] [^()\n]*($|\n))*((^|[ \t\n])[(] [^)]*[)])*[\"]"
    // single line comment, "(^|[ \t\n])[\\] [^()\n]*($|\n)"
    // multiline comment, "(^|[ \t\n])[(] [^)]*[)]"
    // char chaine, "(^|[ \t\n])[\"] [^\"]*[\"]"
    // punctuations and remainders, "[:=;+-*/.,1-9a-zA-Z]+"
    
    regex_t myregexp;
    char * p = wholetext;
    
    if (regcomp(&myregexp, expr, REG_EXTENDED)) { // REG_NEWLINE ne marche pas dans ce cas.
        printf("---analex---\nMauvaise exp rationnelle : %s\n", expr);
        regfree(&myregexp);
        return 0;
    }
    //printf("---analex---\n");
    //printf("expr is now:\n%s\n", expr);
    
    regmatch_t pmatch[3];
    int res = 0, offset = 0, t_c = 0;
    int debut, fin, is_comment;
    
    do {
        
        res = regexec(&myregexp, p, 1, pmatch, 0);
        if (res) break;
        
        debut = (int)pmatch[0].rm_so;
        fin = (int)pmatch[0].rm_eo;
        printf("---analex---\n%.*s (%d, %d)\n", fin - debut, p + debut, debut + offset, fin + offset - 1);
        //every possible lexeme is sent to the treatment analyser.
        is_comment = treatment(debut, fin, p, &lexemes[t_c]);
        
        if (!is_comment) {
            t_c++;
        }
        
        p += fin;
        offset += fin;
        
    } while (res == 0);
    
    regfree(&myregexp);
    
    return t_c;
}
