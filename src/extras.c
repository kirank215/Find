#include<stdio.h>
#include<sys/types.h>
#include<dirent.h>
#include<err.h>
#include<errno.h>
#include<sys/stat.h>
#include<stdlib.h>
#include"myfind.h"
int mylen(char *s)
{
    int i = 1;
    while (*(s + i) != '\0')
        i++;
    return i;
}
int mycmp(char *s1 , char *s2)
{
    int i = 0;
    int j = 0;
    while(*(s1 + i) != '\0' && *(s2 + i) != '\0')
    {
        if(*(s1 + i) != *(s2 + i))
            return -1;
        i++;
        j++;
    }
    if(*(s1 + i) == '\0' && *(s2 + i) == '\0')
        return 1;
    return -1;
}
char *mycat(char *s1 , char *s2)
{
    char *new = malloc(sizeof(char)*(mylen(s1) + mylen(s2) + 1));
    int i = 0;
    int j = -1;
    for (; *(s1 + i) != '\0';i++)
        *(new + i) = *(s1 + i);
    if(*(new + i - 1) != '/')
    {
        *(new + i) = '/';
        i++;
    }
    do
    {
        j++;
        *(new + i + j) = *(s2 + j);
    }while (*(s2 + j) != '\0');
    return new;
}
