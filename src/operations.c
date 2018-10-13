#define _GNU_SOURCE
#include<stdio.h>
#include <sys/wait.h>
#include <fnmatch.h>
#include <fcntl.h>
#include<sys/types.h>
#include<dirent.h>
#include<err.h>
#include<errno.h>
#include<sys/stat.h>
#include<stdlib.h>
#include"myfind.h"

int printd(char *n , char *nl)
{
    printf("%s%s" , n , nl);
    return 1;
}
int name(char *n , char *name)
{
    int val = fnmatch(name , n , FNM_EXTMATCH);
    if(val == 0)
        return 1;
    return 0;
}

int type(char *n , char *t)
{
    struct stat buf;
    if(lstat(n , &buf) == -1)
    {
        warnx("%s: stat failed" , n);
    }
    if(S_ISLNK(buf.st_mode) && *t == 'l')
        return 1;
    else if(S_ISBLK(buf.st_mode) && *t == 'b')
        return 1;
    else if(S_ISCHR(buf.st_mode) && *t == 'c')
        return 1;
    else if(S_ISLNK(buf.st_mode) && *t == 'l')
        return 1;
    else if(S_ISDIR(buf.st_mode) && *t == 'd')
        return 1;
    else if(S_ISREG(buf.st_mode) && *t == 'f')
        return 1;
    else if(S_ISFIFO(buf.st_mode) && *t == 'p')
        return 1;
    else if(S_ISSOCK(buf.st_mode) && *t == 's')
        return 1;
    return 0;
}
/*
int exec(char *arg)
{
    char *exec_arg[10];
    for(int j = 0; j<10; j++)
    {
        for(int i = 0; *(arg + i) != ' '; i++)
        {
            if(*(arg + i) == ';')
            {
                exec_arg[j] = NULL;
                break(2);
            }
            exec_arg[j][i] = *(arg + i);
        }
    }
    pid_t pid = fork();
    if(pid == -1)
        errx(1, "exec failed");
    else if(pid == 0)   // child process
        execvp(exec_arg[0] , exec_arg);
        errx(1 , "exec child failed");
    else                // father process
    {
        int status = 0;
        waitpid(pid , &status , 0);
        return 1;
    } 
    return 1;
}*/
