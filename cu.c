#define _POSIX_SOURCE 1

#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "panic.h"                /* Defines the PANIC macro */

#define BUFFSIZE 256

static int chan = -1;             /* I/O Descriptor for the
                                   * terminal port.
                                   */


/*
 * Setup the communications port
 */
void comm_init(void)
{
struct termios t;


     chan = open("/dev/tty01", O_RDWR|O_NOCTTY);
     if (chan == -1) PANIC;
     if (tcgetattr(chan, &t) != 0) PANIC;
     t.c_cc[VMIN] = 32;            /* Wake up after 32
                                    * characters arrive.
                                    */
     t.c_cc[VTIME] = 1;            /* Wake up 0.1 seconds
                                    * after the first char
                                    * arrives.
                                    */
                                   /* The combination of 
                                    * VMIN/VTIME will cause
                                    * the program to wake up
                                    * 0.1 seconds after the
                                    * first character arrives
                                    * or after 32 characters 
                                    * arrive whichever comes
                                    * first.
                                    */
     t.c_iflag &= ~(BRKINT         /* Ignore break       */
      | IGNPAR | PARMRK |          /* Ignore parity      */
          INPCK |                  /* Ignore parity      */
          ISTRIP |                 /* Don't mask         */
     INLCR | IGNCR | ICRNL         /* No <cr> or <lf>    */
      | IXON);                     /* Ignore STOP char   */
     t.c_iflag |= IGNBRK | IXOFF;  /* Ignore BREAK
                                    * send XON and XOFF for
                                    * flow control.
                                    */
     t.c_oflag &= ~(OPOST);        /* No output flags     */
     t.c_lflag &= ~(               /* No local flags.  In */
          ECHO|ECHOE|ECHOK|ECHONL| /* particular, no echo */
          ICANON |                 /* no canonical input  */
                                   /* processing,         */
          ISIG |                   /* no signals,         */
          NOFLSH |                 /* no queue flush,     */
          TOSTOP);                 /* and no job control.
                                    */
     t.c_cflag &= (                /* Clear out old bits  */
          CSIZE |                  /* Character size      */
          CSTOPB |                 /* Two stop bits       */
          HUPCL |                  /* Hangup on last close*/
          PARENB);                 /* Parity              */
     t.c_cflag |= CLOCAL | CREAD | CS8;
                                   /* CLOCAL => No modem
                                    * CREAD  => Enable
                                    *           receiver
                                    * CS8    => 8-bit data
                                    */

/* Copy input and output speeds into
 * struct termios t
 */
     if (cfsetispeed(&t, B9600) == -1) PANIC;
     if (cfsetospeed(&t, B9600) == -1) PANIC;

/* Throw away any input data (noise) */
     if (tcflush(chan, TCIFLUSH) == -1) PANIC;

/* Now, set the termial port attributes */
     if (tcsetattr(chan,TCSANOW, &t) == -1) PANIC;

     return;
}


/*
 * Here is the receive process.  The call to
 * listen() never returns.
 */
void listen(void)
{
char buf[BUFFSIZE];
int  count;
int  i;

     while(1)                       /* Loop forever */
          {
          count = read(chan, &buf, BUFFSIZE);
          if (count < 0) PANIC;
          (void)write(STDOUT_FILENO,&buf,count);
          }
}

/*
 * Here is the main() function
 */
int main(void)
{
struct termios t;
char           ch;

     comm_init();        /* Fire up the comm port */

     if (tcgetattr(STDIN_FILENO,&t) != 0) PANIC;
                         /* Read the current terminal
                          * parameters into t.
                          */

     t.c_lflag &= ~(ICANON | ECHO);
                         /* Turn off the flags for
                          * echo and canonical
                          * input processing.
                          */

     if (fork() == 0) listen();
                         /* Call listen() as a 
                          * new process.
                          */


     while (1)           /* Loop forever */
          {
          (void)read(STDIN_FILENO,&ch,1);
          if (write(chan,&ch,1) != 1) PANIC;
                         /* Copy standard input
                          * to the comm port.
                          */
          }
}

