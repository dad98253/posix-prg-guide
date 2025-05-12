#define _POSIX_SOURCE 1

#include <termios.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int getpswd(char *buff,unsigned size)
{
struct termios attr;        /* Used for getting and setting
                             * terminal attributes.
                             */

int               n;        /* Number of bytes read */

     (void)printf("Password: "); /* Issue the prompt */
     (void)fflush(stdout);       /* Cause the data to be written out
                                  * to the terminal
                                  */

     /*
      * Now turn off echo.
      */
     
     if(tcgetattr(STDIN_FILENO,&attr) != 0) return(-1);
                            /* Start by getting current
                             * attributes.  This call
                             * copies all of the terminal
                             * parameters into attr.
                             */

     attr.c_lflag &= ~(ECHO);
                            /* Turn off echo flag.
                             * NOTE: We are careful not to
                             * modify any bits except ECHO.
                             */

     if(tcsetattr(STDIN_FILENO,TCSAFLUSH,&attr) != 0)
                    return(-1);
                            /* Wait for all of the data
                             * to be printed.
                             */
                            /* Set all of the terminal
                             * parameters from the (slightly)
                             * modified struct termios.
                             */
                            /* Discard any characters that
                             * have been typed but
                             * not yet read.
                             */

     n = read(STDIN_FILENO,buff,size);
                            /* Read a line from the
                             * terminal.
                             */

     /*
      * Turn echo back on.
      */
     attr.c_lflag |= ECHO;
     if(tcsetattr(STDIN_FILENO,TCSANOW,&attr) != 0)
                    return(-1);

     return(n);
                            /*
                             * Return the number of bytes
                             * in the password
                             */
}

