//
//  analex.h
//  Compiler Lac
//
//  Created by Nicolas YING on 19/09/2016.
//  Copyright © 2016 Nicolas YING. All rights reserved.
//

#ifndef Header_h
#define Header_h

typedef enum lex_Type {
    C,
    I
} lex_Type;

typedef struct lexeme {
    lex_Type type;
    char * lex;
} lexeme_t;

int analex(char *, lexeme_t *);

#endif /* Header_h */
