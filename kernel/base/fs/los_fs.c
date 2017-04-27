#include "los_fs.h"
#include "los_fs_adapter.h"


/*******************************************************************************
* Function Name  : LOS_fopen
* Description    : Open or create a file
* Input          : const char * path,
                   unsigned char mode
* Output         : None
* Return         : int fd
*******************************************************************************/
int LOS_fopen (const char * path, unsigned char mode)
{
    return los_fopen(path, mode);
}

/*******************************************************************************
* Function Name  : LOS_fclose
* Description    : Close an open file object
* Input          : int fd
* Output         : None
* Return         : LOS_FRESULT
*******************************************************************************/
LOS_FRESULT LOS_fclose (int fd)
{
    return los_fclose (fd);
}

/*******************************************************************************
* Function Name  : LOS_fread
* Description    : Read data from a file
* Input          : size_t size,
                   size_t count,
                   int fd
* Output         : void* buffer
* Return         : num of read byte
*******************************************************************************/
size_t LOS_fread (void* buffer, size_t size, size_t count,int fd)
{
    return los_fread(buffer, size, count,fd);
}

/*******************************************************************************
* Function Name  : LOS_fwrite
* Description    : Write data to a file
* Input          : const void* buffer,
                   size_t size,
                   size_t count,
                   int fd
* Output         : None
* Return         : size_t: num of write byte
*******************************************************************************/
size_t LOS_fwrite(const void* buffer, size_t size, size_t count, int fd)
{
    return los_fwrite(buffer, size, count, fd);
}

/*******************************************************************************
* Function Name  : LOS_fseek
* Description    : Move file pointer of a file object
* Input          : long offset,
                   int fd
* Output         : None
* Return         : LOS_FRESULT
*******************************************************************************/
LOS_FRESULT LOS_fseek(int fd, long offset)
{
    return los_fseek (fd, offset);
}

/*******************************************************************************
* Function Name  : LOS_fsync
* Description    : Flush cached data of a writing file
* Input          : int fd
* Output         : None
* Return         : LOS_FRESULT
*******************************************************************************/
LOS_FRESULT LOS_fsync (int fd)
{
    return los_fsync (fd);
}

/*******************************************************************************
* Function Name  : LOS_fmount
* Description    : Mount/Unmount a logical drive
* Input          : const char* path,
                   unsigned char opt
* Output         : None
* Return         : LOS_FRESULT
*******************************************************************************/
LOS_FRESULT LOS_fmount (const char* path, unsigned char opt)
{
    return los_fmount(path, opt);
}

/*******************************************************************************
* Function Name  : LOS_fmkfs
* Description    : Create a file system on the volume
* Input          : const char* path,
                   unsigned char sfd,
                   unsigned int au
* Output         : None
* Return         : LOS_FRESULT
*******************************************************************************/
LOS_FRESULT LOS_fmkfs (const char* path, unsigned char sfd, unsigned int au)
{
    return los_fmkfs(path, sfd, au);
}


