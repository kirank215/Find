#include<stdio.h>
#include<sys/types.h>
#include<dirent.h>
#include<err.h>
#include<errno.h>
#include<sys/stat.h>
#include<stdlib.h>
#include"myfind.h"
int print_dir_loop(char *name)
{
    DIR *curdir = opendir(name);
    struct dirent *dir = readdir(curdir);
    struct stat dirbuf;
    char *dirname;
    for (; dir != NULL ; dir = readdir(curdir))
    {
        if(mycmp(dir->d_name , ".") == 1 || mycmp(dir->d_name ,"..") == 1)
            continue;
        dirname = mycat(name , dir->d_name);
        printf("%s \n" , dirname);
        if(stat(dirname , &dirbuf) == -1)
        {
            warnx(" cannot stat: %d " , errno);
            closedir(curdir);
            return 1;
        }
        if(S_ISDIR (dirbuf.st_mode ))
        {
            if(print_dir_loop(dirname) == 1)
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
int print_dir(char *name)
{
    DIR *firstdir = opendir(name);
    if(!firstdir)
    {
        warnx(" cannot  open %s: bad directory name" , name);
        return 1;
    }
    struct dirent *dir = readdir(firstdir);
    printf("%s\n" , name);
    if(print_dir_loop(name) == 1)
        return 1; 
    closedir(firstdir);
    return 0;
}
int main(int argc , char *argv[])
{   
    int returnval = 0;
    char *name;
    if(argc == 1)
        name = ".";
    else
    {
        name = argv[1];
    }
    returnval = print_dir(name);
    printf("%s" , mycat("ab" , "cd"));
    return returnval;
}
