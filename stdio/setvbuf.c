/*-*- mode:c;indent-tabs-mode:nil;c-basic-offset:2;tab-width:8;coding:utf-8 -*-│
│vi: set net ft=c ts=2 sts=2 sw=2 fenc=utf-8                                :vi│
╞══════════════════════════════════════════════════════════════════════════════╡
│ Copyright 2020 Justine Alexandra Roberts Tunney                              │
│                                                                              │
│ Permission to use, copy, modify, and/or distribute this software for         │
│ any purpose with or without fee is hereby granted, provided that the         │
│ above copyright notice and this permission notice appear in all copies.      │
│                                                                              │
│ THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL                │
│ WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED                │
│ WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE             │
│ AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL         │
│ DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR        │
│ PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER               │
│ TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR             │
│ PERFORMANCE OF THIS SOFTWARE.                                                │
╚─────────────────────────────────────────────────────────────────────────────*/
#include "libc/stdio/stdio.h"
#include "libc/sysv/errfuns.h"

/**
 * Tunes buffering settings for an stdio stream.
 *
 * @param mode may be _IOFBF, _IOLBF, or _IONBF
 * @param buf may optionally be non-NULL to set the stream's underlying
 *     buffer, which the stream will own, but won't free, otherwise the
 *     existing buffer is used
 * @param size is ignored if buf is NULL
 * @return 0 on success or -1 on error
 */
int setvbuf(FILE *f, char *buf, int mode, size_t size) {
  if (buf) {
    if (!size) size = BUFSIZ;
    if (!f->nofree && f->buf != buf) free_s(&f->buf);
    f->buf = buf;
    f->size = size;
    f->nofree = true;
  }
  f->bufmode = mode;
  return 0;
}