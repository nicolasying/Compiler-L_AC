#ifndef INTERPRET_H
#define INTERPRET_H


#define ENTIER 1
#define ANY 0
#define BOOLEAN 2
#define CHAINECHAR 3
#define FLOATANT 4

#define MAX_VM_NUMBER 20
#define MAX_SYMBOL_NUMBER 100

typedef void (*baseFonc)(void);

// definition des piles
typedef struct pileBase pileBase;
struct pileBase // la première element dans la pile seront toujours négligé
{
    int data;
    pileBase * precedent;
};

void pushStack (int data, pileBase * * pileTop);
int popStack (pileBase * * pileTop);
int typeConversion(int typ1, int typ2);
void compilateur();

#endif // !INTERPRET_H
