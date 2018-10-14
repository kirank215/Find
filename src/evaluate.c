#include<stdio.h>
#include<err.h>
#include"myfind.h"
int eval(char *n , char *n_real, struct exptree *t)  //n_real  
{                                                    // is without path
    if(t == NULL)
    {
        warnx("missing expression");
        return 0;
    }
    else if(t->exp == 5)
        exec(t->exec_arg , n);
    else if(t->exp == 4)
        return (eval(n , n_real , t->left) && eval(n , n_real ,t ->right));
    else if(t->exp == 3)
        return (eval(n ,n_real , t->left) || eval(n ,n_real, t->right));
    else if(t->exp == 2)
        return printd(n , "\n");
    else if(t->exp == 1)
        return type(n , t->arg);        
    else if(t->exp == 0)
        return name(n_real , t->arg);
    return -1;     // error - never goes here
}
