/*
To test if implicite conversion of type pointer to int
returns the same result at execution.

Original from Cours8.pdf
2016/10/08
*/

int f (int x) {
    return (x+1);
}

int main () {
    int tab[3] = {1, 2, 3};
    // static int tab[3] = {1, 2, 3};
    return (f(tab));
}