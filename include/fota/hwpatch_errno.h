#ifndef HWERROR_CODE_H
#define HWERROR_CODE_H

#define ERR_OK                      0
#define ERR_ILEGAL_PARAM            -0x01
#define ERR_INTERNAL                -0x02

#define ERR_DIFF_FILE_OPEN          -0x101
#define ERR_DIFF_FILE_READ          -0x102
#define ERR_DIFF_FILE_WRITE         -0x103
#define ERR_DIFF_COMPRESS           -0x104
#define ERR_DIFF_MALLOC             -0x105
#define ERR_DIFF_BSDIFF             -0x106
#define ERR_DIFF_BUFF_NOT_ENOUGHT   -0x107
#define ERR_DIFF_GET_SETS           -0x108
#define ERR_DIFF_GEN_CMD            -0x109
#define ERR_DIFF_GEN_DEPENDENCY     -0x10A
#define ERR_DIFF_TARGET_NOT_FOUND   -0x10B

#define ERR_PATCH_WRITE_BCK         -0x201
#define ERR_PATCH_READ_BCK          -0x202
#define ERR_PATCH_WRITE_IMAGE       -0x203
#define ERR_PATCH_READ_IMAGE        -0x204
#define ERR_PATCH_READ_PATCH        -0x205
#define ERR_PATCH_MALLOC            -0x206
#define ERR_PATCH_RECOVER_BLOCK     -0x207
#define ERR_PATCH_BUFF_NOT_ENOUGHT  -0x207
#define ERR_PATCH_UNCOMPRESS        -0x208
#define ERR_PATCH_CHECKSUM          -0x209
#define ERR_PATCH_RECOVER_VERIFY    -0x20A
#define ERR_PATCH_LAST_RECOVER      -0x20B
#define ERR_PATCH_FLAG_INIT         -0x20C

#endif /* HWERROR_CODE_H */