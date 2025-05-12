/*
 * Functions for program to print file names and sizes
 */
/* Feature test switches */
#define _POSIX_SOURCE 1

/* System Headers */
#include <assert.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>

/* Local Headers */
#include "panic.h"  /* Defines the PANIC macro.
                     * PANIC prints an error
                     * message when a library
                     * function fails
                     */

/* Macros */
#define TRUE     1
#define FALSE    0
#define SIZE     256 /* Arbitrary size */
/* File scope variables */
long nbytes = 0;     /* Number of bytes */
long nfiles = 0;     /* Number of files */

/* External Variables */
     /* NONE */

/* External Functions */
     /* NONE */

/* Structures and Unions */
      /* NONE */

/* Signal Catching Functions */
     /* NONE */






/*
 * Function to process one directory entry
 */
void print_dir_entry(struct dirent *p)
{
/* Prints the file size in bytes followed by the
 * file name.  If the stat() function fails,
 * print question marks.  For special files, which
 * may not have a valid size, print special.
 */
struct stat statbuf;
    if(stat(p->d_name,&statbuf) != 0)
        (void)printf("??????? ");
    else
        {
        if (S_ISREG(statbuf.st_mode))
            {
            (void)printf("%7ld ",(long)statbuf.st_size);
            nbytes += statbuf.st_size;
            }
        else
            (void)printf("Special ");
        }
    (void)printf("%s\n",p->d_name);
    nfiles++;
    return;
}
/*
 * Function to return a pointer to the name
 *  of the current working directory
 */
char *cwdname(void)
{
int     size = SIZE;
char    *ptr;
    while(TRUE)
    {
    ptr = (char *)malloc(size);
    if (ptr == NULL) PANIC;     /* Give up if we run out
                                 * of memory
                                 */
    if (getcwd(ptr,size-1) != NULL) return(ptr);
    if (errno != ERANGE) PANIC; /* Any error other than a
                                 * path name too long for the
                                 * buffer is bad news.
                                 */
    free(ptr);                  /* Return the storage */
    size += SIZE;               /* Try again with a bigger buffer */
    }
}


/*
 * Function to return speed of terminal in baud
 */
long baud(void)
{
struct termios t;
speed_t        baud_code;
    if(tcgetattr(fileno(stdout),&t) == -1)
        {
        /* If standard output is not a terminal
         * return 0.  Any other error is bad news
         */
        if (errno == ENOTTY) return(0);
        PANIC;
        }
    baud_code = cfgetospeed(&t);

/*
 * We must decode the baud rate by hand because the Bxxxx
 * symbols might not be in order.
 */
    switch(baud_code)
        {
    case B0:
        return(0);
    case B50:
        return(50);
    case B75:
        return(75);
    case B110:
        return(110);
    case B134:
        return(134);
    case B150:
        return(150);
    case B200:
        return(200);
    case B300:
        return(300);
    case B600:
        return(600);
    case B1200:
        return(1200);
    case B1800:
        return(1800);
    case B2400:
        return(2400);
    case B4800:
        return(4800);
    case B9600:
        return(9600);
    case B19200:
        return(19200);
    case B38400:
        return(38400);
    default:
        (void)fprintf(stderr,
                   "WARNING: Unknown terminal speed\n");
        return(0);
        }
}

