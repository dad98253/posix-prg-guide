
void display(long start,long stop)
{
FILE *fileid;
int fildes[2];
long i;
double f;
int status;

/* The first thing we do is create a pipe.  The array fildes
 * contains a file descriptor for each end of the pipe,
 * where fildes[0] is the "read" side and fildes[1] is
 * the "write" side.
 */
     if (pipe(fildes) != 0) PANIC;

/* Next we attempt to create a child using the fork()
 * function.  This has three possible returns: failure,
 * normal return to child, and normal return to the parent.
 *
 * The switch statement covers the first two cases.  Failure
 * is detected and a PANIC message is issued.  Otherwise, we
 * get things set for the child.
 */
     switch (fork())
          {
          case -1:
               PANIC;
               break;
          case 0:
/*
 * This is the child.
 * The first step here is to change the child's
 * standard input to be the pipe we just created.
 * Doing this uses an old UNIX trick.  We close
 * the existing STDIN file and then call
 * dup() to create a new descriptor.  This
 * will use the lowest available file descriptor.
 * Since we just closed STDIN, dup() will reuse it
 * and standard input will be connected to the
 * pipe.
 *
 * It is now required to close the child's side of
 * both fildes[0] and fildes[1].  The child will
 * see EOF, when all writers of the pipe close
 * their side.  If we forgot to close the side
 * inherited from the parent, the program would
 * never terminate.
 */
               if (close(STDIN_FILENO) != 0) PANIC;
               if (dup(fildes[0]) != STDIN_FILENO) PANIC;
               /* Close left over file descriptors */
               if (close(fildes[0]) != 0) PANIC;
               if (close(fildes[1]) != 0) PANIC;
/* The final step for the child is to replace
 * itself with the more program.  The execlp()
 * function does that for us.
 */
               execlp("more","more",(char *)0);
               PANIC; /* Should never return */
          }
     /*
      * This is the parent
      */

/* In the meantime, the parent will skip both cases of the
 * switch statement and hit the call to fdopen().  The
 * fdopen() function converts a file descriptor to a stream.
 * This allows the use of standard I/O functions, like
 * fprintf() to do our output.
 */
     fileid = fdopen(fildes[1],"w"); 
     if (close(fildes[0]) != 0) PANIC;
     if (fprintf(fileid,"     N          SQRT(N)\n") < 0)
          PANIC;
/* Next, we do all our computing.  The output will flow
 * through the pipe to the more program which will display
 * it.
 */
     for (i=start; i <= stop; i++)
          {
          f = (float)i;
          if (fprintf(fileid, "%10.0f   %10.6f\n",
               f,sqrt(f)) < 0) 
               {
               perror("Error writing output file");
               abort();
               }
          }

/* When we have computed all of our results, we close fileid.
 * This causes more to see EOF and exit.  Note: the fclose()
 * function will perform a close() on fildes[1] as part of
 * its work.  We do not have to (can't) close it again.
 */
     if (fclose(fileid) != 0) PANIC;

/* The last step is the wait(). This waits for more to exit.*/
     (void)wait(&status);
}

