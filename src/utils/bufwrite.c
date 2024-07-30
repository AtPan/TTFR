#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "Atlib/error.h"
#include "Atlib/bufwrite.h"

static usize __flush(bufwrite_t * self) {
    atlib_compassert(self);
    atlib_compassert(self->fh);

    usize i;
    if(ferror(self->fh) || 
            (i = fwrite(self->buf, 1, __ATLIB_BUFWRITE_SIZE - self->to_write, self->fh)) == 0) return 0;

    self->next = self->buf;
    self->to_write = __ATLIB_BUFWRITE_SIZE;
    fflush(self->fh);

    return i;
}

bufwrite_t * atlib_bufwrite_open(bufwrite_t * restrict self, const char * restrict file_path) {
    atlib_compassert(self);

    if((self->fh = fopen(file_path, "a")) == NULL) return NULL;

    self->next = self->buf;
    self->to_write = __ATLIB_BUFWRITE_SIZE;

    return self;
}

bufwrite_t * atlib_bufwrite_fopen(bufwrite_t * restrict self, FILE * restrict file) {
    atlib_compassert(self);
    atlib_compassert(file);

    self->fh = file;

    self->next = self->buf;
    self->to_write = __ATLIB_BUFWRITE_SIZE;

    return self;
}

void atlib_bufwrite_close(bufwrite_t * self) {
    atlib_compassert(self);

    (void)__flush(self);
    fclose(self->fh);
}

usize atlib_bufwrite_flush(bufwrite_t * self) {
    return __flush(self);
}

usize atlib_bufwrite_write(bufwrite_t * restrict self, const void * restrict data, usize n) {
    atlib_compassert(self);
    atlib_compassert(self->fh);
    atlib_compassert(data);

    if(self->to_write < n && __flush(self) == 0) return 0;

    usize i = 0;
    while(n > self->to_write) {
        memcpy(self->next, data + i, self->to_write);
        __flush(self);
        n -= __ATLIB_BUFWRITE_SIZE;
        i += __ATLIB_BUFWRITE_SIZE;
    }

    memcpy(self->next, data + i, n);
    self->next += n;
    self->to_write -= n;
    return n;
}

usize atlib_bufwrite_writef(bufwrite_t * restrict self, const char * restrict fmt, ...) {
    atlib_compassert(self);
    atlib_compassert(self->fh);
    atlib_compassert(fmt);

    usize n = 0;
    char c[2];
    char * m;

    va_list ap, bp;
    va_start(ap, fmt);
    va_copy(bp, ap);

    /* Find the number of bytes required to format the string */
    n = vsnprintf(c, 1, fmt, ap);

    /* If we can fit the string in the current buffer, do so */
    if(n < self->to_write) {
        vsnprintf(self->next, self->to_write, fmt, bp);
    }
    /* If we can fit the string in an empty buffer, flush and do so */
    else if(n < __ATLIB_BUFWRITE_SIZE && __flush(self)) {
        vsnprintf(self->buf, __ATLIB_BUFWRITE_SIZE, fmt, bp);
    }
    /*  If we can allocate enough memory and copy the string over, do so */
    else if((m = malloc(++n))) {
        vsnprintf(m, n, fmt, bp);
        usize i = n;

        /* We know writer is reset from previous condition's `__flush` call */
        while(i > __ATLIB_BUFWRITE_SIZE) {
            memcpy(self->buf, m + (n - i), __ATLIB_BUFWRITE_SIZE);
            self->to_write = 0;
            i -= __ATLIB_BUFWRITE_SIZE;
            __flush(self);
        }
        memcpy(self->next, m + (n - i), i);
        self->to_write -= i;
        self->next += i;

        free(m);

        goto end;
    }
    /* Otherwise, don't modify the writer at all */
    else n = 0;

    self->next += n;
    self->to_write -= n;

end:
    va_end(bp);
    va_end(ap);
    return n;
}

usize atlib_bufwrite_writefv(bufwrite_t * restrict self, const char * restrict fmt, va_list ap) {
    atlib_compassert(self);
    atlib_compassert(self->fh);
    atlib_compassert(fmt);

    va_list bp;
    va_copy(bp, ap);

    usize n = 0;
    char c[2];
    char * m;

    /* Find the number of bytes required to format the string */
    n = vsnprintf(c, 1, fmt, ap);

    /* If we can fit the string in the current buffer, do so */
    if(n < self->to_write) {
        vsnprintf(self->next, self->to_write, fmt, bp);
    }
    /* If we can fit the string in an empty buffer, flush and do so */
    else if(n < __ATLIB_BUFWRITE_SIZE && __flush(self)) {
        vsnprintf(self->buf, __ATLIB_BUFWRITE_SIZE, fmt, bp);
    }
    /*  If we can allocate enough memory and copy the string over, do so */
    else if((m = malloc(++n))) {
        vsnprintf(m, n, fmt, bp);
        usize i = n;

        /* We know writer is reset from previous condition's `__flush` call */
        while(i > __ATLIB_BUFWRITE_SIZE) {
            memcpy(self->buf, m + (n - i), __ATLIB_BUFWRITE_SIZE);
            self->to_write = 0;
            i -= __ATLIB_BUFWRITE_SIZE;
            __flush(self);
        }
        memcpy(self->next, m + (n - i), i);
        self->to_write -= i;
        self->next += i;

        free(m);

        goto end;
    }
    /* Otherwise, don't modify the writer at all */
    else n = 0;

    self->next += n;
    self->to_write -= n;

end:
    va_end(bp);
    return n;
}
