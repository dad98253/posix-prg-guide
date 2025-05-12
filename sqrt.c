#define _POSIX_SOURCE 1
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define BUFF_MAX 10
#define MAX_NAME 255

#define PANIC (panic(__FILE__,__LINE__))
extern void panic();


void compute_square_root(FILE *fileid,long start,long stop)
{
long i;
double f;

     if (fprintf(fileid,"     N          SQRT(N)\n") < 0)
          PANIC;
     for (i=start; i <= stop; i++)
          {
          f = (float)i;
          if (fprintf(fileid, "%10.0f   %10.6f\n",
               f,sqrt(f)) < 0) 
               {
               perror("Error writing output file");
               exit(EXIT_FAILURE);
               }
          }
}

     
long get_long(char *prompt)
{
long value;
char line[BUFF_MAX];

     while (1) 
          {
          (void)printf("%s: ",prompt);
          if (fgets(line, BUFF_MAX, stdin) == NULL)
                    exit(EXIT_FAILURE);
          if (sscanf(line,"%ld",&value) == 1) return(value);
          (void)printf("Please input an integer\n");
          }
}

main()
{
FILE     *outfile;          /* The output file */
char     filename[MAX_NAME+1];/* Name of the output file */
long     from,to;          /* The limits for the output table */

     (void)printf("What is the name of the output file: ");
     (void)fgets(filename,MAX_NAME+1,stdin);
     filename[strlen(filename) - 1]= '\0';
     outfile = fopen(filename,"w");
     if (outfile == NULL)
          {
          perror("Cannot open output file");
          exit(EXIT_FAILURE);
          }

     from = get_long("Starting value");
     to = get_long("Ending value");

     compute_square_root(outfile, from, to);

     if (fclose(outfile) != 0)
          perror("Error on close");
     return(EXIT_SUCCESS);
}

