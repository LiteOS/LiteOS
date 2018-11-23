/*
 * spiffs_hydrogen.c
 *
 *  Created on: Jun 16, 2013
 *      Author: petera
 */

#include "spiffs.h"
#include "spiffs_nucleus.h"


s32_t SPIFFS_format(spiffs *fs)
{
    return 0;
}

s32_t spiffs_mount_ret = SPIFFS_OK;

s32_t SPIFFS_mount(spiffs *fs, spiffs_config *config, u8_t *work,
                   u8_t *fd_space, u32_t fd_space_size,
                   void *cache, u32_t cache_size,
                   spiffs_check_callback check_cb_f)
{
    static is_first = 1;

    if (is_first)
    {
        is_first = 0;
        return SPIFFS_ERR_NOT_A_FS;
    }
    else
    {
        return spiffs_mount_ret;
    }
}

void SPIFFS_unmount(spiffs *fs)
{
}

s32_t SPIFFS_errno(spiffs *fs)
{
    return fs->err_code;
}

spiffs_file SPIFFS_open(spiffs *fs, const char *path, spiffs_flags flags, spiffs_mode mode)
{
    return SPIFFS_OK;
}

s32_t SPIFFS_read(spiffs *fs, spiffs_file fh, void *buf, s32_t len)
{
    return 0;
}

s32_t SPIFFS_write(spiffs *fs, spiffs_file fh, void *buf, s32_t len)
{
    return 0;
}

s32_t SPIFFS_lseek(spiffs *fs, spiffs_file fh, s32_t offs, int whence)
{
    return 0;
}

int g_spiffs_errno = SPIFFS_OK;

s32_t SPIFFS_remove(spiffs *fs, const char *path)
{
    return g_spiffs_errno;
}

s32_t SPIFFS_fstat(spiffs *fs, spiffs_file fh, spiffs_stat *s)
{
    return SPIFFS_OK;
}

s32_t SPIFFS_fflush(spiffs *fs, spiffs_file fh)
{
    return SPIFFS_OK;
}

s32_t SPIFFS_close(spiffs *fs, spiffs_file fh)
{
    return SPIFFS_OK;
}

s32_t SPIFFS_rename(spiffs *fs, const char *old_path, const char *new_path)
{
    return SPIFFS_OK;
}

spiffs_DIR *SPIFFS_opendir(spiffs *fs, const char *name, spiffs_DIR *d)
{
    return d;
}

struct spiffs_dirent *SPIFFS_readdir(spiffs_DIR *d, struct spiffs_dirent *e)
{
    return d;
}

s32_t SPIFFS_closedir(spiffs_DIR *d)
{
    return 0;
}
