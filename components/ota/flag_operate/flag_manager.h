#ifndef FLAG_MANAGER_H
#define FLAG_MANAGER_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    FLAG_BOOTLOADER = 0,
    FLAG_APP,
} flag_type_e;

typedef struct
{
    int (*func_flag_read)(void *buf, int32_t len);
    int (*func_flag_write)(const void *buf, int32_t len);
} flag_op_s;

int flag_init(flag_op_s *flag);

int flag_read(flag_type_e flag_type, void *buf, int32_t len);

int flag_write(flag_type_e flag_type, const void *buf, int32_t len);

#ifdef __cplusplus
}
#endif

#endif /* FLAG_MANAGER_H */