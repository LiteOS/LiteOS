
#ifndef __LOS_FS_H
#define __LOS_FS_H

/* Includes ------------------------------------------------------------------*/
#include <stddef.h>
#include "stdint.h"
#include "string.h"

#define LOS_FS_READ                     0x01
#define LOS_FS_OPEN_EXISTING            0x00

#define LOS_FS_WRITE                    0x02
#define LOS_FS_CREATE_NEW               0x04
#define LOS_FS_CREATE_ALWAYS            0x08
#define LOS_FS_OPEN_ALWAYS              0x10
#define LOS_FS__WRITTEN                 0x20
#define LOS_FS__DIRTY                   0x40



/* File function return code (LOS_FRESULT) */

typedef enum {
    LOS_FS_OK = 0,                  /* (0) Succeeded */
    LOS_FS_DISK_ERR,                /* (1) A hard error occurred in the low level disk I/O layer */
    LOS_FS_INT_ERR,                 /* (2) Assertion failed */
    LOS_FS_NOT_READY,               /* (3) The physical drive cannot work */
    LOS_FS_NO_FILE,                 /* (4) Could not find the file */
    LOS_FS_NO_PATH,                 /* (5) Could not find the path */
    LOS_FS_INVALID_NAME,            /* (6) The path name format is invalid */
    LOS_FS_DENIED,                  /* (7) Access denied due to prohibited access or directory full */
    LOS_FS_EXIST,                   /* (8) Access denied due to prohibited access */
    LOS_FS_INVALID_OBJECT,          /* (9) The file/directory object is invalid */
    LOS_FS_WRITE_PROTECTED,         /* (10) The physical drive is write protected */
    LOS_FS_INVALID_DRIVE,           /* (11) The logical drive number is invalid */
    LOS_FS_NOT_ENABLED,             /* (12) The volume has no work area */
    LOS_FS_NO_FILESYSTEM,           /* (13) There is no valid FAT volume */
    LOS_FS_MKFS_ABORTED,            /* (14) The LOS_fmkfs() aborted due to any parameter error */
    LOS_FS_TIMEOUT,                 /* (15) Could not get a grant to access the volume within defined period */
    LOS_FS_LOCKED,                  /* (16) The operation is rejected according to the file sharing policy */
    LOS_FS_NOT_ENOUGH_CORE,         /* (17) LFN working buffer could not be allocated */
    LOS_FS_TOO_MANY_OPEN_FILES,     /* (18) Number of open files > _FS_SHARE */
    LOS_FS_INVALID_PARAMETER        /* (19) Given parameter is invalid */
} LOS_FRESULT;


/*---------------------------------------------------------------------------*/
/* Fs application interface                           */

int LOS_fopen (const char * path, unsigned char mode); /* Open or create a file */

LOS_FRESULT LOS_fclose (int fd); /* Close an open file object */

size_t LOS_fread (void* buffer, size_t size, size_t count,int fd); /* Read data from a file */

size_t LOS_fwrite(const void* buffer, size_t size, size_t count, int fd);   /* Write data to a file */

LOS_FRESULT LOS_fseek( int fd, long offset); /* Move file pointer of a file object */

LOS_FRESULT LOS_fsync (int fd); /* Flush cached data of a writing file */

LOS_FRESULT LOS_fmount (const char* path, unsigned char opt); /* Mount/Unmount a logical drive */

LOS_FRESULT LOS_fmkfs (const char* path, unsigned char sfd, unsigned int au);   /* Create a file system on the volume */


#endif /* __LOS_FS_H */

