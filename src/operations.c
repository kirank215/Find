#define _DEFAULT_SOURCE
#include<stdio.h>
#include<unistd.h>
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
    return 0;
}
int name(char *n , char *name)
{
    
    return fnmatch( name , n , 0); 
}

int type(char *n , char *t)
{
    struct stat buf;
    if(lstat(n , &buf) == -1)
    {
        warnx("%s: stat failed" , n);
    }
    if(S_ISLNK(buf.st_mode) && *t == 'l')
        return 0;
    else if(S_ISBLK(buf.st_mode) && *t == 'b')
        return 0;
    else if(S_ISCHR(buf.st_mode) && *t == 'c')
        return 0;
    else if(S_ISLNK(buf.st_mode) && *t == 'l')
        return 0;
    else if(S_ISDIR(buf.st_mode) && *t == 'd')
        return 0;
    else if(S_ISREG(buf.st_mode) && *t == 'f')
        return 0;
    else if(S_ISFIFO(buf.st_mode) && *t == 'p')
        return 0;
    else if(S_ISSOCK(buf.st_mode) && *t == 's')
        return 0;
    return 1;
}
