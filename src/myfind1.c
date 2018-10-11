#define _DEFAULT_SOURCE
#include<stdio.h>
#include<unistd.h>
#include <fcntl.h>
#include<sys/types.h>
#include<dirent.h>
#include<err.h>
#include<errno.h>
#include<sys/stat.h>
#include<stdlib.h>
#include"myfind.h"
int print_dir_loop(char *name , int n)
{
    DIR *curdir = opendir(name);
    int statval;
    if(!curdir)
    {
        warnx(" cannot  open %s: bad directory name" , name);
        return 1;
    }
    struct dirent *dir = readdir(curdir);
    struct stat dirbuf;
    char *dirname;
    for (; dir != NULL ; dir = readdir(curdir))
    {
        if(mycmp(dir->d_name , ".") == 1 || mycmp(dir->d_name ,"..") == 1)
            continue;
        dirname = mycat(name , dir->d_name);
        if((n & 1) != 1 || (n & 2) == 2)
            statval = stat(dirname , &dirbuf);
        else
            statval = lstat(dirname , &dirbuf);
        printf("%s \n" , dirname);
        if(statval == -1)
        {
            warnx(" cannot stat: %d " , errno);
            closedir(curdir);
            return 1;
        }
        if(S_ISLNK (dirbuf.st_mode))
        {
            return 0;
        }
        else if(S_ISDIR (dirbuf.st_mode ))
        {
            if(print_dir_loop(dirname , 1) == 1)
            {
                closedir(curdir);
                return 1;
            }
        }
        free(dirname);
    }
    if(errno == EBADF)
    {
        warnx(" cannot read dir: bad directory name");
        closedir(curdir);
        return 1;
    }
    closedir(curdir);
    return 0;
}
int print_dir(char *name , int n)
{
    DIR *firstdir = opendir(name);
    struct stat fbuf;
    if(!firstdir)
    {
        warnx(" cannot  open %s: bad directory name" , name);
        return 1;
    }
    if (lstat(name , &fbuf) == -1)
    {
        warnx(" cannot stat: %d " , errno);
        closedir(firstdir);
        return 1;
    } 
    printf("%s\n" , name);
    if( S_ISLNK(fbuf.st_mode) && (n == 1))
        return 0;
    if(print_dir_loop(name , n & 5) == 1)
        return 1; 
    //closedir(firstdir);
    return 0;
}
int main(int argc , char *argv[])
{   
    int returnval = 0;
    char *(name)[10];
    name[0] = ".";
    int n_index = 0;
    int option = 1;   // p is set
    for(int i = 1; i < argc; i++)
    {
        if(argv[i][0] == '-')
        {
            if(argv[i][1] == 'P')
                option = option | 1;
            else if(argv[i][1] == 'L')
                option = option & 6;
            else if(argv[i][1] == 'H')
                option = option | 2;
            else if(argv[i][1] == 'd')
                option = option | 4;
        }
        else
        {
            name[n_index] = argv[i];
            n_index++;
        }
    }
    /* if(argc == 1)
       name = ".";
       else
       {
       name = argv[1];
       }*/
    n_index = (n_index == 0) ? 1 : n_index;
    for( int i=0; i < n_index; i++)
        returnval = print_dir(name[i] , option);
    return returnval;
}
