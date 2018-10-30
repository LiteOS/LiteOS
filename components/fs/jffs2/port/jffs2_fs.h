/*----------------------------------------------------------------------------
 * Copyright (c) <2016-2018>, <Huawei Technologies Co., Ltd>
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific prior written
 * permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *---------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which might
 * include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
 * applicable export control laws and regulations.
 *---------------------------------------------------------------------------*/

/*
 * JFFS2 -- Journalling Flash File System, Version 2.
 *
 * Copyright (C) 2002-2003 Free Software Foundation, Inc.
 *
 * Created by David Woodhouse <dwmw2@cambridge.redhat.com>
 *
 * For licensing information, see the file 'LICENCE' in this directory.
 *
 *
 */

#ifndef __JFFS2_FS_H__
#define __JFFS2_FS_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#include "jffs2_config.h"

#include <jffs2_types.h>
#include <jffs2_list.h>
#include "jffs2_os.h"


#define DT_UNKNOWN	0
#define DT_DIR		4
#define DT_REG		8


struct inode;
struct super_block;

struct iovec {
    void *iov_base;
    ssize_t iov_len;
};

static inline unsigned int full_name_hash(const unsigned char * name, unsigned int len) {

	unsigned hash = 0;
 	while (len--) {
		hash = (hash << 4) | (hash >> 28);
		hash ^= *(name++);
	}
	return hash;
}

#ifdef CONFIG_JFFS2_WRITABLE
#define jffs2_is_readonly(c) (0)
#else
#define jffs2_is_readonly(c) (1)
#endif

/* NAND flash not currently supported on eCos */
#define jffs2_can_mark_obsolete(c) (1)

#define JFFS2_INODE_INFO(i) (&(i)->jffs2_i)
#define OFNI_EDONI_2SFFJ(f)  ((struct inode *) ( ((char *)f) - ((char *)(&((struct inode *)NULL)->jffs2_i)) ) )

#define JFFS2_F_I_SIZE(f) (OFNI_EDONI_2SFFJ(f)->i_size)
#define JFFS2_F_I_MODE(f) (OFNI_EDONI_2SFFJ(f)->i_mode)
#define JFFS2_F_I_UID(f) (OFNI_EDONI_2SFFJ(f)->i_uid)
#define JFFS2_F_I_GID(f) (OFNI_EDONI_2SFFJ(f)->i_gid)
#define JFFS2_F_I_CTIME(f) (OFNI_EDONI_2SFFJ(f)->i_ctime)
#define JFFS2_F_I_MTIME(f) (OFNI_EDONI_2SFFJ(f)->i_mtime)
#define JFFS2_F_I_ATIME(f) (OFNI_EDONI_2SFFJ(f)->i_atime)

/* FIXME: eCos doesn't hav a concept of device major/minor numbers */
#define JFFS2_F_I_RDEV_MIN(f) ((OFNI_EDONI_2SFFJ(f)->i_rdev)&0xff)
#define JFFS2_F_I_RDEV_MAJ(f) ((OFNI_EDONI_2SFFJ(f)->i_rdev)>>8)

time_t jffs2_get_timestamp(void);
#define get_seconds jffs2_get_timestamp

struct inode
{
	uint32_t		i_ino;

	int			    i_count;
	mode_t			i_mode;
	nlink_t			i_nlink; // Could we dispense with this?
	uid_t			i_uid;
	gid_t			i_gid;
	time_t			i_atime;
	time_t			i_mtime;
	time_t			i_ctime;
//	union {
		unsigned short	i_rdev;     // For devices only
		struct inode *	i_parent;   // For directories only
		off_t		    i_size;     // For files only
//	};
	struct super_block *	i_sb;

	struct jffs2_inode_info	jffs2_i;

    struct inode *		i_cache_prev; // We need doubly-linked?
    struct inode *		i_cache_next;
};

#define JFFS2_SB_INFO(sb) (&(sb)->jffs2_sb)
#define OFNI_BS_2SFFJ(c)  ((struct super_block *) ( ((char *)c) - ((char *)(&((struct super_block *)NULL)->jffs2_sb)) ) )

struct super_block
{
	struct jffs2_sb_info	jffs2_sb;
	struct inode *		    s_root;
    unsigned long		    s_mount_count;

#ifdef CONFIG_JFFS2_GC_THREAD
	jffs2_mutex_t   s_lock;             // Lock the inode cache
	jffs2_flag_t    s_gc_thread_flags;  // Communication with the gcthread
	jffs2_thread_t  s_gc_thread;
#endif
};

#define sleep_on_spinunlock(wq, sl) spin_unlock(sl)
#define EBADFD 32767

/* gcthread.c */
#ifdef CONFIG_JFFS2_GC_THREAD
void jffs2_garbage_collect_trigger(struct jffs2_sb_info *c);
void jffs2_start_garbage_collect_thread(struct jffs2_sb_info *c);
void jffs2_stop_garbage_collect_thread(struct jffs2_sb_info *c);
#else
static inline void jffs2_garbage_collect_trigger(struct jffs2_sb_info *c)
{
	/* We don't have a GC thread in eCos (yet) */
}
#endif

/* fs-ecos.c */
struct inode *jffs2_new_inode (struct inode *dir_i, int mode, struct jffs2_raw_inode *ri);
struct inode *jffs2_iget(struct super_block *sb, uint32_t ino);
void jffs2_iput(struct inode * i);
void jffs2_gc_release_inode(struct jffs2_sb_info *c, struct jffs2_inode_info *f);
struct jffs2_inode_info *jffs2_gc_fetch_inode(struct jffs2_sb_info *c, int inum, int nlink);
unsigned char *jffs2_gc_fetch_page(struct jffs2_sb_info *c, struct jffs2_inode_info *f,
				   unsigned long offset, unsigned long *priv);
void jffs2_gc_release_page(struct jffs2_sb_info *c, unsigned char *pg, unsigned long *priv);

/* Avoid polluting eCos namespace with names not starting in jffs2_ */
#define os_to_jffs2_mode(x) jffs2_from_os_mode(x)
uint32_t jffs2_from_os_mode(uint32_t osmode);
uint32_t jffs2_to_os_mode (uint32_t jmode);

int jffs2_do_mount(struct super_block *jffs2_sb, struct mtd_info *mtd, int flags);
int jffs2_do_umount(struct super_block *jffs2_sb);


/* flashio.c */
int jffs2_flash_read(struct jffs2_sb_info *c, uint32_t read_buffer_offset,
			  const size_t size, size_t * return_size, unsigned char * write_buffer);
int jffs2_flash_write(struct jffs2_sb_info *c, uint32_t write_buffer_offset,
			   const size_t size, size_t * return_size, unsigned char * read_buffer);
int jffs2_flash_direct_writev(struct jffs2_sb_info *c, const struct iovec *vecs,
			      unsigned long count, loff_t to, size_t *retlen);
int jffs2_flash_erase(struct jffs2_sb_info *c, struct jffs2_eraseblock *jeb);

// dir-ecos.c
struct inode *jffs2_lookup(struct inode *dir_i, const unsigned char *name, int namelen);
int jffs2_create(struct inode *dir_i, const unsigned char *d_name, int mode, struct inode **new_i);
int jffs2_mkdir (struct inode *dir_i, const unsigned char *d_name, int mode);
int jffs2_link (struct inode *old_d_inode, struct inode *dir_i, const unsigned char *d_name);
int jffs2_unlink(struct inode *dir_i, struct inode *d_inode, const unsigned char *d_name);
int jffs2_rmdir (struct inode *dir_i, struct inode *d_inode, const unsigned char *d_name);
int jffs2_rename (struct inode *old_dir_i, struct inode *d_inode, const unsigned char *old_d_name,
		  struct inode *new_dir_i, const unsigned char *new_d_name);

/* erase.c */
static inline void jffs2_erase_pending_trigger(struct jffs2_sb_info *c)
{ }

#ifndef CONFIG_JFFS2_FS_WRITEBUFFER
#define SECTOR_ADDR(x) ( ((unsigned long)(x) & ~(c->sector_size-1)) )
#define jffs2_can_mark_obsolete(c) (1)
#define jffs2_cleanmarker_oob(c) (0)
#define jffs2_write_nand_cleanmarker(c,jeb) (-EIO)

#define jffs2_flush_wbuf_pad(c) (c=c)
#define jffs2_flush_wbuf_gc(c, i) ({ (void)(c), (void) i, 0; })
#define jffs2_nand_read_failcnt(c,jeb) do { ; } while(0)
#define jffs2_write_nand_badblock(c,jeb,p) (0)
#define jffs2_flash_setup(c) (0)
#define jffs2_nand_flash_cleanup(c) do {} while(0)
#define jffs2_wbuf_dirty(c) (0)
#define jffs2_flash_writev(a,b,c,d,e,f) jffs2_flash_direct_writev(a,b,c,d,e)
#define jffs2_wbuf_timeout NULL
#define jffs2_wbuf_process NULL
#define jffs2_nor_ecc(c) (0)
#else
#error no nand yet
#endif

#ifndef BUG
#define BUG() do { printf("BUG() at %s %d\n", __FILE__, __LINE__); } while (0)
#endif

#ifndef BUG_ON
#define BUG_ON(x) do { if (unlikely(x)) BUG(); } while(0)
#endif

#define __init


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __JFFS2_FS_H__ */

