#define _POSIX_SOURCE 1

#include <stdio.h>
#include <limits.h>
#include <unistd.h>
#include <sys/types.h>
#include <grp.h>
#include <pwd.h>

/*
 * Print out the group number in decimal followed by
 * (groupname)
 */
void printgroup(gid_t groupid)
{
unsigned long     lt;               /* temp */
struct group *grpptr;               /* pointer to group info */

     lt = (unsigned  long)groupid;  /* make the gid a long */
     (void)printf(" %lu(",lt);      /* print it */
     grpptr = getgrgid(groupid);    /* get group structure */
     if (grpptr == NULL)            /* print question marks if
                                     * group name is unknown 
                                     */
          {
          (void)printf("??????)");
          return;
          }
     (void)printf("%s)",grpptr->gr_name);     /* print group name */
     return;
}

/*
 * Print out the user ID in decimal followed by
 * (username)
 */
void printuser(uid_t userid)
{
unsigned long     lt;            /* temp */
struct passwd *pwptr;            /* pointer to user info */

     lt = (unsigned long)userid; /* make the uid a long */
     (void)printf(" %lu(",lt);   /* print the number */
     pwptr = getpwuid(userid);   /* get the information */
     if (pwptr == NULL)          /* print question marks if
                                  * user ID is not known
                                  */
          {
          (void)printf("??????)");
          return;
          }
     (void)printf("%s)",pwptr->pw_name);
     return;
}


void printallgroups()
{
int          ngroups;            /* number of active groups */
gid_t     *grpptr;               /* pointer to the list of 
                                  * active groups
                                  */
int          i;
gid_t     gid;

#ifndef NGROUPS_MAX
#define NGROUPS_MAX 0
#endif

#if NGROUPS_MAX == 0 
     (void)printf("Supplementary group IDs are "
                         "not supported\n");
#else
     ngroups = getgroups(0,(gid_t *)NULL);     /* get the number
                                                * of supplementary group
                                                * IDs in use
                                                */
     if (ngroups == -1)
          {
          (void)perror("getgroups() failed");
          return;
          }
     if (ngroups == 0)
          {
          (void)printf("No supplementary groups are "
                                   "available\n");
          return;
          }
     grpptr = calloc(ngroups,sizeof(gid_t));
     if (getgroups(ngroups,grpptr) == -1)
          {
          (void)perror("getgroups() failed");
          return;
          }
     (void)printf("The following supplementary groups are "
                                        " available:\n");
     for (i=1; i <= ngroups; i++)
          {
          gid = *grpptr++;
          (void)printf("\t");
          printgroup(gid);
          (void)printf("\n");
          }
#endif
     return;
}

int main()
{
uid_t     uid;
gid_t     gid;
char     *login;
char     termid[L_ctermid];

     login = getlogin();
     if (login == NULL)
          {
          (void)printf("Login name is not known\n");
          }
     else
          {
          (void)printf("Login name is '%s'\n",login);
          }
     (void)ctermid(termid);
     (void)printf("Terminal pathname is '%s'\n",termid);
     uid = getuid();
     (void)printf("Real UID is");
     printuser(uid);
     uid = geteuid();
     (void)printf(" and effective UID is");
     printuser(uid);
     gid = getgid();
     (void)printf("\nReal group ID is");
     printgroup(gid);
     gid = getegid();
     (void)printf(" and effective group ID is ");
     printgroup(gid);
     (void)printf("\n");

     printallgroups();
     return(0);
}

