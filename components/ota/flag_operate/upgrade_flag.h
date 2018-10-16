#ifndef UPGRADE_FLAG_H
#define UPGRADE_FLAG_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    UPGRADE_DIFF = 0,
    UPGRADE_FULL,
} upgrade_type_e;

typedef enum
{
    OTA_IDLE = 0,
    OTA_NEED_UPGRADE,
    OTA_SUCCEED,
    OTA_FAILED,
} upgrade_state_e;

int flag_upgrade_init(void);

int flag_set_info(upgrade_type_e upgrade_type, uint32_t image_size);

void flag_get_info(upgrade_type_e *upgrade_type, uint32_t *image_size,
                   uint32_t *old_image_size, upgrade_state_e *upgrade_state);

int flag_upgrade_set_result(upgrade_state_e state);

int flag_upgrade_get_result(upgrade_state_e *state);

int flag_set_recover_verify(uint32_t recover_verify);

uint32_t flag_get_recover_verify(void);

int flag_enable_hwpatch(const uint8_t *patch_head, int32_t patch_len);

#ifdef __cplusplus
}
#endif

#endif /* UPGRADE_FLAG_H */