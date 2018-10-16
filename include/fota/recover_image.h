#ifndef RECOVER_IMAGE_H
#define RECOVER_IMAGE_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    int8_t recover_on_oldimage;
    int32_t max_old_image_size;
    int32_t max_new_image_size;
    int32_t max_patch_size;
    int32_t flash_erase_unit;
    uint32_t old_image_addr;
    uint32_t new_image_addr;
    uint32_t patch_addr;
    uint32_t flag_addr;
} recover_info_s;

typedef struct
{
    int (*func_printf)(const char *fmt, ...);
    void *(*func_malloc)(size_t size);
    void (*func_free)(void *ptr);
} recover_assist_s;

typedef enum
{
    FLASH_OLDBIN_READ = 0,
    FLASH_NEWBIN_WRITE,
    FLASH_PATCH,
    FLASH_UPDATE_INFO,
} flash_type_e;

typedef struct
{
    int (*func_flash_read)(flash_type_e flash_type, void *buf, int32_t len, uint32_t offset);
    int (*func_flash_write)(flash_type_e, const void *buf, int32_t len, uint32_t offset);
} recover_flash_s;

typedef enum
{
    RECOVER_UPGRADE_NONE = 0,
    RECOVER_UPGRADE_DIFF,
    RECOVER_UPGRADE_FULL,
} recover_upgrade_type_e;

int recover_init(recover_info_s *info, recover_assist_s *assist, recover_flash_s *flash);

int recover_image(recover_upgrade_type_e *recover_upgrade_type, uint32_t *newbin_size, uint32_t *oldbin_size);

#ifdef __cplusplus
}
#endif

#endif /* RECOVER_IMAGE_H */