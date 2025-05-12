#ifndef _UTIME_
#define _UTIME_

struct utimbuf
    {
    time_t actime;      /* access time */
    time_t modtime;     /* modification time */
    };

#ifdef _  _STDC_  _
    int utime(const char *path,
                 const struct utimbuf *times);
#else
    extern int utime();
#endif /* _  _STDC_  _ */
#endif /* _UTIME_ */

