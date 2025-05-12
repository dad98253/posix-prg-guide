/*
 * Main function
 */

/* Feature test switches */
#define _POSIX_SOURCE 1

/* System Headers */
#include <dirent.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* Local Headers */
#include "panic.h"          /* Defines the PANIC macro */

/* Macros */
#define TRUE   1
#define FALSE  0
#define SIZE   256
/* File scope variables */
volatile sig_atomic_t intr_flag = FALSE;
                            /* Later, set to TRUE if user
                             * types Control-C
                             */

/* External variables */
extern long nbytes;         /* Number of bytes */
extern long nfiles;         /* Number of files */

/* External functions */
void print_dir_entry(struct dirent *p);
char *cwdname(void);        /* Get working directory name */
long baud(void);             /* Get terminal baud rate */


/*
 * Signal catching functions
 */

/* Interrupt key */
void intr_key(int signo)
{

    intr_flag = TRUE;        /* Set flag for main loop */
    return;
}



/*
 * Main function
 */

int main(int argc, char *argv[])
{
int     fast = FALSE;       /* Set to TRUE if terminal is
                             * 2400 baud or faster
                             */
struct sigaction sa;        /* Used to establish
                             * signal handler for
                             * interrupt key
                             */
DIR     *dirptr;            /* for readdir() */
struct dirent *entry;       /* Returned by readdir() */
char    *dirname;           /* Current working directory */
char    junk[SIZE];         /* Used to read <NL>.  Extra size
                             * allows user to type junk.
                             */
    dirname = cwdname();
    (void)printf("\nDirectory %s:\n",dirname);
    dirptr = opendir(dirname);
    if (dirptr == NULL)
        {
        (void)fprintf(stderr,"Can not read directory\n");
        perror("opendir error");
        exit(EXIT_FAILURE);
        }
    free(dirname);           /* cwdname() allocated space */ 
    if (baud() >= 2400) fast = TRUE;
/* Cause interrupt key to call intr_key() */
    sa.sa_handler = intr_key;
    if (sigemptyset(&sa.sa_mask) != 0) PANIC;
    sa.sa_flags = 0;
    if (sigaction(SIGINT,&sa,NULL) != 0) PANIC;


/*
 * Here is the main loop
 */
    while((entry = readdir(dirptr)) != NULL)
        {
        print_dir_entry(entry);
        if (intr_flag)
            {
            (void)printf("\nInterrupted after %d files"
                         " and %d bytes\n",nfiles,nbytes);
            exit(EXIT_SUCCESS);
            }
        if(fast && (div(nfiles,24).rem  == 0))
            {
            /* Terminal is over 2400 baud and we printed
             * a multiple of 24 lines. Allow the user to
             * read the screen
             */
            (void)fprintf(stderr,"Type <NL> to continue");
            (void)fgets(junk,SIZE,stdin);
            }
        }
/* End of directory */
    (void)printf("Total of %d bytes in %d files\n",
                nbytes, nfiles);
    exit(EXIT_SUCCESS);
}

