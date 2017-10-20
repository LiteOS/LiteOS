#ifndef __LOS_BSP_FLASH_H
#define __LOS_BSP_FLASH_H


/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
#include "diskio.h"
#include "ff.h"

typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;
//=============================================================================================
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define ADDR_FLASH_PAGE_0     ((uint32_t)0x08000000) /* Base @ of Page 0, 2 Kbytes */
#define ADDR_FLASH_PAGE_1     ((uint32_t)0x08000800) /* Base @ of Page 1, 2 Kbytes */
#define ADDR_FLASH_PAGE_2     ((uint32_t)0x08001000) /* Base @ of Page 2, 2 Kbytes */
#define ADDR_FLASH_PAGE_3     ((uint32_t)0x08001800) /* Base @ of Page 3, 2 Kbytes */
#define ADDR_FLASH_PAGE_4     ((uint32_t)0x08002000) /* Base @ of Page 4, 2 Kbytes */
#define ADDR_FLASH_PAGE_5     ((uint32_t)0x08002800) /* Base @ of Page 5, 2 Kbytes */
#define ADDR_FLASH_PAGE_6     ((uint32_t)0x08003000) /* Base @ of Page 6, 2 Kbytes */
#define ADDR_FLASH_PAGE_7     ((uint32_t)0x08003800) /* Base @ of Page 7, 2 Kbytes */
#define ADDR_FLASH_PAGE_8     ((uint32_t)0x08004000) /* Base @ of Page 8, 2 Kbytes */
#define ADDR_FLASH_PAGE_9     ((uint32_t)0x08004800) /* Base @ of Page 9, 2 Kbytes */
#define ADDR_FLASH_PAGE_10    ((uint32_t)0x08005000) /* Base @ of Page 10, 2 Kbytes */
#define ADDR_FLASH_PAGE_11    ((uint32_t)0x08005800) /* Base @ of Page 11, 2 Kbytes */
#define ADDR_FLASH_PAGE_12    ((uint32_t)0x08006000) /* Base @ of Page 12, 2 Kbytes */
#define ADDR_FLASH_PAGE_13    ((uint32_t)0x08006800) /* Base @ of Page 13, 2 Kbytes */
#define ADDR_FLASH_PAGE_14    ((uint32_t)0x08007000) /* Base @ of Page 14, 2 Kbytes */
#define ADDR_FLASH_PAGE_15    ((uint32_t)0x08007800) /* Base @ of Page 15, 2 Kbytes */
#define ADDR_FLASH_PAGE_16    ((uint32_t)0x08008000) /* Base @ of Page 16, 2 Kbytes */
#define ADDR_FLASH_PAGE_17    ((uint32_t)0x08008800) /* Base @ of Page 17, 2 Kbytes */
#define ADDR_FLASH_PAGE_18    ((uint32_t)0x08009000) /* Base @ of Page 18, 2 Kbytes */
#define ADDR_FLASH_PAGE_19    ((uint32_t)0x08009800) /* Base @ of Page 19, 2 Kbytes */
#define ADDR_FLASH_PAGE_20    ((uint32_t)0x0800A000) /* Base @ of Page 20, 2 Kbytes */
#define ADDR_FLASH_PAGE_21    ((uint32_t)0x0800A800) /* Base @ of Page 21, 2 Kbytes */
#define ADDR_FLASH_PAGE_22    ((uint32_t)0x0800B000) /* Base @ of Page 22, 2 Kbytes */
#define ADDR_FLASH_PAGE_23    ((uint32_t)0x0800B800) /* Base @ of Page 23, 2 Kbytes */
#define ADDR_FLASH_PAGE_24    ((uint32_t)0x0800C000) /* Base @ of Page 24, 2 Kbytes */
#define ADDR_FLASH_PAGE_25    ((uint32_t)0x0800C800) /* Base @ of Page 25, 2 Kbytes */
#define ADDR_FLASH_PAGE_26    ((uint32_t)0x0800D000) /* Base @ of Page 26, 2 Kbytes */
#define ADDR_FLASH_PAGE_27    ((uint32_t)0x0800D800) /* Base @ of Page 27, 2 Kbytes */
#define ADDR_FLASH_PAGE_28    ((uint32_t)0x0800E000) /* Base @ of Page 28, 2 Kbytes */
#define ADDR_FLASH_PAGE_29    ((uint32_t)0x0800E800) /* Base @ of Page 29, 2 Kbytes */
#define ADDR_FLASH_PAGE_30    ((uint32_t)0x0800F000) /* Base @ of Page 30, 2 Kbytes */
#define ADDR_FLASH_PAGE_31    ((uint32_t)0x0800F800) /* Base @ of Page 31, 2 Kbytes */
#define ADDR_FLASH_PAGE_32    ((uint32_t)0x08010000) /* Base @ of Page 32, 2 Kbytes */
#define ADDR_FLASH_PAGE_33    ((uint32_t)0x08010800) /* Base @ of Page 33, 2 Kbytes */
#define ADDR_FLASH_PAGE_34    ((uint32_t)0x08011000) /* Base @ of Page 34, 2 Kbytes */
#define ADDR_FLASH_PAGE_35    ((uint32_t)0x08011800) /* Base @ of Page 35, 2 Kbytes */
#define ADDR_FLASH_PAGE_36    ((uint32_t)0x08012000) /* Base @ of Page 36, 2 Kbytes */
#define ADDR_FLASH_PAGE_37    ((uint32_t)0x08012800) /* Base @ of Page 37, 2 Kbytes */
#define ADDR_FLASH_PAGE_38    ((uint32_t)0x08013000) /* Base @ of Page 38, 2 Kbytes */
#define ADDR_FLASH_PAGE_39    ((uint32_t)0x08013800) /* Base @ of Page 39, 2 Kbytes */
#define ADDR_FLASH_PAGE_40    ((uint32_t)0x08014000) /* Base @ of Page 40, 2 Kbytes */
#define ADDR_FLASH_PAGE_41    ((uint32_t)0x08014800) /* Base @ of Page 41, 2 Kbytes */
#define ADDR_FLASH_PAGE_42    ((uint32_t)0x08015000) /* Base @ of Page 42, 2 Kbytes */
#define ADDR_FLASH_PAGE_43    ((uint32_t)0x08015800) /* Base @ of Page 43, 2 Kbytes */
#define ADDR_FLASH_PAGE_44    ((uint32_t)0x08016000) /* Base @ of Page 44, 2 Kbytes */
#define ADDR_FLASH_PAGE_45    ((uint32_t)0x08016800) /* Base @ of Page 45, 2 Kbytes */
#define ADDR_FLASH_PAGE_46    ((uint32_t)0x08017000) /* Base @ of Page 46, 2 Kbytes */
#define ADDR_FLASH_PAGE_47    ((uint32_t)0x08017800) /* Base @ of Page 47, 2 Kbytes */
#define ADDR_FLASH_PAGE_48    ((uint32_t)0x08018000) /* Base @ of Page 48, 2 Kbytes */
#define ADDR_FLASH_PAGE_49    ((uint32_t)0x08018800) /* Base @ of Page 49, 2 Kbytes */
#define ADDR_FLASH_PAGE_50    ((uint32_t)0x08019000) /* Base @ of Page 50, 2 Kbytes */
#define ADDR_FLASH_PAGE_51    ((uint32_t)0x08019800) /* Base @ of Page 51, 2 Kbytes */
#define ADDR_FLASH_PAGE_52    ((uint32_t)0x0801A000) /* Base @ of Page 52, 2 Kbytes */
#define ADDR_FLASH_PAGE_53    ((uint32_t)0x0801A800) /* Base @ of Page 53, 2 Kbytes */
#define ADDR_FLASH_PAGE_54    ((uint32_t)0x0801B000) /* Base @ of Page 54, 2 Kbytes */
#define ADDR_FLASH_PAGE_55    ((uint32_t)0x0801B800) /* Base @ of Page 55, 2 Kbytes */
#define ADDR_FLASH_PAGE_56    ((uint32_t)0x0801C000) /* Base @ of Page 56, 2 Kbytes */
#define ADDR_FLASH_PAGE_57    ((uint32_t)0x0801C800) /* Base @ of Page 57, 2 Kbytes */
#define ADDR_FLASH_PAGE_58    ((uint32_t)0x0801D000) /* Base @ of Page 58, 2 Kbytes */
#define ADDR_FLASH_PAGE_59    ((uint32_t)0x0801D800) /* Base @ of Page 59, 2 Kbytes */
#define ADDR_FLASH_PAGE_60    ((uint32_t)0x0801E000) /* Base @ of Page 60, 2 Kbytes */
#define ADDR_FLASH_PAGE_61    ((uint32_t)0x0801E800) /* Base @ of Page 61, 2 Kbytes */
#define ADDR_FLASH_PAGE_62    ((uint32_t)0x0801F000) /* Base @ of Page 62, 2 Kbytes */
#define ADDR_FLASH_PAGE_63    ((uint32_t)0x0801F800) /* Base @ of Page 63, 2 Kbytes */
#define ADDR_FLASH_PAGE_64    ((uint32_t)0x08020000) /* Base @ of Page 64, 2 Kbytes */
#define ADDR_FLASH_PAGE_65    ((uint32_t)0x08020800) /* Base @ of Page 65, 2 Kbytes */
#define ADDR_FLASH_PAGE_66    ((uint32_t)0x08021000) /* Base @ of Page 66, 2 Kbytes */
#define ADDR_FLASH_PAGE_67    ((uint32_t)0x08021800) /* Base @ of Page 67, 2 Kbytes */
#define ADDR_FLASH_PAGE_68    ((uint32_t)0x08022000) /* Base @ of Page 68, 2 Kbytes */
#define ADDR_FLASH_PAGE_69    ((uint32_t)0x08022800) /* Base @ of Page 69, 2 Kbytes */
#define ADDR_FLASH_PAGE_70    ((uint32_t)0x08023000) /* Base @ of Page 70, 2 Kbytes */
#define ADDR_FLASH_PAGE_71    ((uint32_t)0x08023800) /* Base @ of Page 71, 2 Kbytes */
#define ADDR_FLASH_PAGE_72    ((uint32_t)0x08024000) /* Base @ of Page 72, 2 Kbytes */
#define ADDR_FLASH_PAGE_73    ((uint32_t)0x08024800) /* Base @ of Page 73, 2 Kbytes */
#define ADDR_FLASH_PAGE_74    ((uint32_t)0x08025000) /* Base @ of Page 74, 2 Kbytes */
#define ADDR_FLASH_PAGE_75    ((uint32_t)0x08025800) /* Base @ of Page 75, 2 Kbytes */
#define ADDR_FLASH_PAGE_76    ((uint32_t)0x08026000) /* Base @ of Page 76, 2 Kbytes */
#define ADDR_FLASH_PAGE_77    ((uint32_t)0x08026800) /* Base @ of Page 77, 2 Kbytes */
#define ADDR_FLASH_PAGE_78    ((uint32_t)0x08027000) /* Base @ of Page 78, 2 Kbytes */
#define ADDR_FLASH_PAGE_79    ((uint32_t)0x08027800) /* Base @ of Page 79, 2 Kbytes */
#define ADDR_FLASH_PAGE_80    ((uint32_t)0x08028000) /* Base @ of Page 80, 2 Kbytes */
#define ADDR_FLASH_PAGE_81    ((uint32_t)0x08028800) /* Base @ of Page 81, 2 Kbytes */
#define ADDR_FLASH_PAGE_82    ((uint32_t)0x08029000) /* Base @ of Page 82, 2 Kbytes */
#define ADDR_FLASH_PAGE_83    ((uint32_t)0x08029800) /* Base @ of Page 83, 2 Kbytes */
#define ADDR_FLASH_PAGE_84    ((uint32_t)0x0802A000) /* Base @ of Page 84, 2 Kbytes */
#define ADDR_FLASH_PAGE_85    ((uint32_t)0x0802A800) /* Base @ of Page 85, 2 Kbytes */
#define ADDR_FLASH_PAGE_86    ((uint32_t)0x0802B000) /* Base @ of Page 86, 2 Kbytes */
#define ADDR_FLASH_PAGE_87    ((uint32_t)0x0802B800) /* Base @ of Page 87, 2 Kbytes */
#define ADDR_FLASH_PAGE_88    ((uint32_t)0x0802C000) /* Base @ of Page 88, 2 Kbytes */
#define ADDR_FLASH_PAGE_89    ((uint32_t)0x0802C800) /* Base @ of Page 89, 2 Kbytes */
#define ADDR_FLASH_PAGE_90    ((uint32_t)0x0802D000) /* Base @ of Page 90, 2 Kbytes */
#define ADDR_FLASH_PAGE_91    ((uint32_t)0x0802D800) /* Base @ of Page 91, 2 Kbytes */
#define ADDR_FLASH_PAGE_92    ((uint32_t)0x0802E000) /* Base @ of Page 92, 2 Kbytes */
#define ADDR_FLASH_PAGE_93    ((uint32_t)0x0802E800) /* Base @ of Page 93, 2 Kbytes */
#define ADDR_FLASH_PAGE_94    ((uint32_t)0x0802F000) /* Base @ of Page 94, 2 Kbytes */
#define ADDR_FLASH_PAGE_95    ((uint32_t)0x0802F800) /* Base @ of Page 95, 2 Kbytes */
#define ADDR_FLASH_PAGE_96    ((uint32_t)0x08030000) /* Base @ of Page 96, 2 Kbytes */
#define ADDR_FLASH_PAGE_97    ((uint32_t)0x08030800) /* Base @ of Page 97, 2 Kbytes */
#define ADDR_FLASH_PAGE_98    ((uint32_t)0x08031000) /* Base @ of Page 98, 2 Kbytes */
#define ADDR_FLASH_PAGE_99    ((uint32_t)0x08031800) /* Base @ of Page 99, 2 Kbytes */
#define ADDR_FLASH_PAGE_100   ((uint32_t)0x08032000) /* Base @ of Page 100, 2 Kbytes */
#define ADDR_FLASH_PAGE_101   ((uint32_t)0x08032800) /* Base @ of Page 101, 2 Kbytes */
#define ADDR_FLASH_PAGE_102   ((uint32_t)0x08033000) /* Base @ of Page 102, 2 Kbytes */
#define ADDR_FLASH_PAGE_103   ((uint32_t)0x08033800) /* Base @ of Page 103, 2 Kbytes */
#define ADDR_FLASH_PAGE_104   ((uint32_t)0x08034000) /* Base @ of Page 104, 2 Kbytes */
#define ADDR_FLASH_PAGE_105   ((uint32_t)0x08034800) /* Base @ of Page 105, 2 Kbytes */
#define ADDR_FLASH_PAGE_106   ((uint32_t)0x08035000) /* Base @ of Page 106, 2 Kbytes */
#define ADDR_FLASH_PAGE_107   ((uint32_t)0x08035800) /* Base @ of Page 107, 2 Kbytes */
#define ADDR_FLASH_PAGE_108   ((uint32_t)0x08036000) /* Base @ of Page 108, 2 Kbytes */
#define ADDR_FLASH_PAGE_109   ((uint32_t)0x08036800) /* Base @ of Page 109, 2 Kbytes */
#define ADDR_FLASH_PAGE_110   ((uint32_t)0x08037000) /* Base @ of Page 110, 2 Kbytes */
#define ADDR_FLASH_PAGE_111   ((uint32_t)0x08037800) /* Base @ of Page 111, 2 Kbytes */
#define ADDR_FLASH_PAGE_112   ((uint32_t)0x08038000) /* Base @ of Page 112, 2 Kbytes */
#define ADDR_FLASH_PAGE_113   ((uint32_t)0x08038800) /* Base @ of Page 113, 2 Kbytes */
#define ADDR_FLASH_PAGE_114   ((uint32_t)0x08039000) /* Base @ of Page 114, 2 Kbytes */
#define ADDR_FLASH_PAGE_115   ((uint32_t)0x08039800) /* Base @ of Page 115, 2 Kbytes */
#define ADDR_FLASH_PAGE_116   ((uint32_t)0x0803A000) /* Base @ of Page 116, 2 Kbytes */
#define ADDR_FLASH_PAGE_117   ((uint32_t)0x0803A800) /* Base @ of Page 117, 2 Kbytes */
#define ADDR_FLASH_PAGE_118   ((uint32_t)0x0803B000) /* Base @ of Page 118, 2 Kbytes */
#define ADDR_FLASH_PAGE_119   ((uint32_t)0x0803B800) /* Base @ of Page 119, 2 Kbytes */
#define ADDR_FLASH_PAGE_120   ((uint32_t)0x0803C000) /* Base @ of Page 120, 2 Kbytes */
#define ADDR_FLASH_PAGE_121   ((uint32_t)0x0803C800) /* Base @ of Page 121, 2 Kbytes */
#define ADDR_FLASH_PAGE_122   ((uint32_t)0x0803D000) /* Base @ of Page 122, 2 Kbytes */
#define ADDR_FLASH_PAGE_123   ((uint32_t)0x0803D800) /* Base @ of Page 123, 2 Kbytes */
#define ADDR_FLASH_PAGE_124   ((uint32_t)0x0803E000) /* Base @ of Page 124, 2 Kbytes */
#define ADDR_FLASH_PAGE_125   ((uint32_t)0x0803E800) /* Base @ of Page 125, 2 Kbytes */
#define ADDR_FLASH_PAGE_126   ((uint32_t)0x0803F000) /* Base @ of Page 126, 2 Kbytes */
#define ADDR_FLASH_PAGE_127   ((uint32_t)0x0803F800) /* Base @ of Page 127, 2 Kbytes */
#define ADDR_FLASH_PAGE_128   ((uint32_t)0x08040000) /* Base @ of Page 128, 2 Kbytes */
#define ADDR_FLASH_PAGE_129   ((uint32_t)0x08040800) /* Base @ of Page 129, 2 Kbytes */
#define ADDR_FLASH_PAGE_130   ((uint32_t)0x08041000) /* Base @ of Page 130, 2 Kbytes */
#define ADDR_FLASH_PAGE_131   ((uint32_t)0x08041800) /* Base @ of Page 131, 2 Kbytes */
#define ADDR_FLASH_PAGE_132   ((uint32_t)0x08042000) /* Base @ of Page 132, 2 Kbytes */
#define ADDR_FLASH_PAGE_133   ((uint32_t)0x08042800) /* Base @ of Page 133, 2 Kbytes */
#define ADDR_FLASH_PAGE_134   ((uint32_t)0x08043000) /* Base @ of Page 134, 2 Kbytes */
#define ADDR_FLASH_PAGE_135   ((uint32_t)0x08043800) /* Base @ of Page 135, 2 Kbytes */
#define ADDR_FLASH_PAGE_136   ((uint32_t)0x08044000) /* Base @ of Page 136, 2 Kbytes */
#define ADDR_FLASH_PAGE_137   ((uint32_t)0x08044800) /* Base @ of Page 137, 2 Kbytes */
#define ADDR_FLASH_PAGE_138   ((uint32_t)0x08045000) /* Base @ of Page 138, 2 Kbytes */
#define ADDR_FLASH_PAGE_139   ((uint32_t)0x08045800) /* Base @ of Page 139, 2 Kbytes */
#define ADDR_FLASH_PAGE_140   ((uint32_t)0x08046000) /* Base @ of Page 140, 2 Kbytes */
#define ADDR_FLASH_PAGE_141   ((uint32_t)0x08046800) /* Base @ of Page 141, 2 Kbytes */
#define ADDR_FLASH_PAGE_142   ((uint32_t)0x08047000) /* Base @ of Page 142, 2 Kbytes */
#define ADDR_FLASH_PAGE_143   ((uint32_t)0x08047800) /* Base @ of Page 143, 2 Kbytes */
#define ADDR_FLASH_PAGE_144   ((uint32_t)0x08048000) /* Base @ of Page 144, 2 Kbytes */
#define ADDR_FLASH_PAGE_145   ((uint32_t)0x08048800) /* Base @ of Page 145, 2 Kbytes */
#define ADDR_FLASH_PAGE_146   ((uint32_t)0x08049000) /* Base @ of Page 146, 2 Kbytes */
#define ADDR_FLASH_PAGE_147   ((uint32_t)0x08049800) /* Base @ of Page 147, 2 Kbytes */
#define ADDR_FLASH_PAGE_148   ((uint32_t)0x0804a000) /* Base @ of Page 148, 2 Kbytes */
#define ADDR_FLASH_PAGE_149   ((uint32_t)0x0804a800) /* Base @ of Page 149, 2 Kbytes */
#define ADDR_FLASH_PAGE_150   ((uint32_t)0x0804b000) /* Base @ of Page 150, 2 Kbytes */
#define ADDR_FLASH_PAGE_151   ((uint32_t)0x0804b800) /* Base @ of Page 151, 2 Kbytes */
#define ADDR_FLASH_PAGE_152   ((uint32_t)0x0804c000) /* Base @ of Page 152, 2 Kbytes */
#define ADDR_FLASH_PAGE_153   ((uint32_t)0x0804c800) /* Base @ of Page 153, 2 Kbytes */
#define ADDR_FLASH_PAGE_154   ((uint32_t)0x0804d000) /* Base @ of Page 154, 2 Kbytes */
#define ADDR_FLASH_PAGE_155   ((uint32_t)0x0804d800) /* Base @ of Page 155, 2 Kbytes */
#define ADDR_FLASH_PAGE_156   ((uint32_t)0x0804e000) /* Base @ of Page 156, 2 Kbytes */
#define ADDR_FLASH_PAGE_157   ((uint32_t)0x0804e800) /* Base @ of Page 157, 2 Kbytes */
#define ADDR_FLASH_PAGE_158   ((uint32_t)0x0804f000) /* Base @ of Page 158, 2 Kbytes */
#define ADDR_FLASH_PAGE_159   ((uint32_t)0x0804f800) /* Base @ of Page 159, 2 Kbytes */
#define ADDR_FLASH_PAGE_160   ((uint32_t)0x08050000) /* Base @ of Page 160, 2 Kbytes */
#define ADDR_FLASH_PAGE_161   ((uint32_t)0x08050800) /* Base @ of Page 161, 2 Kbytes */
#define ADDR_FLASH_PAGE_162   ((uint32_t)0x08051000) /* Base @ of Page 162, 2 Kbytes */
#define ADDR_FLASH_PAGE_163   ((uint32_t)0x08051800) /* Base @ of Page 163, 2 Kbytes */
#define ADDR_FLASH_PAGE_164   ((uint32_t)0x08052000) /* Base @ of Page 164, 2 Kbytes */
#define ADDR_FLASH_PAGE_165   ((uint32_t)0x08052800) /* Base @ of Page 165, 2 Kbytes */
#define ADDR_FLASH_PAGE_166   ((uint32_t)0x08053000) /* Base @ of Page 166, 2 Kbytes */
#define ADDR_FLASH_PAGE_167   ((uint32_t)0x08053800) /* Base @ of Page 167, 2 Kbytes */
#define ADDR_FLASH_PAGE_168   ((uint32_t)0x08054000) /* Base @ of Page 168, 2 Kbytes */
#define ADDR_FLASH_PAGE_169   ((uint32_t)0x08054800) /* Base @ of Page 169, 2 Kbytes */
#define ADDR_FLASH_PAGE_170   ((uint32_t)0x08055000) /* Base @ of Page 170, 2 Kbytes */
#define ADDR_FLASH_PAGE_171   ((uint32_t)0x08055800) /* Base @ of Page 171, 2 Kbytes */
#define ADDR_FLASH_PAGE_172   ((uint32_t)0x08056000) /* Base @ of Page 172, 2 Kbytes */
#define ADDR_FLASH_PAGE_173   ((uint32_t)0x08056800) /* Base @ of Page 173, 2 Kbytes */
#define ADDR_FLASH_PAGE_174   ((uint32_t)0x08057000) /* Base @ of Page 174, 2 Kbytes */
#define ADDR_FLASH_PAGE_175   ((uint32_t)0x08057800) /* Base @ of Page 175, 2 Kbytes */
#define ADDR_FLASH_PAGE_176   ((uint32_t)0x08058000) /* Base @ of Page 176, 2 Kbytes */
#define ADDR_FLASH_PAGE_177   ((uint32_t)0x08058800) /* Base @ of Page 177, 2 Kbytes */
#define ADDR_FLASH_PAGE_178   ((uint32_t)0x08059000) /* Base @ of Page 178, 2 Kbytes */
#define ADDR_FLASH_PAGE_179   ((uint32_t)0x08059800) /* Base @ of Page 179, 2 Kbytes */
#define ADDR_FLASH_PAGE_180   ((uint32_t)0x0805a000) /* Base @ of Page 180, 2 Kbytes */
#define ADDR_FLASH_PAGE_181   ((uint32_t)0x0805a800) /* Base @ of Page 181, 2 Kbytes */
#define ADDR_FLASH_PAGE_182   ((uint32_t)0x0805b000) /* Base @ of Page 182, 2 Kbytes */
#define ADDR_FLASH_PAGE_183   ((uint32_t)0x0805b800) /* Base @ of Page 183, 2 Kbytes */
#define ADDR_FLASH_PAGE_184   ((uint32_t)0x0805c000) /* Base @ of Page 184, 2 Kbytes */
#define ADDR_FLASH_PAGE_185   ((uint32_t)0x0805c800) /* Base @ of Page 185, 2 Kbytes */
#define ADDR_FLASH_PAGE_186   ((uint32_t)0x0805d000) /* Base @ of Page 186, 2 Kbytes */
#define ADDR_FLASH_PAGE_187   ((uint32_t)0x0805d800) /* Base @ of Page 187, 2 Kbytes */
#define ADDR_FLASH_PAGE_188   ((uint32_t)0x0805e000) /* Base @ of Page 188, 2 Kbytes */
#define ADDR_FLASH_PAGE_189   ((uint32_t)0x0805e800) /* Base @ of Page 189, 2 Kbytes */
#define ADDR_FLASH_PAGE_190   ((uint32_t)0x0805f000) /* Base @ of Page 190, 2 Kbytes */
#define ADDR_FLASH_PAGE_191   ((uint32_t)0x0805f800) /* Base @ of Page 191, 2 Kbytes */
#define ADDR_FLASH_PAGE_192   ((uint32_t)0x08060000) /* Base @ of Page 192, 2 Kbytes */
#define ADDR_FLASH_PAGE_193   ((uint32_t)0x08060800) /* Base @ of Page 193, 2 Kbytes */
#define ADDR_FLASH_PAGE_194   ((uint32_t)0x08061000) /* Base @ of Page 194, 2 Kbytes */
#define ADDR_FLASH_PAGE_195   ((uint32_t)0x08061800) /* Base @ of Page 195, 2 Kbytes */
#define ADDR_FLASH_PAGE_196   ((uint32_t)0x08062000) /* Base @ of Page 196, 2 Kbytes */
#define ADDR_FLASH_PAGE_197   ((uint32_t)0x08062800) /* Base @ of Page 197, 2 Kbytes */
#define ADDR_FLASH_PAGE_198   ((uint32_t)0x08063000) /* Base @ of Page 198, 2 Kbytes */
#define ADDR_FLASH_PAGE_199   ((uint32_t)0x08063800) /* Base @ of Page 199, 2 Kbytes */
#define ADDR_FLASH_PAGE_200   ((uint32_t)0x08064000) /* Base @ of Page 200, 2 Kbytes */
#define ADDR_FLASH_PAGE_201   ((uint32_t)0x08064800) /* Base @ of Page 201, 2 Kbytes */
#define ADDR_FLASH_PAGE_202   ((uint32_t)0x08065000) /* Base @ of Page 202, 2 Kbytes */
#define ADDR_FLASH_PAGE_203   ((uint32_t)0x08065800) /* Base @ of Page 203, 2 Kbytes */
#define ADDR_FLASH_PAGE_204   ((uint32_t)0x08066000) /* Base @ of Page 204, 2 Kbytes */
#define ADDR_FLASH_PAGE_205   ((uint32_t)0x08066800) /* Base @ of Page 205, 2 Kbytes */
#define ADDR_FLASH_PAGE_206   ((uint32_t)0x08067000) /* Base @ of Page 206, 2 Kbytes */
#define ADDR_FLASH_PAGE_207   ((uint32_t)0x08067800) /* Base @ of Page 207, 2 Kbytes */
#define ADDR_FLASH_PAGE_208   ((uint32_t)0x08068000) /* Base @ of Page 208, 2 Kbytes */
#define ADDR_FLASH_PAGE_209   ((uint32_t)0x08068800) /* Base @ of Page 209, 2 Kbytes */
#define ADDR_FLASH_PAGE_210   ((uint32_t)0x08069000) /* Base @ of Page 210, 2 Kbytes */
#define ADDR_FLASH_PAGE_211   ((uint32_t)0x08069800) /* Base @ of Page 211, 2 Kbytes */
#define ADDR_FLASH_PAGE_212   ((uint32_t)0x0806a000) /* Base @ of Page 212, 2 Kbytes */
#define ADDR_FLASH_PAGE_213   ((uint32_t)0x0806a800) /* Base @ of Page 213, 2 Kbytes */
#define ADDR_FLASH_PAGE_214   ((uint32_t)0x0806b000) /* Base @ of Page 214, 2 Kbytes */
#define ADDR_FLASH_PAGE_215   ((uint32_t)0x0806b800) /* Base @ of Page 215, 2 Kbytes */
#define ADDR_FLASH_PAGE_216   ((uint32_t)0x0806c000) /* Base @ of Page 216, 2 Kbytes */
#define ADDR_FLASH_PAGE_217   ((uint32_t)0x0806c800) /* Base @ of Page 217, 2 Kbytes */
#define ADDR_FLASH_PAGE_218   ((uint32_t)0x0806d000) /* Base @ of Page 218, 2 Kbytes */
#define ADDR_FLASH_PAGE_219   ((uint32_t)0x0806d800) /* Base @ of Page 219, 2 Kbytes */
#define ADDR_FLASH_PAGE_220   ((uint32_t)0x0806e000) /* Base @ of Page 220, 2 Kbytes */
#define ADDR_FLASH_PAGE_221   ((uint32_t)0x0806e800) /* Base @ of Page 221, 2 Kbytes */
#define ADDR_FLASH_PAGE_222   ((uint32_t)0x0806f000) /* Base @ of Page 222, 2 Kbytes */
#define ADDR_FLASH_PAGE_223   ((uint32_t)0x0806f800) /* Base @ of Page 223, 2 Kbytes */
#define ADDR_FLASH_PAGE_224   ((uint32_t)0x08070000) /* Base @ of Page 224, 2 Kbytes */
#define ADDR_FLASH_PAGE_225   ((uint32_t)0x08070800) /* Base @ of Page 225, 2 Kbytes */
#define ADDR_FLASH_PAGE_226   ((uint32_t)0x08071000) /* Base @ of Page 226, 2 Kbytes */
#define ADDR_FLASH_PAGE_227   ((uint32_t)0x08071800) /* Base @ of Page 227, 2 Kbytes */
#define ADDR_FLASH_PAGE_228   ((uint32_t)0x08072000) /* Base @ of Page 228, 2 Kbytes */
#define ADDR_FLASH_PAGE_229   ((uint32_t)0x08072800) /* Base @ of Page 229, 2 Kbytes */
#define ADDR_FLASH_PAGE_230   ((uint32_t)0x08073000) /* Base @ of Page 230, 2 Kbytes */
#define ADDR_FLASH_PAGE_231   ((uint32_t)0x08073800) /* Base @ of Page 231, 2 Kbytes */
#define ADDR_FLASH_PAGE_232   ((uint32_t)0x08074000) /* Base @ of Page 232, 2 Kbytes */
#define ADDR_FLASH_PAGE_233   ((uint32_t)0x08074800) /* Base @ of Page 233, 2 Kbytes */
#define ADDR_FLASH_PAGE_234   ((uint32_t)0x08075000) /* Base @ of Page 234, 2 Kbytes */
#define ADDR_FLASH_PAGE_235   ((uint32_t)0x08075800) /* Base @ of Page 235, 2 Kbytes */
#define ADDR_FLASH_PAGE_236   ((uint32_t)0x08076000) /* Base @ of Page 236, 2 Kbytes */
#define ADDR_FLASH_PAGE_237   ((uint32_t)0x08076800) /* Base @ of Page 237, 2 Kbytes */
#define ADDR_FLASH_PAGE_238   ((uint32_t)0x08077000) /* Base @ of Page 238, 2 Kbytes */
#define ADDR_FLASH_PAGE_239   ((uint32_t)0x08077800) /* Base @ of Page 239, 2 Kbytes */
#define ADDR_FLASH_PAGE_240   ((uint32_t)0x08078000) /* Base @ of Page 240, 2 Kbytes */
#define ADDR_FLASH_PAGE_241   ((uint32_t)0x08078800) /* Base @ of Page 241, 2 Kbytes */
#define ADDR_FLASH_PAGE_242   ((uint32_t)0x08079000) /* Base @ of Page 242, 2 Kbytes */
#define ADDR_FLASH_PAGE_243   ((uint32_t)0x08079800) /* Base @ of Page 243, 2 Kbytes */
#define ADDR_FLASH_PAGE_244   ((uint32_t)0x0807a000) /* Base @ of Page 244, 2 Kbytes */
#define ADDR_FLASH_PAGE_245   ((uint32_t)0x0807a800) /* Base @ of Page 245, 2 Kbytes */
#define ADDR_FLASH_PAGE_246   ((uint32_t)0x0807b000) /* Base @ of Page 246, 2 Kbytes */
#define ADDR_FLASH_PAGE_247   ((uint32_t)0x0807b800) /* Base @ of Page 247, 2 Kbytes */
#define ADDR_FLASH_PAGE_248   ((uint32_t)0x0807c000) /* Base @ of Page 248, 2 Kbytes */
#define ADDR_FLASH_PAGE_249   ((uint32_t)0x0807c800) /* Base @ of Page 249, 2 Kbytes */
#define ADDR_FLASH_PAGE_250   ((uint32_t)0x0807d000) /* Base @ of Page 250, 2 Kbytes */
#define ADDR_FLASH_PAGE_251   ((uint32_t)0x0807d800) /* Base @ of Page 251, 2 Kbytes */
#define ADDR_FLASH_PAGE_252   ((uint32_t)0x0807e000) /* Base @ of Page 252, 2 Kbytes */
#define ADDR_FLASH_PAGE_253   ((uint32_t)0x0807e800) /* Base @ of Page 253, 2 Kbytes */
#define ADDR_FLASH_PAGE_254   ((uint32_t)0x0807f000) /* Base @ of Page 254, 2 Kbytes */
#define ADDR_FLASH_PAGE_255   ((uint32_t)0x0807f800) /* Base @ of Page 255, 2 Kbytes */
#define ADDR_FLASH_PAGE_256   ((uint32_t)0x08080000) /* Base @ of Page 256, 2 Kbytes */
#define ADDR_FLASH_PAGE_257   ((uint32_t)0x08080800) /* Base @ of Page 257, 2 Kbytes */
#define ADDR_FLASH_PAGE_258   ((uint32_t)0x08081000) /* Base @ of Page 258, 2 Kbytes */
#define ADDR_FLASH_PAGE_259   ((uint32_t)0x08081800) /* Base @ of Page 259, 2 Kbytes */
#define ADDR_FLASH_PAGE_260   ((uint32_t)0x08082000) /* Base @ of Page 260, 2 Kbytes */
#define ADDR_FLASH_PAGE_261   ((uint32_t)0x08082800) /* Base @ of Page 261, 2 Kbytes */
#define ADDR_FLASH_PAGE_262   ((uint32_t)0x08083000) /* Base @ of Page 262, 2 Kbytes */
#define ADDR_FLASH_PAGE_263   ((uint32_t)0x08083800) /* Base @ of Page 263, 2 Kbytes */
#define ADDR_FLASH_PAGE_264   ((uint32_t)0x08084000) /* Base @ of Page 264, 2 Kbytes */
#define ADDR_FLASH_PAGE_265   ((uint32_t)0x08084800) /* Base @ of Page 265, 2 Kbytes */
#define ADDR_FLASH_PAGE_266   ((uint32_t)0x08085000) /* Base @ of Page 266, 2 Kbytes */
#define ADDR_FLASH_PAGE_267   ((uint32_t)0x08085800) /* Base @ of Page 267, 2 Kbytes */
#define ADDR_FLASH_PAGE_268   ((uint32_t)0x08086000) /* Base @ of Page 268, 2 Kbytes */
#define ADDR_FLASH_PAGE_269   ((uint32_t)0x08086800) /* Base @ of Page 269, 2 Kbytes */
#define ADDR_FLASH_PAGE_270   ((uint32_t)0x08087000) /* Base @ of Page 270, 2 Kbytes */
#define ADDR_FLASH_PAGE_271   ((uint32_t)0x08087800) /* Base @ of Page 271, 2 Kbytes */
#define ADDR_FLASH_PAGE_272   ((uint32_t)0x08088000) /* Base @ of Page 272, 2 Kbytes */
#define ADDR_FLASH_PAGE_273   ((uint32_t)0x08088800) /* Base @ of Page 273, 2 Kbytes */
#define ADDR_FLASH_PAGE_274   ((uint32_t)0x08089000) /* Base @ of Page 274, 2 Kbytes */
#define ADDR_FLASH_PAGE_275   ((uint32_t)0x08089800) /* Base @ of Page 275, 2 Kbytes */
#define ADDR_FLASH_PAGE_276   ((uint32_t)0x0808A000) /* Base @ of Page 276, 2 Kbytes */
#define ADDR_FLASH_PAGE_277   ((uint32_t)0x0808A800) /* Base @ of Page 277, 2 Kbytes */
#define ADDR_FLASH_PAGE_278   ((uint32_t)0x0808B000) /* Base @ of Page 278, 2 Kbytes */
#define ADDR_FLASH_PAGE_279   ((uint32_t)0x0808B800) /* Base @ of Page 279, 2 Kbytes */
#define ADDR_FLASH_PAGE_280   ((uint32_t)0x0808C000) /* Base @ of Page 280, 2 Kbytes */
#define ADDR_FLASH_PAGE_281   ((uint32_t)0x0808C800) /* Base @ of Page 281, 2 Kbytes */
#define ADDR_FLASH_PAGE_282   ((uint32_t)0x0808D000) /* Base @ of Page 282, 2 Kbytes */
#define ADDR_FLASH_PAGE_283   ((uint32_t)0x0808D800) /* Base @ of Page 283, 2 Kbytes */
#define ADDR_FLASH_PAGE_284   ((uint32_t)0x0808E000) /* Base @ of Page 284, 2 Kbytes */
#define ADDR_FLASH_PAGE_285   ((uint32_t)0x0808E800) /* Base @ of Page 285, 2 Kbytes */
#define ADDR_FLASH_PAGE_286   ((uint32_t)0x0808F000) /* Base @ of Page 286, 2 Kbytes */
#define ADDR_FLASH_PAGE_287   ((uint32_t)0x0808F800) /* Base @ of Page 287, 2 Kbytes */
#define ADDR_FLASH_PAGE_288   ((uint32_t)0x08090000) /* Base @ of Page 288, 2 Kbytes */
#define ADDR_FLASH_PAGE_289   ((uint32_t)0x08090800) /* Base @ of Page 289, 2 Kbytes */
#define ADDR_FLASH_PAGE_290   ((uint32_t)0x08091000) /* Base @ of Page 290, 2 Kbytes */
#define ADDR_FLASH_PAGE_291   ((uint32_t)0x08091800) /* Base @ of Page 291, 2 Kbytes */
#define ADDR_FLASH_PAGE_292   ((uint32_t)0x08092000) /* Base @ of Page 292, 2 Kbytes */
#define ADDR_FLASH_PAGE_293   ((uint32_t)0x08092800) /* Base @ of Page 293, 2 Kbytes */
#define ADDR_FLASH_PAGE_294   ((uint32_t)0x08093000) /* Base @ of Page 294, 2 Kbytes */
#define ADDR_FLASH_PAGE_295   ((uint32_t)0x08093800) /* Base @ of Page 295, 2 Kbytes */
#define ADDR_FLASH_PAGE_296   ((uint32_t)0x08094000) /* Base @ of Page 296, 2 Kbytes */
#define ADDR_FLASH_PAGE_297   ((uint32_t)0x08094800) /* Base @ of Page 297, 2 Kbytes */
#define ADDR_FLASH_PAGE_298   ((uint32_t)0x08095000) /* Base @ of Page 298, 2 Kbytes */
#define ADDR_FLASH_PAGE_299   ((uint32_t)0x08095800) /* Base @ of Page 299, 2 Kbytes */
#define ADDR_FLASH_PAGE_300   ((uint32_t)0x08096000) /* Base @ of Page 300, 2 Kbytes */
#define ADDR_FLASH_PAGE_301   ((uint32_t)0x08096800) /* Base @ of Page 301, 2 Kbytes */
#define ADDR_FLASH_PAGE_302   ((uint32_t)0x08097000) /* Base @ of Page 302, 2 Kbytes */
#define ADDR_FLASH_PAGE_303   ((uint32_t)0x08097800) /* Base @ of Page 303, 2 Kbytes */
#define ADDR_FLASH_PAGE_304   ((uint32_t)0x08098000) /* Base @ of Page 304, 2 Kbytes */
#define ADDR_FLASH_PAGE_305   ((uint32_t)0x08098800) /* Base @ of Page 305, 2 Kbytes */
#define ADDR_FLASH_PAGE_306   ((uint32_t)0x08099000) /* Base @ of Page 306, 2 Kbytes */
#define ADDR_FLASH_PAGE_307   ((uint32_t)0x08099800) /* Base @ of Page 307, 2 Kbytes */
#define ADDR_FLASH_PAGE_308   ((uint32_t)0x0809A000) /* Base @ of Page 308, 2 Kbytes */
#define ADDR_FLASH_PAGE_309   ((uint32_t)0x0809A800) /* Base @ of Page 309, 2 Kbytes */
#define ADDR_FLASH_PAGE_310   ((uint32_t)0x0809B000) /* Base @ of Page 310, 2 Kbytes */
#define ADDR_FLASH_PAGE_311   ((uint32_t)0x0809B800) /* Base @ of Page 311, 2 Kbytes */
#define ADDR_FLASH_PAGE_312   ((uint32_t)0x0809C000) /* Base @ of Page 312, 2 Kbytes */
#define ADDR_FLASH_PAGE_313   ((uint32_t)0x0809C800) /* Base @ of Page 313, 2 Kbytes */
#define ADDR_FLASH_PAGE_314   ((uint32_t)0x0809D000) /* Base @ of Page 314, 2 Kbytes */
#define ADDR_FLASH_PAGE_315   ((uint32_t)0x0809D800) /* Base @ of Page 315, 2 Kbytes */
#define ADDR_FLASH_PAGE_316   ((uint32_t)0x0809E000) /* Base @ of Page 316, 2 Kbytes */
#define ADDR_FLASH_PAGE_317   ((uint32_t)0x0809E800) /* Base @ of Page 317, 2 Kbytes */
#define ADDR_FLASH_PAGE_318   ((uint32_t)0x0809F000) /* Base @ of Page 318, 2 Kbytes */
#define ADDR_FLASH_PAGE_319   ((uint32_t)0x0809F800) /* Base @ of Page 319, 2 Kbytes */
#define ADDR_FLASH_PAGE_320   ((uint32_t)0x080a0000) /* Base @ of Page 320, 2 Kbytes */
#define ADDR_FLASH_PAGE_321   ((uint32_t)0x080a0800) /* Base @ of Page 321, 2 Kbytes */
#define ADDR_FLASH_PAGE_322   ((uint32_t)0x080a1000) /* Base @ of Page 322, 2 Kbytes */
#define ADDR_FLASH_PAGE_323   ((uint32_t)0x080a1800) /* Base @ of Page 323, 2 Kbytes */
#define ADDR_FLASH_PAGE_324   ((uint32_t)0x080a2000) /* Base @ of Page 324, 2 Kbytes */
#define ADDR_FLASH_PAGE_325   ((uint32_t)0x080a2800) /* Base @ of Page 325, 2 Kbytes */
#define ADDR_FLASH_PAGE_326   ((uint32_t)0x080a3000) /* Base @ of Page 326, 2 Kbytes */
#define ADDR_FLASH_PAGE_327   ((uint32_t)0x080a3800) /* Base @ of Page 327, 2 Kbytes */
#define ADDR_FLASH_PAGE_328   ((uint32_t)0x080a4000) /* Base @ of Page 328, 2 Kbytes */
#define ADDR_FLASH_PAGE_329   ((uint32_t)0x080a4800) /* Base @ of Page 329, 2 Kbytes */
#define ADDR_FLASH_PAGE_330   ((uint32_t)0x080a5000) /* Base @ of Page 330, 2 Kbytes */
#define ADDR_FLASH_PAGE_331   ((uint32_t)0x080a5800) /* Base @ of Page 331, 2 Kbytes */
#define ADDR_FLASH_PAGE_332   ((uint32_t)0x080a6000) /* Base @ of Page 332, 2 Kbytes */
#define ADDR_FLASH_PAGE_333   ((uint32_t)0x080a6800) /* Base @ of Page 333, 2 Kbytes */
#define ADDR_FLASH_PAGE_334   ((uint32_t)0x080a7000) /* Base @ of Page 334, 2 Kbytes */
#define ADDR_FLASH_PAGE_335   ((uint32_t)0x080a7800) /* Base @ of Page 335, 2 Kbytes */
#define ADDR_FLASH_PAGE_336   ((uint32_t)0x080a8000) /* Base @ of Page 336, 2 Kbytes */
#define ADDR_FLASH_PAGE_337   ((uint32_t)0x080a8800) /* Base @ of Page 337, 2 Kbytes */
#define ADDR_FLASH_PAGE_338   ((uint32_t)0x080a9000) /* Base @ of Page 338, 2 Kbytes */
#define ADDR_FLASH_PAGE_339   ((uint32_t)0x080a9800) /* Base @ of Page 339, 2 Kbytes */
#define ADDR_FLASH_PAGE_340   ((uint32_t)0x080aA000) /* Base @ of Page 340, 2 Kbytes */
#define ADDR_FLASH_PAGE_341   ((uint32_t)0x080aA800) /* Base @ of Page 341, 2 Kbytes */
#define ADDR_FLASH_PAGE_342   ((uint32_t)0x080aB000) /* Base @ of Page 342, 2 Kbytes */
#define ADDR_FLASH_PAGE_343   ((uint32_t)0x080aB800) /* Base @ of Page 343, 2 Kbytes */
#define ADDR_FLASH_PAGE_344   ((uint32_t)0x080aC000) /* Base @ of Page 344, 2 Kbytes */
#define ADDR_FLASH_PAGE_345   ((uint32_t)0x080aC800) /* Base @ of Page 345, 2 Kbytes */
#define ADDR_FLASH_PAGE_346   ((uint32_t)0x080aD000) /* Base @ of Page 346, 2 Kbytes */
#define ADDR_FLASH_PAGE_347   ((uint32_t)0x080aD800) /* Base @ of Page 347, 2 Kbytes */
#define ADDR_FLASH_PAGE_348   ((uint32_t)0x080aE000) /* Base @ of Page 348, 2 Kbytes */
#define ADDR_FLASH_PAGE_349   ((uint32_t)0x080aE800) /* Base @ of Page 349, 2 Kbytes */
#define ADDR_FLASH_PAGE_350   ((uint32_t)0x080aF000) /* Base @ of Page 350, 2 Kbytes */
#define ADDR_FLASH_PAGE_351   ((uint32_t)0x080aF800) /* Base @ of Page 351, 2 Kbytes */
#define ADDR_FLASH_PAGE_352   ((uint32_t)0x080b0000) /* Base @ of Page 352, 2 Kbytes */
#define ADDR_FLASH_PAGE_353   ((uint32_t)0x080b0800) /* Base @ of Page 353, 2 Kbytes */
#define ADDR_FLASH_PAGE_354   ((uint32_t)0x080b1000) /* Base @ of Page 354, 2 Kbytes */
#define ADDR_FLASH_PAGE_355   ((uint32_t)0x080b1800) /* Base @ of Page 355, 2 Kbytes */
#define ADDR_FLASH_PAGE_356   ((uint32_t)0x080b2000) /* Base @ of Page 356, 2 Kbytes */
#define ADDR_FLASH_PAGE_357   ((uint32_t)0x080b2800) /* Base @ of Page 357, 2 Kbytes */
#define ADDR_FLASH_PAGE_358   ((uint32_t)0x080b3000) /* Base @ of Page 358, 2 Kbytes */
#define ADDR_FLASH_PAGE_359   ((uint32_t)0x080b3800) /* Base @ of Page 359, 2 Kbytes */
#define ADDR_FLASH_PAGE_360   ((uint32_t)0x080b4000) /* Base @ of Page 360, 2 Kbytes */
#define ADDR_FLASH_PAGE_361   ((uint32_t)0x080b4800) /* Base @ of Page 361, 2 Kbytes */
#define ADDR_FLASH_PAGE_362   ((uint32_t)0x080b5000) /* Base @ of Page 362, 2 Kbytes */
#define ADDR_FLASH_PAGE_363   ((uint32_t)0x080b5800) /* Base @ of Page 363, 2 Kbytes */
#define ADDR_FLASH_PAGE_364   ((uint32_t)0x080b6000) /* Base @ of Page 364, 2 Kbytes */
#define ADDR_FLASH_PAGE_365   ((uint32_t)0x080b6800) /* Base @ of Page 365, 2 Kbytes */
#define ADDR_FLASH_PAGE_366   ((uint32_t)0x080b7000) /* Base @ of Page 366, 2 Kbytes */
#define ADDR_FLASH_PAGE_367   ((uint32_t)0x080b7800) /* Base @ of Page 367, 2 Kbytes */
#define ADDR_FLASH_PAGE_368   ((uint32_t)0x080b8000) /* Base @ of Page 368, 2 Kbytes */
#define ADDR_FLASH_PAGE_369   ((uint32_t)0x080b8800) /* Base @ of Page 369, 2 Kbytes */
#define ADDR_FLASH_PAGE_370   ((uint32_t)0x080b9000) /* Base @ of Page 370, 2 Kbytes */
#define ADDR_FLASH_PAGE_371   ((uint32_t)0x080b9800) /* Base @ of Page 371, 2 Kbytes */
#define ADDR_FLASH_PAGE_372   ((uint32_t)0x080bA000) /* Base @ of Page 372, 2 Kbytes */
#define ADDR_FLASH_PAGE_373   ((uint32_t)0x080bA800) /* Base @ of Page 373, 2 Kbytes */
#define ADDR_FLASH_PAGE_374   ((uint32_t)0x080bB000) /* Base @ of Page 374, 2 Kbytes */
#define ADDR_FLASH_PAGE_375   ((uint32_t)0x080bB800) /* Base @ of Page 375, 2 Kbytes */
#define ADDR_FLASH_PAGE_376   ((uint32_t)0x080bC000) /* Base @ of Page 376, 2 Kbytes */
#define ADDR_FLASH_PAGE_377   ((uint32_t)0x080bC800) /* Base @ of Page 377, 2 Kbytes */
#define ADDR_FLASH_PAGE_378   ((uint32_t)0x080bD000) /* Base @ of Page 378, 2 Kbytes */
#define ADDR_FLASH_PAGE_379   ((uint32_t)0x080bD800) /* Base @ of Page 379, 2 Kbytes */
#define ADDR_FLASH_PAGE_380   ((uint32_t)0x080bE000) /* Base @ of Page 380, 2 Kbytes */
#define ADDR_FLASH_PAGE_381   ((uint32_t)0x080bE800) /* Base @ of Page 381, 2 Kbytes */
#define ADDR_FLASH_PAGE_382   ((uint32_t)0x080bF000) /* Base @ of Page 382, 2 Kbytes */
#define ADDR_FLASH_PAGE_383   ((uint32_t)0x080bF800) /* Base @ of Page 383, 2 Kbytes */
#define ADDR_FLASH_PAGE_384   ((uint32_t)0x080c0000) /* Base @ of Page 384, 2 Kbytes */
#define ADDR_FLASH_PAGE_385   ((uint32_t)0x080c0800) /* Base @ of Page 385, 2 Kbytes */
#define ADDR_FLASH_PAGE_386   ((uint32_t)0x080c1000) /* Base @ of Page 386, 2 Kbytes */
#define ADDR_FLASH_PAGE_387   ((uint32_t)0x080c1800) /* Base @ of Page 387, 2 Kbytes */
#define ADDR_FLASH_PAGE_388   ((uint32_t)0x080c2000) /* Base @ of Page 388, 2 Kbytes */
#define ADDR_FLASH_PAGE_389   ((uint32_t)0x080c2800) /* Base @ of Page 389, 2 Kbytes */
#define ADDR_FLASH_PAGE_390   ((uint32_t)0x080c3000) /* Base @ of Page 390, 2 Kbytes */
#define ADDR_FLASH_PAGE_391   ((uint32_t)0x080c3800) /* Base @ of Page 391, 2 Kbytes */
#define ADDR_FLASH_PAGE_392   ((uint32_t)0x080c4000) /* Base @ of Page 392, 2 Kbytes */
#define ADDR_FLASH_PAGE_393   ((uint32_t)0x080c4800) /* Base @ of Page 393, 2 Kbytes */
#define ADDR_FLASH_PAGE_394   ((uint32_t)0x080c5000) /* Base @ of Page 394, 2 Kbytes */
#define ADDR_FLASH_PAGE_395   ((uint32_t)0x080c5800) /* Base @ of Page 395, 2 Kbytes */
#define ADDR_FLASH_PAGE_396   ((uint32_t)0x080c6000) /* Base @ of Page 396, 2 Kbytes */
#define ADDR_FLASH_PAGE_397   ((uint32_t)0x080c6800) /* Base @ of Page 397, 2 Kbytes */
#define ADDR_FLASH_PAGE_398   ((uint32_t)0x080c7000) /* Base @ of Page 398, 2 Kbytes */
#define ADDR_FLASH_PAGE_399   ((uint32_t)0x080c7800) /* Base @ of Page 399, 2 Kbytes */
#define ADDR_FLASH_PAGE_400   ((uint32_t)0x080c8000) /* Base @ of Page 400, 2 Kbytes */
#define ADDR_FLASH_PAGE_401   ((uint32_t)0x080c8800) /* Base @ of Page 401, 2 Kbytes */
#define ADDR_FLASH_PAGE_402   ((uint32_t)0x080c9000) /* Base @ of Page 402, 2 Kbytes */
#define ADDR_FLASH_PAGE_403   ((uint32_t)0x080c9800) /* Base @ of Page 403, 2 Kbytes */
#define ADDR_FLASH_PAGE_404   ((uint32_t)0x080ca000) /* Base @ of Page 404, 2 Kbytes */
#define ADDR_FLASH_PAGE_405   ((uint32_t)0x080ca800) /* Base @ of Page 405, 2 Kbytes */
#define ADDR_FLASH_PAGE_406   ((uint32_t)0x080cb000) /* Base @ of Page 406, 2 Kbytes */
#define ADDR_FLASH_PAGE_407   ((uint32_t)0x080cb800) /* Base @ of Page 407, 2 Kbytes */
#define ADDR_FLASH_PAGE_408   ((uint32_t)0x080cc000) /* Base @ of Page 408, 2 Kbytes */
#define ADDR_FLASH_PAGE_409   ((uint32_t)0x080cc800) /* Base @ of Page 409, 2 Kbytes */
#define ADDR_FLASH_PAGE_410   ((uint32_t)0x080cd000) /* Base @ of Page 410, 2 Kbytes */
#define ADDR_FLASH_PAGE_411   ((uint32_t)0x080cd800) /* Base @ of Page 411, 2 Kbytes */
#define ADDR_FLASH_PAGE_412   ((uint32_t)0x080ce000) /* Base @ of Page 412, 2 Kbytes */
#define ADDR_FLASH_PAGE_413   ((uint32_t)0x080ce800) /* Base @ of Page 413, 2 Kbytes */
#define ADDR_FLASH_PAGE_414   ((uint32_t)0x080cf000) /* Base @ of Page 414, 2 Kbytes */
#define ADDR_FLASH_PAGE_415   ((uint32_t)0x080cf800) /* Base @ of Page 415, 2 Kbytes */
#define ADDR_FLASH_PAGE_416   ((uint32_t)0x080d0000) /* Base @ of Page 416, 2 Kbytes */
#define ADDR_FLASH_PAGE_417   ((uint32_t)0x080d0800) /* Base @ of Page 417, 2 Kbytes */
#define ADDR_FLASH_PAGE_418   ((uint32_t)0x080d1000) /* Base @ of Page 418, 2 Kbytes */
#define ADDR_FLASH_PAGE_419   ((uint32_t)0x080d1800) /* Base @ of Page 419, 2 Kbytes */
#define ADDR_FLASH_PAGE_420   ((uint32_t)0x080d2000) /* Base @ of Page 420, 2 Kbytes */
#define ADDR_FLASH_PAGE_421   ((uint32_t)0x080d2800) /* Base @ of Page 421, 2 Kbytes */
#define ADDR_FLASH_PAGE_422   ((uint32_t)0x080d3000) /* Base @ of Page 422, 2 Kbytes */
#define ADDR_FLASH_PAGE_423   ((uint32_t)0x080d3800) /* Base @ of Page 423, 2 Kbytes */
#define ADDR_FLASH_PAGE_424   ((uint32_t)0x080d4000) /* Base @ of Page 424, 2 Kbytes */
#define ADDR_FLASH_PAGE_425   ((uint32_t)0x080d4800) /* Base @ of Page 425, 2 Kbytes */
#define ADDR_FLASH_PAGE_426   ((uint32_t)0x080d5000) /* Base @ of Page 426, 2 Kbytes */
#define ADDR_FLASH_PAGE_427   ((uint32_t)0x080d5800) /* Base @ of Page 427, 2 Kbytes */
#define ADDR_FLASH_PAGE_428   ((uint32_t)0x080d6000) /* Base @ of Page 428, 2 Kbytes */
#define ADDR_FLASH_PAGE_429   ((uint32_t)0x080d6800) /* Base @ of Page 429, 2 Kbytes */
#define ADDR_FLASH_PAGE_430   ((uint32_t)0x080d7000) /* Base @ of Page 430, 2 Kbytes */
#define ADDR_FLASH_PAGE_431   ((uint32_t)0x080d7800) /* Base @ of Page 431, 2 Kbytes */
#define ADDR_FLASH_PAGE_432   ((uint32_t)0x080d8000) /* Base @ of Page 432, 2 Kbytes */
#define ADDR_FLASH_PAGE_433   ((uint32_t)0x080d8800) /* Base @ of Page 433, 2 Kbytes */
#define ADDR_FLASH_PAGE_434   ((uint32_t)0x080d9000) /* Base @ of Page 434, 2 Kbytes */
#define ADDR_FLASH_PAGE_435   ((uint32_t)0x080d9800) /* Base @ of Page 435, 2 Kbytes */
#define ADDR_FLASH_PAGE_436   ((uint32_t)0x080da000) /* Base @ of Page 436, 2 Kbytes */
#define ADDR_FLASH_PAGE_437   ((uint32_t)0x080da800) /* Base @ of Page 437, 2 Kbytes */
#define ADDR_FLASH_PAGE_438   ((uint32_t)0x080db000) /* Base @ of Page 438, 2 Kbytes */
#define ADDR_FLASH_PAGE_439   ((uint32_t)0x080db800) /* Base @ of Page 439, 2 Kbytes */
#define ADDR_FLASH_PAGE_440   ((uint32_t)0x080dc000) /* Base @ of Page 440, 2 Kbytes */
#define ADDR_FLASH_PAGE_441   ((uint32_t)0x080dc800) /* Base @ of Page 441, 2 Kbytes */
#define ADDR_FLASH_PAGE_442   ((uint32_t)0x080dd000) /* Base @ of Page 442, 2 Kbytes */
#define ADDR_FLASH_PAGE_443   ((uint32_t)0x080dd800) /* Base @ of Page 443, 2 Kbytes */
#define ADDR_FLASH_PAGE_444   ((uint32_t)0x080de000) /* Base @ of Page 444, 2 Kbytes */
#define ADDR_FLASH_PAGE_445   ((uint32_t)0x080de800) /* Base @ of Page 445, 2 Kbytes */
#define ADDR_FLASH_PAGE_446   ((uint32_t)0x080df000) /* Base @ of Page 446, 2 Kbytes */
#define ADDR_FLASH_PAGE_447   ((uint32_t)0x080df800) /* Base @ of Page 447, 2 Kbytes */
#define ADDR_FLASH_PAGE_448   ((uint32_t)0x080e0000) /* Base @ of Page 448, 2 Kbytes */
#define ADDR_FLASH_PAGE_449   ((uint32_t)0x080e0800) /* Base @ of Page 449, 2 Kbytes */
#define ADDR_FLASH_PAGE_450   ((uint32_t)0x080e1000) /* Base @ of Page 450, 2 Kbytes */
#define ADDR_FLASH_PAGE_451   ((uint32_t)0x080e1800) /* Base @ of Page 451, 2 Kbytes */
#define ADDR_FLASH_PAGE_452   ((uint32_t)0x080e2000) /* Base @ of Page 452, 2 Kbytes */
#define ADDR_FLASH_PAGE_453   ((uint32_t)0x080e2800) /* Base @ of Page 453, 2 Kbytes */
#define ADDR_FLASH_PAGE_454   ((uint32_t)0x080e3000) /* Base @ of Page 454, 2 Kbytes */
#define ADDR_FLASH_PAGE_455   ((uint32_t)0x080e3800) /* Base @ of Page 455, 2 Kbytes */
#define ADDR_FLASH_PAGE_456   ((uint32_t)0x080e4000) /* Base @ of Page 456, 2 Kbytes */
#define ADDR_FLASH_PAGE_457   ((uint32_t)0x080e4800) /* Base @ of Page 457, 2 Kbytes */
#define ADDR_FLASH_PAGE_458   ((uint32_t)0x080e5000) /* Base @ of Page 458, 2 Kbytes */
#define ADDR_FLASH_PAGE_459   ((uint32_t)0x080e5800) /* Base @ of Page 459, 2 Kbytes */
#define ADDR_FLASH_PAGE_460   ((uint32_t)0x080e6000) /* Base @ of Page 460, 2 Kbytes */
#define ADDR_FLASH_PAGE_461   ((uint32_t)0x080e6800) /* Base @ of Page 461, 2 Kbytes */
#define ADDR_FLASH_PAGE_462   ((uint32_t)0x080e7000) /* Base @ of Page 462, 2 Kbytes */
#define ADDR_FLASH_PAGE_463   ((uint32_t)0x080e7800) /* Base @ of Page 463, 2 Kbytes */
#define ADDR_FLASH_PAGE_464   ((uint32_t)0x080e8000) /* Base @ of Page 464, 2 Kbytes */
#define ADDR_FLASH_PAGE_465   ((uint32_t)0x080e8800) /* Base @ of Page 465, 2 Kbytes */
#define ADDR_FLASH_PAGE_466   ((uint32_t)0x080e9000) /* Base @ of Page 466, 2 Kbytes */
#define ADDR_FLASH_PAGE_467   ((uint32_t)0x080e9800) /* Base @ of Page 467, 2 Kbytes */
#define ADDR_FLASH_PAGE_468   ((uint32_t)0x080ea000) /* Base @ of Page 468, 2 Kbytes */
#define ADDR_FLASH_PAGE_469   ((uint32_t)0x080ea800) /* Base @ of Page 469, 2 Kbytes */
#define ADDR_FLASH_PAGE_470   ((uint32_t)0x080eb000) /* Base @ of Page 470, 2 Kbytes */
#define ADDR_FLASH_PAGE_471   ((uint32_t)0x080eb800) /* Base @ of Page 471, 2 Kbytes */
#define ADDR_FLASH_PAGE_472   ((uint32_t)0x080ec000) /* Base @ of Page 472, 2 Kbytes */
#define ADDR_FLASH_PAGE_473   ((uint32_t)0x080ec800) /* Base @ of Page 473, 2 Kbytes */
#define ADDR_FLASH_PAGE_474   ((uint32_t)0x080ed000) /* Base @ of Page 474, 2 Kbytes */
#define ADDR_FLASH_PAGE_475   ((uint32_t)0x080ed800) /* Base @ of Page 475, 2 Kbytes */
#define ADDR_FLASH_PAGE_476   ((uint32_t)0x080ee000) /* Base @ of Page 476, 2 Kbytes */
#define ADDR_FLASH_PAGE_477   ((uint32_t)0x080ee800) /* Base @ of Page 477, 2 Kbytes */
#define ADDR_FLASH_PAGE_478   ((uint32_t)0x080ef000) /* Base @ of Page 478, 2 Kbytes */
#define ADDR_FLASH_PAGE_479   ((uint32_t)0x080ef800) /* Base @ of Page 479, 2 Kbytes */
#define ADDR_FLASH_PAGE_480   ((uint32_t)0x080f0000) /* Base @ of Page 480, 2 Kbytes */
#define ADDR_FLASH_PAGE_481   ((uint32_t)0x080f0800) /* Base @ of Page 481, 2 Kbytes */
#define ADDR_FLASH_PAGE_482   ((uint32_t)0x080f1000) /* Base @ of Page 482, 2 Kbytes */
#define ADDR_FLASH_PAGE_483   ((uint32_t)0x080f1800) /* Base @ of Page 483, 2 Kbytes */
#define ADDR_FLASH_PAGE_484   ((uint32_t)0x080f2000) /* Base @ of Page 484, 2 Kbytes */
#define ADDR_FLASH_PAGE_485   ((uint32_t)0x080f2800) /* Base @ of Page 485, 2 Kbytes */
#define ADDR_FLASH_PAGE_486   ((uint32_t)0x080f3000) /* Base @ of Page 486, 2 Kbytes */
#define ADDR_FLASH_PAGE_487   ((uint32_t)0x080f3800) /* Base @ of Page 487, 2 Kbytes */
#define ADDR_FLASH_PAGE_488   ((uint32_t)0x080f4000) /* Base @ of Page 488, 2 Kbytes */
#define ADDR_FLASH_PAGE_489   ((uint32_t)0x080f4800) /* Base @ of Page 489, 2 Kbytes */
#define ADDR_FLASH_PAGE_490   ((uint32_t)0x080f5000) /* Base @ of Page 490, 2 Kbytes */
#define ADDR_FLASH_PAGE_491   ((uint32_t)0x080f5800) /* Base @ of Page 491, 2 Kbytes */
#define ADDR_FLASH_PAGE_492   ((uint32_t)0x080f6000) /* Base @ of Page 492, 2 Kbytes */
#define ADDR_FLASH_PAGE_493   ((uint32_t)0x080f6800) /* Base @ of Page 493, 2 Kbytes */
#define ADDR_FLASH_PAGE_494   ((uint32_t)0x080f7000) /* Base @ of Page 494, 2 Kbytes */
#define ADDR_FLASH_PAGE_495   ((uint32_t)0x080f7800) /* Base @ of Page 495, 2 Kbytes */
#define ADDR_FLASH_PAGE_496   ((uint32_t)0x080f8000) /* Base @ of Page 496, 2 Kbytes */
#define ADDR_FLASH_PAGE_497   ((uint32_t)0x080f8800) /* Base @ of Page 497, 2 Kbytes */
#define ADDR_FLASH_PAGE_498   ((uint32_t)0x080f9000) /* Base @ of Page 498, 2 Kbytes */
#define ADDR_FLASH_PAGE_499   ((uint32_t)0x080f9800) /* Base @ of Page 499, 2 Kbytes */
#define ADDR_FLASH_PAGE_500   ((uint32_t)0x080fa000) /* Base @ of Page 500, 2 Kbytes */
#define ADDR_FLASH_PAGE_501   ((uint32_t)0x080fa800) /* Base @ of Page 501, 2 Kbytes */
#define ADDR_FLASH_PAGE_502   ((uint32_t)0x080fb000) /* Base @ of Page 502, 2 Kbytes */
#define ADDR_FLASH_PAGE_503   ((uint32_t)0x080fb800) /* Base @ of Page 503, 2 Kbytes */
#define ADDR_FLASH_PAGE_504   ((uint32_t)0x080fc000) /* Base @ of Page 504, 2 Kbytes */
#define ADDR_FLASH_PAGE_505   ((uint32_t)0x080fc800) /* Base @ of Page 505, 2 Kbytes */
#define ADDR_FLASH_PAGE_506   ((uint32_t)0x080fd000) /* Base @ of Page 506, 2 Kbytes */
#define ADDR_FLASH_PAGE_507   ((uint32_t)0x080fd800) /* Base @ of Page 507, 2 Kbytes */
#define ADDR_FLASH_PAGE_508   ((uint32_t)0x080fe000) /* Base @ of Page 508, 2 Kbytes */
#define ADDR_FLASH_PAGE_509   ((uint32_t)0x080fe800) /* Base @ of Page 509, 2 Kbytes */
#define ADDR_FLASH_PAGE_510   ((uint32_t)0x080ff000) /* Base @ of Page 510, 2 Kbytes */
#define ADDR_FLASH_PAGE_511   ((uint32_t)0x080ff800) /* Base @ of Page 511, 2 Kbytes */


#define FLASH_USER_START_ADDR   ADDR_FLASH_PAGE_156   /* Start @ of user Flash area */
#define FLASH_USER_END_ADDR     ADDR_FLASH_PAGE_255 + FLASH_PAGE_SIZE - 1   /* End @ of user Flash area */

#define FLASH_FLAG_PGERR               ((uint32_t)0x00000004)  /*!< FLASH Program error flag */
#define FLASH_FLAG_WRPRTERR            ((uint32_t)0x00000010)  /*!< FLASH Write protected error flag */

/* Flash memory address of physics */
#define STM32_FLASH_ADDR        ((u32)0x08000000)
#define STM32_PAGE_SIZE         ((u32)0x800)//2K

/* Flash  memory parameters */
#define STM_FLASH_SECTOR_SIZE        ((u16)0x200)          /* 512 bytes per sector w/o Spare Area */
#define STM_FLASH_BLOCK_COUNT        ((u16)0x004)
#define STM_FLASH_SECTOR_TOTAL      ((u16)((FLASH_USER_END_ADDR - FLASH_USER_START_ADDR +1)/STM_FLASH_SECTOR_SIZE)) /* (128/4)*7= 224*/

/* Flash memory status */
//#define STM_FLASH_VALID_ADDRESS       ((u32)0x00010000)
#define STM_FLASH_INVALID_ADDRESS       ((u32)0x00020000)
#define STM_FLASH_ERROR                 ((u32)0x00040000)
#define STM_FLASH_SUCCESS               ((u32)0x00080000)

/* Flash memory data computation */
#define DATA_1st(DATA)       (u8)((DATA) & 0x000000FF)          /* 1st data byte */
#define DATA_2nd(DATA)       (u8)(((DATA)& 0x0000FF00) >> 8)    /* 2nd data byte */
#define DATA_3rd(DATA)       (u8)(((DATA)& 0x00FF0000) >> 16)   /* 3rd data byte */
#define DATA_4th(DATA)       (u8)(((DATA)& 0xFF000000) >> 24)   /* 4th data byte */

/* Exported functions ------------------------------------------------------- */
u32 Flash_WriteSector(u8 *pBuffer, DWORD uwSector, u32 ulNumSectorWrite);
u32 Flash_ReadSector (u8 *pBuffer, DWORD uwSector, u32 ulNumSectorRead);
u32 Flash_EraseBlock(u32 BlockNum);

DSTATUS TM_FATFS_FLASH_disk_initialize(void);
DSTATUS TM_FATFS_FLASH_disk_status(void);
DRESULT TM_FATFS_FLASH_disk_ioctl(BYTE cmd, char *buff);
DRESULT TM_FATFS_FLASH_disk_read(BYTE *buff, DWORD sector, UINT count);
DRESULT TM_FATFS_FLASH_disk_write(BYTE *buff, DWORD sector, UINT count);

uint32_t GetSectorSize(uint32_t Sector);

void Fill_Buffer(u8 *pBuffer, u16 BufferLenght, u32 Offset);


#endif /* __LOS_BSP_FLASH_H */

