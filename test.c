/*
To test if implicite conversion of type pointer to int
returns the same result at execution.

Original from Cours8.pdf
2016/10/08
*/

#include <stdio.h>
#include "interpreter.c"


// static, auto return test
// int f (int x) {
//     return (x+1);
// }
//
// int main () {
//     auto int tab[3] = {1, 2, 3};
//     // static int tab[3] = {1, 2, 3};
//     return (f(tab));
// }

// addFunctionBase test
// /*
int main() {
    //int typeIn[] = {ENTIER, ENTIER}, typeOut[] = {ENTIER};
    addFunctionBase(0, "+", 2, (int[]){ENTIER, ENTIER}, 1, (int[]){ENTIER});
    return 0;
}
// */

