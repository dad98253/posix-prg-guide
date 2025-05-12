/* Define _POSIX_SOURCE to indicate
 * that this is a POSIX program
 */
#define _POSIX_SOURCE 1

/* System Headers */
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

main(argc,argv)
int argc;          /* Argument count -  - unused */
char **argv;       /* Argument list -  - unused */
{
struct tm *tmptr;  /* Pointer to date and time 
                    * broken down by component.
                    * The only member used is 
                    * tm_dst
                    */
time_t timer;      /* Number of seconds since 
                    * January 1, 1970.
                    */

    timer = time(NULL);         /* Get current time */
    tmptr = localtime(&timer);  /* Break it down */
    printf("The current time is:\n%s",
                       ctime(&timer));
if (tmptr -> tm_isdst)          /* tm_isdst is non-zero
                                 * if daylight savings
                                 * is in effect
                                 */
    printf("Daylight savings time\n");
else
    printf("Standard time\n");
exit(EXIT_SUCCESS);             /* Return to system */
}

