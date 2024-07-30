#pragma once

#include "types.h"
#include <fcntl.h>
#include <bits/types/FILE.h>

#ifndef __ATLIB_BUFREAD_SIZE
#define __ATLIB_BUFREAD_SIZE 4096
#endif

/* =========================== bufread_t ================================
 *
 * A Buffer Reader structure. Used to read in buffered data from a file.
 * The data can be either text (ASCII, UTF-8, etc.), or binary data.
 * The user is responsible for decoding the text. This struct only acts 
 * as an intermediate buffer between a user and a file.
 * The internal buffer's size is controlled by the macro __ATLIB_BUFREAD_SIZE,
 * which defaults to '4096'. To act as an unbuffered reader, define this value
 * to '1'.
 *
 * ============================ fields ==================================
 *
 * * FILE * fh: The FILE handler to read from.
 * * isize to_read: The number of unread bytes in the buffer before the next refill.
 * * char * next: The pointer to the next byte to read.
 * * char buf[__ATLIB_BUFREAD_SIZE]: The stack-defined buffer.
 *
 * ========================== notes ====================================
 *
 * A valid Buffered Reader, or `bufread_t` object, is any Buffered Reader
 * that has been initialized (see `open_bufread`).
 * The use of any non-valid Buffered Reader object is undefined behavior.
 *
 * =====================================================================
 */
#ifndef __ATLIB_BUFREAD_T
#define __ATLIB_BUFREAD_T
typedef struct {
    FILE * fh;
    isize to_read;
    usize skip;
    char * next;
    char buf[__ATLIB_BUFREAD_SIZE];
} bufread_t;
#endif

/* ========================== Atlib_bufread_open =============================
 *
 * Initializes a Buffered Reader object into a valid state.
 * Opens the file at the path specified by `file_path` in read-only mode.
 *
 * ========================== parameters ===============================
 *
 * * bufread_t * br: A pointer to a Buffered Reader object.
 * * const char * file_path: The path of the file to open.
 *
 * ========================== returns ==================================
 *
 * The pointer `br` on success, or NULL if there was an error.
 * Currently, there is no way to determine what the error was for.
 * 
 * ========================== notes ====================================
 *
 * This function assumes all pointers are `restrict` and do not alias.
 * This function assumes both pointers are valid.
 * If `br` points to a valid `bufread_t` object, it is overwritten and
 * will leave the associated file dangling. Always properly close a
 * Buffered Reader object (see `close_bufread`).
 *
 * =====================================================================
 */
extern bufread_t * atlib_bufread_open(bufread_t *__restrict br, const char *__restrict file_path);

/* ========================== Atlib_bufread_fopen ============================
 *
 * Initializes a Buffered Reader object from an existing FILE object.
 * This wrapper only performs read operations, so the existing FILE object must
 * have read capabilities and permissions.
 *
 * ========================== parameters ===============================
 *
 * * bufread_t * br: A pointer to a Buffered Reader object.
 * * FILE * file: A pointer to a FILE object.
 *
 * ========================== returns ==================================
 *
 * The pointer `br` on success, or NULL if there was an error.
 * Currently, there is no way to determine what the error was for.
 * 
 * ========================== notes ====================================
 *
 * This function assumes all pointers are `restrict` and do not alias.
 * This function assumes both pointers are valid.
 * If `br` points to a valid `bufread_t` object, it is overwritten and
 * will leave the associated file dangling. Always properly close a
 * Buffered Reader object (see `close_bufread`).
 *
 * =====================================================================
 */
extern bufread_t * atlib_bufread_fopen(bufread_t *__restrict br, FILE *__restrict file);

/* ========================== Atlib_bufread_close ============================
 *
 * Closes and invalidates a Buffered Reader object.
 * If a Buffered Reader object is passed to this function, it cannot
 * be used safely until revalidated again (see `Atlib_bufread_open`).
 *
 * ========================== parameters ===============================
 *
 * * bufread_t * br: A pointer to a valid Buffered Reader object.
 *
 * =====================================================================
 */
extern void atlib_bufread_close(bufread_t * br);

/* ========================== Atlib_bufread_read_u8 ==================================
 *
 * Reads a byte as an `unsigned char`, or `u8`, from the stream.
 * If the buffer is empty, the file is read from to refill the buffer.
 * Here, a stream refers to the buffer and the file, as the file will
 * supply the buffer with more data if the buffer empties.
 *
 * ========================== parameters ===============================
 *
 * * bufread_t * br: A pointer to a valid Buffered Reader object.
 *
 * ========================== returns ==================================
 *
 * The next `unsigned char` in the stream.
 *
 * =====================================================================
 */
extern u8 atlib_bufread_read_u8(bufread_t * br);

/* ========================== Atlib_bufread_read_u16 =================================
 *
 * Reads the next `u16`, from the stream.
 * If the buffer is empty, or does not have enough 
 * information remaining, the file is read from to refill the buffer.
 * The value is read in little-endian.
 *
 * ========================== parameters ===============================
 *
 * * bufread_t * br: A pointer to a valid Buffered Reader object.
 *
 * ========================== returns ==================================
 *
 * The next `unsigned short` in the stream.
 *
 * =====================================================================
 */
extern u16 atlib_bufread_read_u16(bufread_t * br);

/* ========================== Atlib_bufread_read_u32 =================================
 *
 * Reads the next `u32`, from the stream.
 * If the buffer is empty, or does not have enough 
 * information remaining, the file is read from to refill the buffer.
 * The value is read in little-endian.
 *
 * ========================== parameters ===============================
 *
 * * bufread_t * br: A pointer to a valid Buffered Reader object.
 *
 * ========================== returns ==================================
 *
 * The next `unsigned int` in the stream.
 *
 * =====================================================================
 */
extern u32 atlib_bufread_read_u32(bufread_t * br);

/* ========================== Atlib_bufread_read_u64 =================================
 *
 * Reads the next `u64`, from the stream.
 * If the buffer is empty, or does not have enough 
 * information remaining, the file is read from to refill the buffer.
 * The value is read in little-endian.
 *
 * ========================== parameters ===============================
 *
 * * bufread_t * br: A pointer to a valid Buffered Reader object.
 *
 * ========================== returns ==================================
 *
 * The next `unsigned long` in the stream.
 *
 * =====================================================================
 */
extern u64 atlib_bufread_read_u64(bufread_t * br);

/* The following functions are the signed counter-parts 
 * of the above, unsigned, functions. For more information,
 * see the unsigned functions.
 */

/* (See `Atlib_bufread_read_u8` for more information).
 */
extern i8 atlib_bufread_read_i8(bufread_t * br);
/* (See `Atlib_bufread_read_u16` for more information).
 */
extern i16 atlib_bufread_read_i16(bufread_t * br);
/* (See `Atlib_bufread_read_u32` for more information).
 */
extern i32 atlib_bufread_read_i32(bufread_t * br);
/* (See `Atlib_bufread_read_u64` for more information).
 */
extern i64 atlib_bufread_read_i64(bufread_t * br);

/* ========================== Atlib_bufread_skip ===============================
 *
 * Will skip `n` bytes in the stream, throwing them away.
 * If skipping more bytes than what remains in the buffer, `br` will
 * incur a fill call the next time it is read from.
 *
 * If skipping too far, the file is just set to the end, incurring EOF.
 *
 * ========================== parameters ===============================
 *
 * * bufread_t * br: A pointer to a valid Buffered Reader object.
 * * usize n: The number of bytes to skip.
 *
 * =====================================================================
 */
extern void atlib_bufread_skip(bufread_t *, usize);

/* ========================== Atlib_bufread_rewind =============================
 *
 * Will rewind `n` bytes in the stream, essentially "re-reading" them.
 * If the value is smaller than the number of bytes already read in
 * the buffer, no call will be made. Otherwise, `br` will incur a fill
 * call on the next read called on it.
 *
 * If rewinding too far, the file is just set to the start.
 * If the `br` is EOF, it is removed.
 *
 * ========================== parameters ===============================
 *
 * * bufread_t * br: A pointer to a valid Buffered Reader object.
 * * usize n: The number of bytes to rewind.
 *
 * =====================================================================
 */
extern void atlib_bufread_rewind(bufread_t *, usize);

/* ========================== Atlib_bufread_seek ==================================
 *
 * Attempts to seek the buffer to byte `n` of the file, from the
 * beginning of the file. Where `skip_bytes` and `rewind_bytes` are
 * relative to the current position in the file, `n` here is an
 * absolute position in the file, relative to the start of the file.
 *
 * ========================== parameters ===============================
 *
 * * bufread_t * br: A pointer to a valid Buffered Reader object.
 * * usize n: The byte-position of the file to seek to.
 *
 * =====================================================================
 */
extern void atlib_bufread_seek(bufread_t * br, usize n);

extern usize atlib_bufread_pos(const bufread_t * br);
extern usize atlib_bufread_fpos(const bufread_t * br);
extern usize atlib_bufread_err(const bufread_t * br);
extern usize atlib_bufread_eof(const bufread_t * br);
