#include<stdio.h>
#include<stdlib.h>
#include<err.h>
#include"myfind.h"

struct exptree *create_n(char *name , char *arg , char **exec_arg)
{
    struct exptree *n = malloc(sizeof(struct exptree));
    n->arg = arg;      // not valid for operators
    n->exec_arg = exec_arg;
    n->left = NULL;
    n->right = NULL;
    if(mycmp(name ,"-name") == 1)
    {
        n->exp = 0;
        if(n->arg[mylen(n->arg) - 1] == '/')
            warnx("file names dont contain slashes");
    }
    else if(mycmp(name ,"-type") == 1)
        n->exp = 1;
    else if(mycmp(name ,"-print") == 1)
        n->exp = 2;
    else if(mycmp(name ,"-o") == 1)
        n->exp = 3;
    else if(mycmp(name ,"-a") == 1)
        n->exp = 4;
    else if(mycmp(name ,"-exec") == 1)
        n->exp = 5;
    else
        errx(1 , "%s: invalid option" , name);
    return n;
}
struct exptree *add_l(struct exptree *n1 , struct exptree *n2)
{
    n1->left = n2;
    return n1;
}
struct exptree *add_r(struct exptree *n1 , struct exptree *n2)
{
    n1->right = n2;
    return n1;
}
void free_tree(struct exptree *t)
{
    if(t->left == NULL && t->right == NULL)
    {
        free(t->exec_arg);
        free(t);
        return;
    }
    if(t->left != NULL)
        free_tree(t->left);
    if(t->right != NULL)
        free_tree(t->right);
    free(t->exec_arg);
    free(t);
}
void print_tree(struct exptree *t)
{
    if(t == NULL)
        return;
    print_tree(t->left);
    printf(" %d " , t->exp);
    print_tree(t->right);
}
struct exptree *parse(char *argv[] , int pos , int maxpos)
{
    struct exptree *tree = NULL;
    struct exptree *node = NULL;
    char **exec_arg = NULL;
    int op = 0;
    int action = 0; 
    char *cmd;
    char *arg;
    char *operator;
    for(int i=pos; i < maxpos; i++)
    {
        if(!op)
        {
            cmd = argv[i];
            if( i + 1 == maxpos && mycmp(cmd , "-print") != 1)
                errx(1 , "%s: missing operator " , argv[i]);
            arg = argv[i+1];
            if(mycmp(argv[i] , "-print") == 1)
            {
                action = 1;
                arg = "\n";
            }   
            else if(mycmp(argv[i] , "-exec") == 1)
            {
                action = 1;
                i+=1;  // skip '-exec'
                exec_arg = parse_exec(argv , &i , maxpos);
                i--;
            }
            node = create_n(cmd , arg , exec_arg);
            if(tree == NULL) // first element
                tree = node;
            else
                tree = add_r(tree , node); // adds node to the right of tree
            if(mycmp(cmd , "-print") != 1)
                i++;
            op = 1;
            exec_arg = NULL;
        }
        else if(op)
        {
            operator = argv[i];
            if(mycmp("-a" , operator) != 1 && mycmp("-o" , operator) != 1)
            {
                operator = "-a";  // add and if nothing present
                i--;
            }
            node = create_n(operator , 0 ,exec_arg );
            tree = add_l(node , tree);
            op = 0;
        }
    }
    if(tree == NULL)
        tree = create_n("-print" , "\n" , exec_arg);
    else if(action == 0)
    {
        node = create_n("-a" , 0 , exec_arg);
        tree = add_l(node , tree);
        node = create_n("-print" , "\n" , exec_arg);
        tree = add_r(tree , node);
    }
    return tree;
}
