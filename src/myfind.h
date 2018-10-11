#ifndef MYFIND_H
#define MYFIND_H

struct test
{
    char *name;
    int (*fun)(char* , char*);
};

int mylen(char *s);
int mycmp(char *s1 , char *s2);
char *mycat(char *s1 , char *s2);
int find_dir(char *name , int n , char *exp , char *earg);
int dir_loop(char *name , int n , char *exp , char *earg);
int printd(char *n , char *nl);
int name(char *n , char *name);
int type(char *n , char *t);

#endif
