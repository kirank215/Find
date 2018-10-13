#include<stdio.h>
#include<err.h>
#include"myfind.h"
int eval(char *n , struct exptree *t)
{
    if(t == NULL)
        warnx("missing expression");
    else if(t->exp == 4)
        return (eval(n , t->left) && eval(n ,t ->right));
    else if(t->exp == 3)
        return (eval(n , t->left) || eval(n , t->right));
    else if(t->exp == 2)
        return printd(n , "\n");
    else if(t->exp == 1)
        return type(n , t->arg);        
    else if(t->exp == 0)
        return name(n , t->arg);
    return 0;
}
