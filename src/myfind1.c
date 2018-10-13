#define _DEFAULT_SOURCE
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<dirent.h>
#include<err.h>
#include<errno.h>
#include<sys/stat.h>
#include<stdlib.h>
#include"myfind.h"
int dir_loop(char *name , int n , struct exptree *t)
{
    int rv = 2;
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
        if(eval(dirname , t) == 1)
            rv = 0;
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
            return rv;
        }
        else if(S_ISDIR (dirbuf.st_mode ))
        {
            if(dir_loop(dirname , 1 , t) == 1)
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
    return rv;
}
int find_dir(char *name , int n , struct exptree *t)
{
    int returnv = 1;
    int valreturn;
    struct stat fbuf;
    if (lstat(name , &fbuf) == -1)
    {
        warnx(" cannot stat: %s " , name);
        return 1;
    }
    if(eval(name , t) == 1)
        returnv=0;
    if( S_ISLNK(fbuf.st_mode) && (n == 1))
        return returnv;
    valreturn = dir_loop(name , n & 5 , t);  // if 1 -> error
    if(valreturn != 2)                              // if 2->file not found
        return valreturn;
    return returnv;
}
int main(int argc , char *argv[])
{   
    int returnval = 0;
    struct exptree *tree;
    char *(name)[10];
    name[0] = ".";
    int n_index = 0;
    int option = 1;   // p is set
    for(int i = 1; i < argc; i++)
    {
        if(argv[i][0] == '-' && mylen(argv[i]) == 2)
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
        else if( argv[i][0] != '-')
        {
            name[n_index] = argv[i];
            n_index++;
        }
        else 
        {
            tree = parse(argv , i , argc);
            break;
        }
    }
    n_index = (n_index == 0) ? 1 : n_index;
    for( int i=0; i < n_index; i++)
        returnval = find_dir(name[i] , option, tree);
    return returnval;
}
