/**
 * @defgroup stdio Stdio
 * @ingroup libc
 */

#ifndef _STDIO_H
#define _STDIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <features.h>

#define __NEED_FILE
#define __NEED___isoc_va_list
#define __NEED_size_t
#ifdef __LITEOS__
#define __NEED_ldouble_t
#endif

#if __STDC_VERSION__ < 201112L
#define __NEED_struct__IO_FILE
#endif

#if defined(_POSIX_SOURCE) || defined(_POSIX_C_SOURCE) \
 || defined(_XOPEN_SOURCE) || defined(_GNU_SOURCE) \
 || defined(_BSD_SOURCE)
#define __NEED_ssize_t
#define __NEED_off_t
#define __NEED_va_list
#endif

#ifdef __LITEOS__
#define __NEED_int64_t
#endif

#include <bits/alltypes.h>

#ifdef __LITEOS__
#undef NULL
#endif
#ifdef __cplusplus
#define NULL 0L
#else
#define NULL ((void*)0)
#endif

#undef EOF
#define EOF (-1)

#undef SEEK_SET
#undef SEEK_CUR
#undef SEEK_END
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

#define _IOFBF 0
#define _IOLBF 1
#define _IONBF 2

#ifdef __LITEOS__
#define BUFSIZ (8 * 1024)
#else
#define BUFSIZ 1024
#endif
#define FILENAME_MAX 4096
#define FOPEN_MAX 1000
#define TMP_MAX 10000
#define L_tmpnam 20

#ifndef __LITEOS__
typedef union _G_fpos64_t {
	char __opaque[16];
	long long __lldata;
	double __align;
} fpos_t;
#else
typedef off_t fpos_t;
#endif

extern FILE *const stdin;
extern FILE *const stdout;
extern FILE *const stderr;

#define stdin  (stdin)
#define stdout (stdout)
#define stderr (stderr)

/**
 * @ingroup stdio
 *
 * @par Description:
 * The fopen() function shall open the file whose pathname is the string pointed to by pathname, and associates a stream
 * with it.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #FILE* Upon successful completion, fopen() shall return a pointer to the object controlling the stream.\n
 *                Otherwise, a null pointer shall be returned, and errno shall be set to indicate the error.
 *
 * @par Errors
 * <ul>
 * <li><b>EINVAL</b>: The mode argument is not a valid mode.</li>
 * <li><b>EMFILE</b>: The file descriptor is greater than SHRT_MAX</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>stdio.h</li></ul>
 * @see open | fdopen | fclose
 * @since Huawei LiteOS V100R001C00
 */
FILE *fopen(const char *__restrict, const char *__restrict);

/**
 * @ingroup stdio
 *
 * @par Description:
 * The freopen() function opens the file whose name is the string pointed to by path and associates the stream pointed
 * to by stream with it. The original stream (if it exists) is closed.
 *
 * @attention
 * <ul>
 * <li>when the file where is in the target directory mounted by the file system, the func returns NULL.</li>
 * </ul>
 *
 * @retval #FILE* The freopen() function shall return a pointer to a stream, if it completes successfully;\n
 *                otherwise, a null pointer shall be returned and errno set to indicate the error.
 *
 * @par Dependency:
 * <ul><li>stdio.h</li></ul>
 * @see open | fopen | fclose
 * @since Huawei LiteOS V100R001C00
 */
FILE *freopen(const char *__restrict, const char *__restrict, FILE *__restrict);

/**
 * @ingroup stdio
 *
 * @par Description:
 * The fclose() function shall clear the end-of-file and error indicators for the stream to which stream points.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #int The fclose() shall return 0, if it completes successfully;\n
 *             otherwise, it shall return EOF.
 *
 * @par Errors
 * <ul>
 * <li><b>EBADF</b>: The file descriptor underlying stream is not valid.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>stdio.h</li></ul>
 * @see fopen | fflush
 * @since Huawei LiteOS V100R001C00
 */
int fclose(FILE *);

/**
 * @ingroup stdio
 *
 * @par Description:
 * The remove() function shell remove a file or directory.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #int The remove() function shall return 0, if it completes successfully;\n
 *              otherwise, it shall return -1 and set errno to indicate the error.
 *
 * @par Dependency:
 * <ul><li>stdio.h</li></ul>
 * @see unlink | rmdir
 * @since Huawei LiteOS V100R001C00
 */
int remove(const char *);

/**
 * @ingroup stdio
 *
 * @par Description:
 * The rename() function shall change the name of a file.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #int Upon successful completion, the rename() function shall return 0.\n
 *              Otherwise, it shall return -1 and errno shall be set to indicate the error.
 *
 * @par Errors
 * <ul>
 * <li><b>EINVAL</b>: Invalid argument.</li>
 * <li><b>ENOENT</b>: No such file or directory./li>
 * <li><b>EXDEV</b>: Cross-device link.</li>
 * <li><b>ENOSYS</b>: Rename function not implemented.</li>
 * <li><b>EBUSY</b>: Device or resource busy.</li>
 * <li><b>EEXIST</b>: File exists.</li>
 * <li><b>ENXIO</b>: No such device or address.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>stdio.h</li></ul>
 * @see rmdir | unlink
 * @since Huawei LiteOS V100R001C00
 */
int rename(const char *, const char *);

/**
 * @ingroup stdio
 *
 * @par Description:
 * The feof() function shall test the end-of-file indicator for the stream pointed to by stream.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #int The feof() shall return non-zero if and only if the end-of-file indicator is set for stream.
 * @par Dependency:
 * <ul><li>stdio.h</li></ul>
 * @see clearerr | ferror | fopen
 * @since Huawei LiteOS V100R001C00
 */
int feof(FILE *);

/**
 * @ingroup stdio
 *
 * @par Description:
 * The ferror() function shall test the error indicator for the stream pointed to by stream.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #int The ferror() function shall return non-zero if and only if the error indicator is set for stream.
 * @par Dependency:
 * <ul><li>stdio.h</li></ul>
 * @see clearerr | feof | fopen
 * @since Huawei LiteOS V100R001C00
 */
int ferror(FILE *);

/**
 * @ingroup stdio
 *
 * @par Description:
 * If stream points to an output stream or an update stream in which the most recent operation was not input, fflush()
 * shall cause any unwritten data for that stream to be written to the file, [CX] [Option Start]  and the last data
 * modification and last file status change timestamps of the underlying file shall be marked for update.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #int The fflush() function shall return 0, if it completes successfully;\n
 *              otherwise, it shall set the error indicator for the stream, return EOF and set errno to
 *              indicate the error.
 *
 * @par Errors
 * <ul>
 * <li><b>EBADF</b>: The file descriptor underlying stream is not valid.</li>
 * <li><b>ENOBUFS</b>: No buffer space available.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>stdio.h</li></ul>
 * @see sync | write | fclose | fopen
 * @since Huawei LiteOS V100R001C00
 */
int fflush(FILE *);

/**
 * @ingroup stdio
 *
 * @par Description:
 * The clearerr() function shall clear the end-of-file and error indicators for the stream to which stream points.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #void None.
 * @par Dependency:
 * <ul><li>stdio.h</li></ul>
 * @see None
 * @since Huawei LiteOS V100R001C00
 */
void clearerr(FILE *);

/**
 * @ingroup stdio
 *
 * @par Description:
 * The fseek() function shall set the file-position indicator for the stream pointed to by stream.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #int The fseek() function shall return 0, if it completes successfully;\n
 *              otherwise, it shall return -1 and set errno to indicate the error.
 *
 * @par Errors
 * <ul>
 * <li><b>ESPIPE</b>: Illegal seek.</li>
 * <li><b>EINVAL</b>: The whence argument to fseek() was not SEEK_SET, SEEK_END, or SEEK_CUR.</li>
 * <li><b>EBADF</b>: The stream specified is not a seekable stream.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>stdio.h</li></ul>
 * @see fopen | fgetpos | fseeko
 * @since Huawei LiteOS V100R001C00
 */
int fseek(FILE *, long, int);

/**
 * @ingroup stdio
 *
 * @par Description:
 * The ftell() function shall obtain the current value of the file-position indicator for the stream pointed to by stream.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #long The ftell() function shall return the current offset, if it completes successfully;\n
 *               otherwise, it shall return -1 and set errno to indicate the error.
 *
 * @par Errors
 * <ul>
 * <li><b>ESPIPE</b>: Illegal seek.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>stdio.h</li></ul>
 * @see fseek | ftello
 * @since Huawei LiteOS V100R001C00
 */
long ftell(FILE *);

/**
 * @ingroup stdio
 *
 * @par Description:
 * The rewind() function sets the file position indicator for the stream pointed to by stream
 * to the beginning of the file.  It is equivalent to:\n
 *              (void) fseek(stream, 0L, SEEK_SET)
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #void None.
 *
 * @par Dependency:
 * <ul><li>stdio.h</li></ul>
 * @see fseek | fseeko
 * @since Huawei LiteOS V100R001C00
 */
void rewind(FILE *);

/**
 * @ingroup stdio
 *
 * @par Description:
 * The fgetpos() function shall store the current values of the parse state (if any) and file position indicator for the
 * stream pointed to by stream in the object pointed to by pos.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #int The fgetpos() function shall return 0, if it completes successfully;\n
 *              otherwise, it shall return 1 and set errno to indicate the error.
 *
 * @par Errors
 * <ul>
 * <li><b>ESPIPE</b>: Illegal seek.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>stdio.h</li></ul>
 * @see fopen | fsetpos
 * @since Huawei LiteOS V100R001C00
 */
int fgetpos(FILE *__restrict, fpos_t *__restrict);

/**
 * @ingroup stdio
 *
 * @par Description:
 * The fsetpos() function shall set the file position and state indicators for the stream.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #int The fsetpos() function shall return 0, if it completes successfully;\n
 *              otherwise, it shall return -1 and set errno to indicate the error.
 *
 * @par Errors
 * <ul>
 * <li><b>ESPIPE</b>: Illegal seek.</li>
 * <li><b>EINVAL</b>: The whence argument to fseek() was not SEEK_SET, SEEK_END, or SEEK_CUR.</li>
 * <li><b>EBADF</b>: The stream specified is not a seekable stream.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>stdio.h</li></ul>
 * @see fseeko | fgetpos
 * @since Huawei LiteOS V100R001C00
 */
int fsetpos(FILE *, const fpos_t *);

/**
 * @ingroup stdio
 *
 * @par Description:
 * The fread() function shall read into the array pointed to by ptr up to nitems elements whose size is specified by
 * size in bytes, from the stream pointed to by stream.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #size_t Upon successful completion, fread() shall return the number of elements successfully read
 *                 which is less than nitems only if a read error or end-of-file is encountered. \n
 *                 If size or nitems is 0, fread() shall return 0 and the contents of the array
 *                 and the state of the stream remain unchanged.\n
 *                 Otherwise, if a read error occurs, the error indicator for the stream shall be set,
 *                 and errno shall be set to indicate the error.
 *
 * @par Errors
 * <ul>
 * <li><b>EEOF</b>: The end of file has been reached</li>
 * <li><b>EBADF</b>: Bad file number.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>stdio.h</li></ul>
 * @see read | write | feof
 * @since Huawei LiteOS V100R001C00
 */
size_t fread(void *__restrict, size_t, size_t, FILE *__restrict);

/**
 * @ingroup stdio
 *
 * @par Description:
 * The fwrite() function shall write, from the array pointed to by ptr, up to nitems elements whose size is specified by
 * size, to the stream pointed to by stream.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #size_t The fwrite() function shall return the number of elements successfully written,
 *                 which may be less than nitems if a write error is encountered.
 *                 If size or nitems is 0, fwrite() shall return 0 and the state of the stream remains unchanged.
 *                 Otherwise, if a write error occurs, the error indicator for the stream shall be set,
 *                 and errno shall be set to indicate the error.
 *
 * @par Errors
 * <ul>
 * <li><b>EINVAL</b>: Invalid argument.</li>
 * <li><b>EBADF</b>: Bad file number.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>stdio.h</li></ul>
 * @see puts | fwrite | ferror | fopen
 * @since Huawei LiteOS V100R001C00
 */
size_t fwrite(const void *__restrict, size_t, size_t, FILE *__restrict);

/**
 * @ingroup stdio
 *
 * @par Description:
 * the fgetc() function shall obtain the next byte as an unsigned char converted to an int, from the input stream.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #int The fgetc() shall return the next byte from the input stream pointed to by stream
 * @par Dependency:
 * <ul><li>stdio.h</li></ul>
 * @see feof | ferror | fgets | fread | fscanf | getchar | getc | gets
 * @since Huawei LiteOS V100R001C00
 */
int fgetc(FILE *);

/**
 * @ingroup stdio
 *
 * @par Description:
 * The getc() function shell get a byte from a stream.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #int The getc() function return the character read as an unsigned char
 *              cast to an int or EOF on end of file or error.
 *
 * @par Dependency:
 * <ul><li>stdio.h</li></ul>
 * @see putc | getchar
 * @since Huawei LiteOS V100R001C00
 */
int getc(FILE *);

/**
 * @ingroup stdio
 *
 * @par Description:
 * The getchar() function shell get a byte from a stdin stream.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #int The getchar() function return the character read as an unsigned char
 *              cast to an int or EOF on end of file or error.
 *
 * @par Dependency:
 * <ul><li>stdio.h</li></ul>
 * @see putchar | getc
 * @since Huawei LiteOS V100R001C00
 */
int getchar(void);

/**
 * @ingroup stdio
 *
 * @par Description:
 *  The ungetc() function shall push the byte specified by c (converted to an unsigned char)
 *  back onto the input stream pointed to by stream.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #int Upon successful completion, ungetc() shall return the byte pushed back after conversion.\n
 *              Otherwise, it shall return EOF.
 *
 * @par Dependency:
 * <ul><li>stdio.h</li></ul>
 * @see fseek | getc | fsetpos
 * @since Huawei LiteOS V100R001C00
 */
int ungetc(int, FILE *);

/**
 * @ingroup stdio
 *
 * @par Description:
 * The fputc() function shall write the byte (converted to an unsigned char) to the output stream.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #int The fputc() shall return the value it has written. Otherwise, it shall return EOF.
 * @par Dependency:
 * <ul><li>stdio.h</li></ul>
 * @see fprintf | fscanf | fputs
 * @since Huawei LiteOS V100R001C00
 */
int fputc(int, FILE *);

/**
 * @ingroup stdio
 *
 * @par Description:
 * The putc() function shell put a byte from a stream.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #int The putc() function return the character written as an unsigned char
 *              cast to an int or EOF on end of file or error.
 *
 * @par Dependency:
 * <ul><li>stdio.h</li></ul>
 * @see putchar | getc
 * @since Huawei LiteOS V100R001C00
 */
int putc(int, FILE *);

/**
 * @ingroup stdio
 *
 * @par Description:
 * The putchar() function shell put a byte from a stdin stream.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #int The putchar() function return the character written as an unsigned char
 *              cast to an int or EOF on end of file or error.
 *
 * @par Dependency:
 * <ul><li>stdio.h</li></ul>
 * @see getchar | putc
 * @since Huawei LiteOS V100R001C00
 */
int putchar(int);

/**
 * @ingroup stdio
 *
 * @par Description:
 * The fgets() function shall read bytes from stream into the array pointed to by s until n-1 bytes are read, or a <
 * newline> is read and transferred to s, or an end-of-file condition is encountered.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #char* Upon successful completion, fgets() shall return s. If the stream is at end-of-file,
 * the end-of-file indicator for the stream shall be set and fgets() shall return a null pointer.
 * If a read error occurs, the error indicator for the stream shall be set,
 * fgets() shall return a null pointer,  and shall set errno to indicate the error.
 *
 * @par Errors
 * <ul>
 * <li><b>EINVAL</b>: Invalid argument.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>stdio.h</li></ul>
 * @see fgetc | fopen | fread | fscanf | getc
 * @since Huawei LiteOS V100R001C00
 */
char *fgets(char *__restrict, int, FILE *__restrict);
#if __STDC_VERSION__ < 201112L
/**
 * @ingroup stdio
 *
 * @par Description:
 * The gets() function shall get a string from a stdin stream.
 *
 * @attention
 * <ul>
 * <li>gets is unsafe, use fgets instead.</li>
 * </ul>
 *
 * @retval #char* Upon successful completion, gets() shall return s; otherwise, it shall return NULL;
 *
 * @par Dependency:
 * <ul><li>stdio.h</li></ul>
 * @see printf | dprintf | fprintf
 * @since Huawei LiteOS V100R001C00
 */
char *gets(char *);
#endif

/**
 * @ingroup stdio
 *
 * @par Description:
 * The fputs() function shall  writes the string s to stream.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #int The fputs() shall return the 0, if it completes successfully;\n
 *              Otherwise, it shall return EOF.
 *
 * @par Dependency:
 * <ul><li>stdio.h</li></ul>
 * @see fopen | fputc | puts
 * @since Huawei LiteOS V100R001C00
 */
int fputs(const char *__restrict, FILE *__restrict);

/**
 * @ingroup stdio
 *
 * @par Description:
 * The puts() function shell put a string on standard output.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #int Upon successful completion, puts() shall return 0. Otherwise, it shall return EOF, shall set an
 *              error indicator for the stream, and errno shall be set to indicate the error.
 *
 * @par Errors
 * <ul>
 * <li><b>EBADF</b>: Bad file number.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>stdio.h</li></ul>
 * @see fputs | putc
 * @since Huawei LiteOS V100R001C00
 */
int puts(const char *);

/**
 * @ingroup stdio
 *
 * @par Description:
 * The printf() function shall place output on the stdout stream.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #int The printf() function shall return the number of characters printed.
 *
 * @par Dependency:
 * <ul><li>stdio.h</li></ul>
 * @see snprintf | dprintf | fprintf
 * @since Huawei LiteOS V100R001C00
 */
int printf(const char *__restrict, ...);

/**
 * @ingroup stdio
 *
 * @par Description:
 * The fprintf() function shall place output on the named output stream.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #int The fprintf() functions shall return the number of bytes transmitted.
 * @par Dependency:
 * <ul><li>stdio.h</li></ul>
 * @see fputc | fscanf
 * @since Huawei LiteOS V100R001C00
 */
int fprintf(FILE *__restrict, const char *__restrict, ...);

/**
 * @ingroup stdio
 *
 * @par Description:
 * The sprintf() function shall write to the string according to the format.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #int The sprintf() function shall return the number of characters printed.
 *
 * @par Dependency:
 * <ul><li>stdio.h</li></ul>
 * @see printf | dprintf | fprintf
 * @since Huawei LiteOS V100R001C00
 */
int sprintf(char *__restrict, const char *__restrict, ...);

/**
 * @ingroup stdio
 *
 * @par Description:
 * The snprintf() function shall write at most size bytes to the string.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #int The snprintf() function shall return the number of characters printed.
 *
 * @par Dependency:
 * <ul><li>stdio.h</li></ul>
 * @see printf | dprintf | fprintf
 * @since Huawei LiteOS V100R001C00
 */
int snprintf(char *__restrict, size_t, const char *__restrict, ...);

/**
 * @ingroup stdio
 *
 * @par Description:
 * The vprintf() function shall format output of a stdarg argument list to the stdout.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #int The vprintf() function shall return the number of characters printed.
 *
 * @par Dependency:
 * <ul><li>stdio.h</li></ul>
 * @see printf | dprintf | fprintf
 * @since Huawei LiteOS V100R001C00
 */
int vprintf(const char *__restrict, __isoc_va_list);

/**
 * @ingroup stdio
 *
 * @par Description:
 * The vfprintf() function shall format output of a stdarg argument list to the named file descriptor.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #int The vfprintf() function shall return the number of characters printed.
 *
 * @par Dependency:
 * <ul><li>stdio.h</li></ul>
 * @see printf | dprintf | fprintf
 * @since Huawei LiteOS V100R001C00
 */
int vfprintf(FILE *__restrict, const char *__restrict, __isoc_va_list);

/**
 * @ingroup stdio
 *
 * @par Description:
 * The vsprintf() function shall format output of a stdarg argument list to the string.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #int The vsprintf() function shall return the number of characters printed.
 *
 * @par Dependency:
 * <ul><li>stdio.h</li></ul>
 * @see printf | dprintf | fprintf
 * @since Huawei LiteOS V100R001C00
 */
int vsprintf(char *__restrict, const char *__restrict, __isoc_va_list);

/**
 * @ingroup stdio
 *
 * @par Description:
 * The vsnprintf() function shall format output of a stdarg argument list to the string at most size bytes.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #int The vsnprintf() function shall return the number of characters printed.
 *
 * @par Dependency:
 * <ul><li>stdio.h</li></ul>
 * @see printf | dprintf | fprintf
 * @since Huawei LiteOS V100R001C00
 */
int vsnprintf(char *__restrict, size_t, const char *__restrict, __isoc_va_list);

/**
 * @ingroup stdio
 *
 * @par Description:
 *  The scanf() function scans shall read from the stdin stream
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #int On success, these functions return the number of input items successfully matched and assigned;\n
 *              this can be fewer than provided for, or even zero, in the event of an early matching failure.\n
 *              The  value  EOF is returned if the end of input is reached
 *              before either the first successful conversion or a matching failure  occurs.
 *
 * @par Dependency:
 * <ul><li>stdio.h</li></ul>
 * @see fprintf | getc | scanf
 * @since Huawei LiteOS V100R001C00
 */
int scanf(const char *__restrict, ...);

/**
 * @ingroup stdio
 *
 * @par Description:
 *  The fscanf() function scans shall read from the named input stream.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #int On success, these functions return the number of input items successfully matched and assigned;\n
 *              this can be fewer than provided for, or even zero, in the event of an early matching failure.\n
 *              The  value  EOF is returned if the end of input is reached
 *              before either the first successful conversion or a matching failure  occurs.
 *
 * @par Dependency:
 * <ul><li>stdio.h</li></ul>
 * @see fprintf | getc | scanf
 * @since Huawei LiteOS V100R001C00
 */
int fscanf(FILE *__restrict, const char *__restrict, ...);

/**
 * @ingroup stdio
 *
 * @par Description:
 *  The scanf() function scans shall read from the named string.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #int On success, these functions return the number of input items successfully matched and assigned;\n
 *              this can be fewer than provided for, or even zero, in the event of an early matching failure.\n
 *              The  value  EOF is returned if the end of input is reached
 *              before either the first successful conversion or a matching failure  occurs.
 *
 * @par Dependency:
 * <ul><li>stdio.h</li></ul>
 * @see fprintf | getc | scanf
 * @since Huawei LiteOS V100R001C00
 */
int sscanf(const char *__restrict, const char *__restrict, ...);

/**
 * @ingroup stdio
 *
 * @par Description:
 *  The vscanf() function scans a variable argument list from the standard input.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #int On success, the vscanf() function return the number of input items successfully matched and assigned;\n
 * this can be fewer than provided for, or even zero, in the event of an early matching failure.
 *
 * @par Dependency:
 * <ul><li>stdio.h</li></ul>
 * @see vfscanf | getc | fprintf
 * @since Huawei LiteOS V100R001C00
 */
int vscanf(const char *__restrict, __isoc_va_list);

/**
 * @ingroup stdio
 *
 * @par Description:
 *  The vfscanf() function scans a variable argument list from the named input stream.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #int On success, these functions return the number of input items successfully matched and assigned;\n
 *              this can be fewer than provided for, or even zero, in the event of an early matching failure.\n
 *              The  value  EOF is returned if the end of input is reached
 *              before either the first successful conversion or a matching failure  occurs.
 *
 * @par Dependency:
 * <ul><li>stdio.h</li></ul>
 * @see vscanf | getc | fprintf
 * @since Huawei LiteOS V100R001C00
 */
int vfscanf(FILE *__restrict, const char *__restrict, __isoc_va_list);
int vsscanf(const char *__restrict, const char *__restrict, __isoc_va_list);

/**
 * @ingroup stdio
 *
 * @par Description:
 * The perror() function shell write error messages to standard error.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #void None.
 *
 * @par Dependency:
 * <ul><li>stdio.h</li></ul>
 * @see fprintf | fputc
 * @since Huawei LiteOS V100R001C00
 */
void perror(const char *);

/**
 * @ingroup stdio
 *
 * @par Description:
 * The setvbuf() function may be used after the stream pointed to by stream
 * is associated with an open file but before any other operation
 * (other than an unsuccessful call to setvbuf()) is performed on the stream.
 *
 * @attention
 * <ul>
 * <li>when buf is NULL, the default requested memory size is BUFSIZ.</li>
 * </ul>
 *
 * @retval #int Upon successful completion, setvbuf() shall return 0.\n
 *              Otherwise, it shall return EOF if an invalid value
 *              is given for type or if the request cannot be honored,
 *              and may set errno to indicate the error.
 *
 * @par Errors
 * <ul>
 * <li><b>EINVAL</b>: Invalid argument.</li>
 * <li><b>ENOBUFS</b>: No buffer space available.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>stdio.h</li></ul>
 * @see setbuf | fopen
 * @since Huawei LiteOS V100R001C00
 */
int setvbuf(FILE *__restrict, char *__restrict, int, size_t);

/**
 * @ingroup stdio
 *
 * @par Description:
 * The setvbuf() function assign buffering to a stream, and is exactly equivalent to the call\n
 *           setvbuf(stream, buf, buf ? _IOFBF : _IONBF, BUFSIZ);
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #void None.
 *
 * @par Errors
 * <ul>
 * <li><b>EINVAL</b>: Invalid argument.</li>
 * <li><b>ENOBUFS</b>: No buffer space available.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>stdio.h</li></ul>
 * @see setvbuf | fopen
 * @since Huawei LiteOS V100R001C00
 */
void setbuf(FILE *__restrict, char *__restrict);

/**
 * @ingroup stdio
 *
 * @par Description:
 * The tmpnam() function shall generate a string that is a valid pathname that does not name an existing file.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #char* Upon successful completion, tmpnam() shall return a pointer to a string. If no suitable string can be generated, the
 *                tmpnam() function shall return a null pointer.
 *
 * @par Dependency:
 * <ul><li>stdio.h</li></ul>
 * @see fopen | mkstemp
 * @since Huawei LiteOS V100R001C00
 */
char *tmpnam(char *);
FILE *tmpfile(void);

#if defined(_POSIX_SOURCE) || defined(_POSIX_C_SOURCE) \
 || defined(_XOPEN_SOURCE) || defined(_GNU_SOURCE) \
 || defined(_BSD_SOURCE)
FILE *fmemopen(void *__restrict, size_t, const char *__restrict);
FILE *open_memstream(char **, size_t *);

/**
 * @ingroup stdio
 *
 * @par Description:
 * The fdopen() function shall associate a stream with a file descriptor.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #FILE* The fdopen() function shall return a pointer to a stream, if it completes successfully;\n
 *                otherwise, a null pointer shall be returned and errno set to indicate the error.
 *
 * @par Errors
 * <ul>
 * <li><b>EMFILE</b>: The file descriptor is greater than SHRT_MAX</li>
 * <li><b>EINVAL</b>: The mode argument is not a valid mode.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>stdio.h</li></ul>
 * @see fclose | fopen | open
 * @since Huawei LiteOS V100R001C00
 */
FILE *fdopen(int, const char *);
FILE *popen(const char *, const char *);
int pclose(FILE *);

/**
 * @ingroup stdio
 *
 * @par Description:
 * The fileno() function shall return the integer file descriptor associated with the stream.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #int The fileno() function return the integer value of the file descriptor associated with stream.\n
 *              Otherwise, the value -1 shall be returned and errno set to indicate the error.
 *
 * @par Errors
 * <ul>
 * <li><b>EBADF</b>: The stream is not associated with a file.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>stdio.h</li></ul>
 * @see fdopen | fopen
 * @since Huawei LiteOS V100R001C00
 */
int fileno(FILE *);

/**
 * @ingroup stdio
 *
 * @par Description:
 * The fseeko() function shall set the file-position indicator for the stream pointed to by stream.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #int The fseeko() function shall return 0, if it completes successfully;\n
 *              otherwise, it shall return -1 and set errno to indicate the error.
 *
 * @par Errors
 * <ul>
 * <li><b>ESPIPE</b>: Illegal seek.</li>
 * <li><b>EINVAL</b>: The whence argument to fseek() was not SEEK_SET, SEEK_END, or SEEK_CUR.</li>
 * <li><b>EBADF</b>: The stream specified is not a seekable stream.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>stdio.h</li></ul>
 * @see fopen | fgetpos | fseek
 * @since Huawei LiteOS V100R001C00
 */
int fseeko(FILE *, off_t, int);

/**
 * @ingroup stdio
 *
 * @par Description:
 * The ftello() function shall obtain the current value of the file-position indicator for the stream pointed to by stream.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #off_t The ftello() function shall return the current offset, if it completes successfully;\n
 *                otherwise, it shall return -1 and set errno to indicate the error.
 *
 * @par Errors
 * <ul>
 * <li><b>ESPIPE</b>: Illegal seek.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>stdio.h</li></ul>
 * @see fseeko | ftell
 * @since Huawei LiteOS V100R001C00
 */
off_t ftello(FILE *);
#ifndef __LITEOS__
int dprintf(int, const char *__restrict, ...);
#endif
int vdprintf(int, const char *__restrict, __isoc_va_list);

/**
 * @ingroup stdio
 *
 * @par Description:
 * The flockfile() function shall acquire for a thread ownership of a (FILE *) object.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #void None.
 *
 * @par Dependency:
 * <ul><li>stdio.h</li></ul>
 * @see ftrylockfile | funlockfile
 * @since Huawei LiteOS V100R001C00
 */
void flockfile(FILE *);

/**
 * @ingroup stdio
 *
 * @par Description:
 * The ftrylockfile() function shall acquire for a thread ownership of a (FILE *) object if the object is available;
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #int The ftrylockfile() function shall return zero for success and non-zero to indicate that the lock cannot be acquired.
 *
 * @par Dependency:
 * <ul><li>stdio.h</li></ul>
 * @see flockfile | funlockfile
 * @since Huawei LiteOS V100R001C00
 */
int ftrylockfile(FILE *);

/**
 * @ingroup stdio
 *
 * @par Description:
 * The funlockfile() function shall relinquish the ownership granted to the thread.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #void None.
 *
 * @par Dependency:
 * <ul><li>stdio.h</li></ul>
 * @see flockfile | ftrylockfile
 * @since Huawei LiteOS V100R001C00
 */
void funlockfile(FILE *);

/**
 * @ingroup stdio
 *
 * @par Description:
 * The getc_unlocked() function shell get a byte from a stream without the thread-safe.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #int The getc_unlocked() function return the character read as an unsigned char cast to an int or EOF on end of file or error.
 *
 * @par Dependency:
 * <ul><li>stdio.h</li></ul>
 * @see putc_unlocked | getc
 * @since Huawei LiteOS V100R001C00
 */
int getc_unlocked(FILE *);

/**
 * @ingroup stdio
 *
 * @par Description:
 * The getchar_unlocked() function shell get a byte from a stdin stream without the thread-safe.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #int The getchar_unlocked() function return the character read as an unsigned char cast to an int or EOF on end of file or error.
 *
 * @par Dependency:
 * <ul><li>stdio.h</li></ul>
 * @see putchar_unlocked | getchar
 * @since Huawei LiteOS V100R001C00
 */
int getchar_unlocked(void);
int putc_unlocked(int, FILE *);
int putchar_unlocked(int);
ssize_t getdelim(char **__restrict, size_t *__restrict, int, FILE *__restrict);
ssize_t getline(char **__restrict, size_t *__restrict, FILE *__restrict);
int renameat(int, const char *, int, const char *);
char *ctermid(char *);
#define L_ctermid 20
#endif


#if defined(_XOPEN_SOURCE) || defined(_GNU_SOURCE) \
 || defined(_BSD_SOURCE)
#define P_tmpdir "/tmp"
char *tempnam(const char *, const char *);
#endif

#if defined(_GNU_SOURCE) || defined(_BSD_SOURCE)
#define L_cuserid 20
char *cuserid(char *);
void setlinebuf(FILE *);
void setbuffer(FILE *, char *, size_t);
int fgetc_unlocked(FILE *);
int fputc_unlocked(int, FILE *);
int fflush_unlocked(FILE *);
size_t fread_unlocked(void *, size_t, size_t, FILE *);
size_t fwrite_unlocked(const void *, size_t, size_t, FILE *);
void clearerr_unlocked(FILE *);
int feof_unlocked(FILE *);
int ferror_unlocked(FILE *);
int fileno_unlocked(FILE *);
int getw(FILE *);
int putw(int, FILE *);
char *fgetln(FILE *, size_t *);

/**
 * @ingroup stdio
 *
 * @par Description:
 * The asprintf() function shell allocate a string large enough to hold the output.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #int  The asprintf() function return the number of bytes printed, if it completes successfully;\n
 *               otherwise, it shall return -1.
 *
 * @par Errors
 * <ul>
 * <li><b>ENOMEM</b>: Insufficient storage space is available.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>stdio.h</li></ul>
 * @see malloc | free | printf
 * @since Huawei LiteOS V100R001C00
 */
int asprintf(char **, const char *, ...);

/**
 * @ingroup stdio
 *
 * @par Description:
 * The vasprintf() function shell allocate a string large enough to hold the output.
 *
 * @attention
 * <ul>
 * <li>None.</li>
 * </ul>
 *
 * @retval #int  The vasprintf() function return the number of bytes printed, if it completes successfully;\n
 *               otherwise, it shall return -1.
 *
 * @par Errors
 * <ul>
 * <li><b>ENOMEM</b>: Insufficient storage space is available.</li>
 * </ul>
 *
 * @par Dependency:
 * <ul><li>stdio.h</li></ul>
 * @see malloc | free | printf
 * @since Huawei LiteOS V100R001C00
 */
int vasprintf(char **, const char *, __isoc_va_list);
#endif

#ifdef _GNU_SOURCE
char *fgets_unlocked(char *, int, FILE *);
int fputs_unlocked(const char *, FILE *);

typedef ssize_t (cookie_read_function_t)(void *, char *, size_t);
typedef ssize_t (cookie_write_function_t)(void *, const char *, size_t);
typedef int (cookie_seek_function_t)(void *, off_t *, int);
typedef int (cookie_close_function_t)(void *);

typedef struct _IO_cookie_io_functions_t {
	cookie_read_function_t *read;
	cookie_write_function_t *write;
	cookie_seek_function_t *seek;
	cookie_close_function_t *close;
} cookie_io_functions_t;

FILE *fopencookie(void *, const char *, cookie_io_functions_t);
#endif

#if defined(_LARGEFILE64_SOURCE) || defined(_GNU_SOURCE)
#ifdef __LITEOS__
#define off64_t int64_t
#define fpos64_t fpos_t
FILE *tmpfile64(void);
FILE *fopen64(const char *, const char *);
FILE *freopen64(const char *, const char *, FILE *);
int fseeko64(FILE *, off64_t, int);
off64_t ftello64(FILE *);
int fsetpos64(FILE *, const fpos64_t *);
#else
#define tmpfile64 tmpfile
#define fopen64 fopen
#define freopen64 freopen
#define fseeko64 fseeko
#define ftello64 ftello
#define fgetpos64 fgetpos
#define fsetpos64 fsetpos
#define fpos64_t fpos_t
#define off64_t off_t
#endif
#endif

#ifdef __cplusplus
}
#endif

#endif
