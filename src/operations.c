#define _GNU_SOURCE
#include <unistd.h>
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
char **parse_exec(char *argv[] , int *pos , int maxpos)
{
    char **exe_arg = malloc(sizeof(char) * 550);
    int j = 0;
    for(; *pos < maxpos ; (*pos)+=1)
    {
        if(argv[*pos][0] == ';')
        {
            exe_arg[j] = NULL;
            break;   
        }
        else
            exe_arg[j] = argv[*pos];
        j++;
    }
    if(exe_arg[j] != NULL)
    {
        free(exe_arg);
        errx(1 , "exec : missing ;");
    }
    return exe_arg;
}
int exec(char **arg , char *name)
{
    pid_t pid = fork();
    if(pid == -1)
        errx(1, "exec failed");
    else if(pid == 0)   // child process
    {
        for(int i = 0; arg[i] != NULL ; i++)
        {
            if(arg[i][0] == '{' && arg[i][1] == '}')
            {
                arg[i] = malloc(sizeof(char) * 255);
                arg[i] = name;
            }
        }
        execvp(arg[0] , arg);
        errx(1 , "exec child failed");
    }
    else                // father process
    {
        int status = 0;
        waitpid(pid , &status , 0);
        return 1;
    } 
    return 1;
} 
