//
//  Lexical Analysis Header
//  Compiler Lac_reorg
//
//  Created by Nicolas YING on 06/12/2016.
//  Copyright © 2016 Nicolas YING. All rights reserved.
//

#ifndef ANALEX_H
#define ANALEX_H

typedef enum lexeme_Type {
    C, // chain de character
    I // identificateur
} lexeme_Type;

typedef struct lexeme_Element {
    lexeme_Type type;
    int begin, end;
} lexeme_Element;

int analyseLexical(char *, lexeme_Element *);

#endif // !ANALEX_H