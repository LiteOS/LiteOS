#ifndef __JFFS2_MTD_H__
#define __JFFS2_MTD_H__


#include <jffs2_types.h>

#define MTD_ERASE_PENDING	0x01
#define MTD_ERASING		    0x02
#define MTD_ERASE_SUSPEND	0x04
#define MTD_ERASE_DONE		0x08
#define MTD_ERASE_FAILED	0x10

#define MTD_FAIL_ADDR_UNKNOWN -1LL

#define MTD_TYPE_NOR_FLASH      1
#define MTD_TYPE_NAND_FLASH     2
#define MTD_TYPE_SPI_FLASH      3

struct mtd_info {
	u_char type;
	uint32_t flags;
	uint32_t size;	 // Total size of the MTD

	/* "Major" erase size for the device. Naïve users may take this
	 * to be the only erase size available, or may use the more detailed
	 * information below if they desire
	 */
	uint32_t erasesize;
	/* Minimal writable flash unit size. In case of NOR flash it is 1 (even
	 * though individual bits can be cleared), in case of NAND flash it is
	 * one NAND page (or half, or one-fourths of it), in case of ECC-ed NOR
	 * it is of ECC block size, etc. It is illegal to have writesize = 0.
	 * Any driver registering a struct mtd_info must ensure a writesize of
	 * 1 or larger.
	 */
	uint32_t writesize;

	int (*read) (struct mtd_info *mtd, loff_t from, size_t len,
		      size_t *retlen, u_char *buf);
	int (*write) (struct mtd_info *mtd, loff_t to, size_t len,
		       size_t *retlen, const u_char *buf);
	int (*erase) (struct mtd_info *mtd, loff_t from, size_t len);
};


#endif /* __JFFS2_MTD_H__ */
