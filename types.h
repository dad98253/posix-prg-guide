#ifndef _TYPES_
#define _TYPES_


#if (_  _STDC_  _ != 1) || defined(_IN_KERNEL)
/*
 * Machine specific system types
 */
typedef  struct{int r[1];}    *physadr;
typedef  unsigned short       iord_t;
typedef  int                  label_t[13];
typedef  unsigned short       pgadr_t;
typedef  char                 swck_t;
typedef  unsigned char        use_t;
#define  MAXSUSE              255

/*
 * Machine independent system parameters
 */
typedef  long                 daddr_t;
typedef  char                 *caddr_t;
typedef  unsigned char        uchar_t;
typedef  unsigned char        u_char;
typedef  unsigned short       u_short;
typedef  unsigned int         u_int;
typedef  unsigned long        u_long;
typedef  unsigned char        unchar;
typedef  unsigned int         uint;
typedef  unsigned short       ushort;
typedef  unsigned long        ulong;
typedef  ulong                ino_tl;
typedef  short                cnt_t;
typedef  long                 ubadr_t;
#endif  /* (_  _STDC_  _ != 1) || defined(_IN_KERNEL) */

#if (_  _STDC_  _ != 1) || defined(_POSIX_SOURCE)  || defined(_SYSV_SOURCE)
typedef  unsigned long        clock_t;
typedef  unsigned long        dev_t;
typedef  unsigned long        gid_t;
typedef  unsigned long        ino_t;
typedef  unsigned long        mode_t;
typedef  unsigned long        nlink_t;
typedef  long                 off_t;
typedef  long                 pid_t;
typedef  unsigned long        size_t;
typedef  long                 ssize_t;
typedef  unsigned long        uid_t;
#endif /* (_  _STDC_  _ != 1) || defined(_POSIX_SOURCE) */

#if (_  _STDC_  _ != 1) || defined(_SYSV_SOURCE)
typedef  unsigned char        uchar_t;
typedef  unsigned short       ushort_t;
typedef  unsigned int         uint_t;
typedef  unsigned long        ulong_t;

typedef  char *               addr_t;
typedef  char *               caddr_t;
typedef  long                 daddr_t;
typedef  short                cnt_t;
typedef  ulong_t              paddr_t;
typedef  short                sysid_t;
typedef  short                index_t;
typedef  short                lock_t;
typedef  long                 id_t;
typedef  short                o_dev_t;
typedef  unsigned short       o_gid_t;
typedef  unsigned short       o_ino_t;
typedef  unsigned short       o_mode_t;
typedef  short                o_nlink_t;
typedef  short                o_pid_t;
typedef  unsigned short       o_uid_t;
typedef  unsigned char        uchar_t;
typedef  unsigned char        u_char;
typedef  unsigned short       u_short;
typedef  unsigned int         u_int;
typedef  unsigned long        u_long;
typedef  unsigned char        unchar;
typedef  unsigned int         uint;
typedef  unsigned short       ushort;
typedef  unsigned long        ulong;
#endif  /* (_  _STDC_  _ != 1) || defined(_SYSV_SOURCE) */
#endif  /* _TYPES_ */

