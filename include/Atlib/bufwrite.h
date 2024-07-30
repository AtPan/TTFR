#ifndef __ATLIB_BUFWRITE_H
#define __ATLIB_BUFWRITE_H

#include "Atlib/types.h"
#include <bits/types/FILE.h>

typedef __builtin_va_list va_list;

#ifndef __ATLIB_BUFWRITE_SIZE
#define __ATLIB_BUFWRITE_SIZE 4096
#endif

/* =========================== bufwrite_t ================================
 *
 * A Buffer Writer structure. Used to write buffered data to a file.
 * The data can be either text (ASCII, UTF-8, etc.), or binary data.
 * The user is responsible for ecoding the text. This struct only acts 
 * as an intermediate buffer between a user and a file.
 * The internal buffer's size is controlled by the macro __ATLIB_BUFWRITE_SIZE,
 * which defaults to '4096'. To act as an unbuffered writer, define this value
 * to '1'.
 *
 * ============================ fields ==================================
 *
 * * FILE * fh: The file handler attached to this buffered writer.
 * * isize to_write: The number of bytes currently free in the buffer.
 * * char * next: The pointer to the next byte to write to.
 * * char buf[__ATLIB_BUFWRITE_SIZE]: The stack-defined buffer.
 *
 * ========================== notes ====================================
 *
 * A valid Buffered Writer, or `bufwrite_t` object, is any Buffered Writer
 * that has been initialized (see `open_bufwrite`).
 * The use of any non-valid Buffered Writer object is undefined behavior.
 *
 * =====================================================================
 */
typedef struct {
    FILE * fh;
    isize to_write;
    char * next;
    char buf[__ATLIB_BUFWRITE_SIZE];
} bufwrite_t;

extern bufwrite_t * atlib_bufwrite_open(bufwrite_t *__restrict bw, const char *__restrict file_path);
extern bufwrite_t * atlib_bufwrite_fopen(bufwrite_t *__restrict bw, FILE *__restrict file);
extern void atlib_bufwrite_close(bufwrite_t * bw);

extern usize atlib_bufwrite_flush(bufwrite_t * bw);
extern usize atlib_bufwrite_write(bufwrite_t *__restrict bw, const void *__restrict data, usize n);
extern usize atlib_bufwrite_writef(bufwrite_t *__restrict bw, const char *__restrict fmt, ...) __attribute__ ((format (printf, 2, 3)));
extern usize atlib_bufwrite_writefv(bufwrite_t *__restrict bw, const char *__restrict fmt, va_list ap);

#endif
