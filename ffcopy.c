#define _POSIX_SOURCE 1

#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "panic.h"             /* Defines the PANIC macro */

#define HUNK_MAX INT_MAX
#define MAX_PATH 2048          /* It would be nice if POSIX
                                * provided some way to determine
                                * the longest path name a user
                                * may need to type.  Since there
                                * is no way to get that number,
                                * I am picking something large.
                                */

int main()
{
char ifpath[MAX_PATH+1];       /* name of input file */
char ofpath[MAX_PATH+1];       /* name of output file */
char scanf_string[10];         /* argument string for
                                   scanf() */
struct stat ifstat;            /* result of stat() call */
char *bigbuf;                  /* pointer to buffer */
int ifdes,ofdes;               /* input/output file
                                   descriptors */
size_t hunk;                   /* number of bytes to
                                   transfer in one piece */
size_t left;                   /* number of bytes left to
                                   transfer */

     /* Build the string "%2048s" */
     (void)sprintf(scanf_string,"%%%ds",MAX_PATH);
     /* Get the input path */
     (void)printf("Input file: ");
     if (scanf(scanf_string,ifpath) != 1) PANIC;
     /* See if the file exists and how big it is */
     if (stat(ifpath,&ifstat) != 0)
          {
          (void)perror("? Can not stat file");
          exit(EXIT_FAILURE);
          }
     left = hunk = ifstat.st_size; /* left is the amount left
                                    * to copy.  Start it out
                                    * with the size of the 
                                    * whole file.
                                    */
     if (hunk > HUNK_MAX) hunk = HUNK_MAX;
     /* Get a buffer for the whole file (or 1 hunk if the file
      * is too big.
      */
     if((bigbuf = (char *)malloc(hunk)) == NULL)
          {
          (void)fprintf(stderr,
               "? File is too big for fast copy\n");
          exit(EXIT_FAILURE);
          }
     /* Open the input file */
     if ((ifdes = open(ifpath,O_RDONLY)) == -1) PANIC;
     /* Now that we have the input file open, ask for the
      * path for the output file
      */
     (void)printf("Output file: ");
     if (scanf(scanf_string,ofpath) != 1) PANIC;
     /* Open the output file */
     if ((ofdes = open(ofpath,O_WRONLY|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR))
           == -1) PANIC;
     while (left > 0)
          {
          /* Read the file in one big bite */
          if (read(ifdes,bigbuf,hunk) != hunk)
               {
               (void)fprintf(stderr,
                    "? Error reading file %s\n",ifpath);
               exit(EXIT_FAILURE);
               }
          /* Write out the copy */
          if(write(ofdes,bigbuf,hunk) != hunk)
               {
               (void)fprintf(stderr, " Error writing file %s\n",ofpath);
               exit(EXIT_FAILURE);
               }
          left -= hunk;
          if (left < hunk) hunk = left;
          }
     /* Close the files */
     if (close(ifdes) != 0) PANIC;
     if (close(ofdes) != 0) PANIC;

     /* Print out a status message */
     (void)printf("%s copied to %s (%d bytes)\n",
          ifpath,ofpath,ifstat.st_size);
     return(0);
}

