#include <stddef.h>
#include <string.h>
#include "flag_manager.h"

static flag_op_s g_flag_op;

#define FLASH_UNIT_SIZE (256)
#define FLASH_FLAG_SIZE (512)

int flag_init(flag_op_s *flag)
{
    if (NULL == flag
        || NULL == flag->func_flag_read
        || NULL == flag->func_flag_write)
    return -1;

    g_flag_op.func_flag_read = flag->func_flag_read;
    g_flag_op.func_flag_write = flag->func_flag_write;

    return 0;
}

int flag_read(flag_type_e flag_type, void *buf, int32_t len)
{
    uint8_t flag_buf[FLASH_FLAG_SIZE];

    if (NULL == buf
        || len < 0
        || len > FLASH_UNIT_SIZE
        || g_flag_op.func_flag_read(flag_buf, FLASH_FLAG_SIZE) != 0)
        return -1;

    switch (flag_type)
    {
    case FLAG_BOOTLOADER:
        memcpy(buf, flag_buf, len);
        break;
    case FLAG_APP:
        memcpy(buf, flag_buf + FLASH_UNIT_SIZE, len);
        break;
    default:
        break;
    }

    return 0;
}

int flag_write(flag_type_e flag_type, const void *buf, int32_t len)
{
    uint8_t flag_buf[FLASH_FLAG_SIZE];

    if (NULL == buf
        || len < 0
        || len > FLASH_UNIT_SIZE
        || g_flag_op.func_flag_read(flag_buf, FLASH_FLAG_SIZE) != 0)
        return -1;

    switch (flag_type)
    {
    case FLAG_BOOTLOADER:
        memcpy(flag_buf, buf, len);
        break;
    case FLAG_APP:
        memcpy(flag_buf + FLASH_UNIT_SIZE, buf, len);
        break;
    default:
        break;
    }

    return g_flag_op.func_flag_write(flag_buf, FLASH_FLAG_SIZE);
}