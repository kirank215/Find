#ifndef MYFIND_H
#define MYFIND_H

struct test
{
    char *name;
    int (*fun)(char* , char*);
};
enum expr
{
    Name , Type , Print , Or , And 
};
struct exptree
{
    char *arg;
    enum expr exp;
    struct exptree *left;
    struct exptree *right;
};

int mylen(char *s);
int mycmp(char *s1 , char *s2);
char *mycat(char *s1 , char *s2);
int find_dir(char *name , int n , char *exp , char *earg);
int dir_loop(char *name , int n , char *exp , char *earg);
int printd(char *n , char *nl);
int name(char *n , char *name);
int type(char *n , char *t);
struct exptree *create_n(char *name , char *arg);
struct exptree *add_l(struct exptree *n1 , struct exptree *n2);
struct exptree *add_r(struct exptree *n1 , struct exptree *n2);
void free_tree(struct exptree *t);
void print_tree(struct exptree *t);
struct exptree *parse(char *input[] , int pos , int maxpos);
#endif
