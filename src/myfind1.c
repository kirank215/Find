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
int dir_loop(char *name , int n , struct exptree *t , int d)
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
        if(n != 1)
            statval = stat(dirname , &dirbuf);
        else
            statval = lstat(dirname , &dirbuf);
        if(d == 0)
            eval(dirname ,dir->d_name , t);
        if(statval == -1)
        {
            warnx("%s: cannot stat" , dirname);
            closedir(curdir);
            return 1;
        }
        if(S_ISLNK (dirbuf.st_mode))
        {
            free(dirname);
            continue;
        }
        else if(S_ISDIR (dirbuf.st_mode ))
        {
            if(dir_loop(dirname , 1 , t , d) == 1)
            {
                closedir(curdir);
                return 1;
            }
        }
        if(d == 1)
            eval(dirname , dir->d_name , t);
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
int find_dir(char *name , int n , struct exptree *t , int d)
{
    int returnv = 1;
    struct stat fbuf;
    if (lstat(name , &fbuf) == -1)
    {
        warnx(" cannot stat: %s " , name);
        return 1;
    }
    if(d == 0)
        eval(name ,name, t);
    if( S_ISLNK(fbuf.st_mode) && (n == 1))
        return returnv;
    if(n == 2)
        n = 1;
    returnv = dir_loop(name , n , t , d);  // if 1 -> error
    if(d == 1)
        eval(name , name , t);
    return returnv;
}
int main(int argc , char *argv[])
{   
    int returnval = 0;
    struct exptree *tree = NULL;
    int d;
    char *(name)[10];
    name[0] = ".";
    int n_index = 0;
    int option = 1;   // p is set
    for(int i = 1; i < argc; i++)
    {
        if(argv[i][0] == '-' && mylen(argv[i]) == 2)
        {
                if(argv[i][1] == 'P')
                    option = 1;
                else if(argv[i][1] == 'L')
                    option = 0;
                else if(argv[i][1] == 'H')
                    option = 2;
                else if(argv[i][1] == 'd')
                    d = 1;
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
    if(tree == NULL)
        tree = create_n("-print" , "\n" , NULL);
    n_index = (n_index == 0) ? 1 : n_index;
    for( int i=0; i < n_index; i++)
    {
        if(find_dir(name[i] , option, tree , d) == 1)
            returnval = 1;
    }
    free_tree(tree);
    return returnval;
}
