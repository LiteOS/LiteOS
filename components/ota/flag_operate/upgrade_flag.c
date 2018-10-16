#include <stddef.h>
#include "upgrade_flag.h"
#include "flag_manager.h"
#include "ota_crc.h"

typedef struct
{
    upgrade_type_e upgrade_type;
    upgrade_state_e upgrade_state;
    uint32_t image_size;
    uint32_t old_image_size;
    uint32_t recover_verify;
    uint32_t crc_flag;
} upgrade_flag_s;

static upgrade_flag_s g_flag;

static int save_flag(void)
{
    g_flag.crc_flag = calc_crc32(0, &g_flag, sizeof(upgrade_flag_s) - sizeof(uint32_t));

    return flag_write(FLAG_BOOTLOADER, &g_flag, sizeof(upgrade_flag_s));
}

int flag_upgrade_init(void)
{
    int ret;
    uint32_t crc;

    ret = flag_read(FLAG_BOOTLOADER, &g_flag, sizeof(upgrade_flag_s));
    if (ret != 0) return ret;

    crc = calc_crc32(0, &g_flag, sizeof(upgrade_flag_s) - sizeof(uint32_t));
    if (crc != g_flag.crc_flag)
    {
        g_flag.upgrade_state = OTA_IDLE;
        g_flag.recover_verify = 0;
        g_flag.image_size = 0;
        g_flag.old_image_size = 0;
        return save_flag();
    }

    return 0;
}

int flag_set_info(upgrade_type_e upgrade_type, uint32_t image_size)
{
    g_flag.upgrade_type = upgrade_type;
    g_flag.image_size = image_size;
    g_flag.upgrade_state = OTA_NEED_UPGRADE;

    return save_flag();
}

void flag_get_info(upgrade_type_e *upgrade_type, uint32_t *image_size,
                   uint32_t *old_image_size, upgrade_state_e *upgrade_state)
{
    if (NULL != upgrade_type)
        *upgrade_type = g_flag.upgrade_type;
    if (NULL != image_size)
        *image_size = g_flag.image_size;
    if (NULL != old_image_size)
        *old_image_size = g_flag.old_image_size;
    if (NULL != upgrade_state)
        *upgrade_state = g_flag.upgrade_state;
}

int flag_upgrade_set_result(upgrade_state_e state)
{
    g_flag.upgrade_state = state;

    return save_flag();
}

int flag_upgrade_get_result(upgrade_state_e *state)
{
    if (NULL != state)
        *state = g_flag.upgrade_state;
    g_flag.upgrade_state = OTA_IDLE;
    g_flag.old_image_size = g_flag.image_size;

    return save_flag();
}

int flag_set_recover_verify(uint32_t recover_verify)
{
    g_flag.recover_verify = recover_verify;

    return save_flag();
}

uint32_t flag_get_recover_verify(void)
{
    return g_flag.recover_verify;
}