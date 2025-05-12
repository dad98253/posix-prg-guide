/*
 * Include all of the required headers
 */
#define _POSIX_SOURCE 1
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include "panic.h"          /* Defines the PANIC macro */
                            /* See Page 58 for a description of PANIC */

#define MAX_PATH     256
/*
 * This function will process 1 directory. It is called with
 * two arguments:
 *  indent -  - The number of columns to indent this directory
 *  name -  - The file name of the directory to process.  This
 *          is most often a relative directory
 *
 * The onedir function calls itself to process nested
 * directories
 */
void onedir(short indent, char *name)
{
DIR     *current_directory;     /* pointer for readdir */
struct dirent *this_entry;      /* current directory entry */
struct stat status;             /* for the stat() function */
char cwd[MAX_PATH+1];           /* save current working
                                 * directory
                                 */
int i;                          /* temp */

     /*
      * Print out the name of the current directory with
      * leading spaces.
      */
     for (i=1; i <= indent; i++) (void)printf(" ");
     (void)printf("%s\n",name);
     
     /* Now open the directory for reading */
     current_directory = opendir(name);
     if (current_directory == NULL)
          {
          (void)perror("Can not open directory");
          return;
          }
     /* Remember the current working directory and connect to
      * new directory.  We will then be able to stat() the 
      * files without building a prefix.
      */
     if (getcwd(cwd,MAX_PATH+1) == NULL) PANIC;
     if (chdir(name) != 0) PANIC;
     
     /* Now, look at every entry in the directory */
     while ((this_entry = readdir(current_directory))
                     != NULL)
          {
          /* Ignore "." and ".." or we will get confused */
          if ((strcmp(this_entry->d_name,".") != 0) &&
               (strcmp(this_entry->d_name,"..") != 0))
               {
               if (stat(this_entry->d_name,&status) != 0) 
                         PANIC;
               /* Ignore anything that is not a directory */
               if (S_ISDIR(status.st_mode))
                    {
                    /* If this is a nested directory,
                     * process it */
                    onedir(indent+2,this_entry->d_name);
                    }
               }
          }
     /* All done.  Close the directory */
     if (closedir(current_directory) != 0) PANIC;
     /* change back to the "previous" directory */
     if (chdir(cwd) != 0) PANIC;
     return;
}

int main()
{
char root[MAX_PATH+1];          /* array to store the pathname of
                                 * the starting directory
                                 */
char scanf_string[20];          /* used to hold a format string
                                 * for scanf
                                 */
struct stat root_status;        /* stat structure for starting 
                                 * directory
                                 */

     /* Build a format string for scanf that looks like
      * %<MAX_PATH>s.
      */
     (void)sprintf(scanf_string,"%%%ds",MAX_PATH);
     (void)printf("Starting directory: ");
     /* Read the name of the starting directory which
      * may be up to MAX_PATH bytes long
      */
     (void)scanf(scanf_string,root);

     /* Verify that it is an existing directory file */
     if (stat(root,&root_status) != 0)
          {
          (void)perror("Can not access starting directory");
          (void)exit(EXIT_FAILURE);
          }
     if (S_ISDIR(root_status.st_mode) == 0)
          {
          (void)fprintf(stderr,"%s is not a directory\n",root);
          (void)exit(EXIT_FAILURE);
          }

     /* If all is well, list the directory */
     onedir(0,root);
     return(0);
}

