#ifndef _LOS_BUILD_H
#define _LOS_BUILD_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define LOS_CB_START(module)  __##module##_start
#define LOS_CB_END(module)  __##module##_end
#define LOS_BLOCK_START(module)  __block_##module##_start
#define LOS_BLOCK_END(module)  __block_##module##_end

#define LOS_GET_SPACE_FOR_SYSTEM(module, X)  \
    . = ALIGN(8);\
    LOS_BLOCK_START(module) = .;\
    LOS_CB_START(module) = .;\
    KEEP(*(.##module)); \
    LOS_CB_END(module) = .;\
    . += (LOS_CB_END(module) - LOS_CB_START(module))*(X - 1);\
    LOS_BLOCK_END(module) = .

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */


#endif /* _LOS_BUILD_H */