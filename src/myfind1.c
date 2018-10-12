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
struct test tests[3] = 
{
    { .name = "-name" , .fun = name } ,
    { .name = "-type" , .fun = type } , 
    { .name = "-print" , .fun = printd }  
};
int dir_loop(char *name , int n , char *exp , char *earg)
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
    char *dirname= NULL;
    for (; dir != NULL ; dir = readdir(curdir))
    {
        if(mycmp(dir->d_name , ".") == 1 || mycmp(dir->d_name ,"..") == 1)
            continue;
        dirname = mycat(name , dir->d_name);
        if((n & 1) != 1 || (n & 2) == 2)
            statval = stat(dirname , &dirbuf);
        else
            statval = lstat(dirname , &dirbuf);
        for(int l = 0; l < 3; l++)
        {
            if(mycmp(exp , tests[l].name) == 1)
                tests[l].fun(dirname , earg);
        }
        //printf("%s \n" , dirname);
        if(statval == -1)
        {
            warnx("%s: cannot stat" , dirname);
            closedir(curdir);
            return 1;
        }
        if(S_ISLNK (dirbuf.st_mode))
        {
            free(dirname);
            closedir(curdir);
            return 0;
        }
        else if(S_ISDIR (dirbuf.st_mode ))
        {
            if(dir_loop(dirname , 1 , exp , earg) == 1)
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
int find_dir(char *name , int n , char *exp , char *earg)
{
    struct stat fbuf;
    if (lstat(name , &fbuf) == -1)
    {
        warnx(" cannot stat: %s " , name);
        return 1;
    } 
    for(int l = 0; l < 3; l++)
    {
        if(mycmp(exp , tests[l].name) == 1)
            tests[l].fun(name , earg);
    }
    //printf("%s\n" , name);
    if( S_ISLNK(fbuf.st_mode) && (n == 1))
        return 0;
    if(dir_loop(name , n & 5 , exp , earg) == 1)
        return 1; 
    return 0;
}
int main(int argc , char *argv[])
{   
    int returnval = 0;
    char *(name)[10];
    name[0] = ".";
    int n_index = 0;
    int option = 1;   // p is set
    char *exp = "-print";
    char *earg = "\n";
    for(int i = 1; i < argc; i++)
    {
        if(argv[i][0] == '-')
        {
            if(mylen(argv[i]) == 2)
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
                exp = argv[i];
                earg = argv[i+1];
                printf(" %s , %s " , exp , earg);
            }
        }
        else
        {
            name[n_index] = argv[i];
            n_index++;
        }
    }
    n_index = (n_index == 0) ? 1 : n_index;
    for( int i=0; i < n_index; i++)
        returnval = find_dir(name[i] , option, exp , earg);
    return returnval;
}
