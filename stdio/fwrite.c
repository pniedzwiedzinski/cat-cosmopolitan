/*-*- mode:c;indent-tabs-mode:nil;c-basic-offset:2;tab-width:8;coding:utf-8 -*-│
│vi: set net ft=c ts=8 sts=2 sw=2 fenc=utf-8                                :vi│
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
#include "libc/calls/calls.h"
#include "libc/calls/struct/iovec.h"
#include "libc/errno.h"
#include "libc/fmt/conv.h"
#include "libc/macros.internal.h"
#include "libc/runtime/runtime.h"
#include "libc/sock/sock.h"
#include "libc/stdio/internal.h"
#include "libc/stdio/stdio.h"
#include "libc/str/str.h"
#include "libc/sysv/consts/o.h"

/**
 * Writes data to stream.
 *
 * @param stride specifies the size of individual items
 * @param count is the number of strides to write
 * @return count on success, [0,count) on EOF, 0 on error or count==0
 */
size_t fwrite(const void *data, size_t stride, size_t count, FILE *f) {
  ldiv_t d;
  ssize_t rc;
  size_t n, m;
  const char *p;
  struct iovec iov[2];
  if ((f->iomode & O_ACCMODE) == O_RDONLY) {
    f->state = errno = EBADF;
    return 0;
  }
  n = stride * count;
  m = f->size - f->beg;
  if (n <= m && f->bufmode != _IONBF) {
    memcpy(f->buf + f->beg, data, n);
    f->beg += n;
    if (f->fd != -1 && f->bufmode == _IOLBF &&
        (p = memrchr(f->buf, '\n', f->beg))) {
      n = p + 1 - f->buf;
      if ((rc = write(f->fd, f->buf, n)) == -1) {
        if (errno == EINTR || errno == EAGAIN) return count;
        f->state = errno;
        return 0;
      }
      n = rc;
      memmove(f->buf, f->buf + n, f->beg - n);
      f->beg -= n;
    }
    return count;
  }
  if (f->fd == -1) {
    n = MIN(n, m);
    d = ldiv(n, stride);
    n -= d.rem;
    memcpy(f->buf + f->beg, data, n);
    f->beg += n;
    f->state = -1;
    return d.quot;
  }
  iov[0].iov_base = f->buf;
  iov[0].iov_len = f->beg;
  iov[1].iov_base = data;
  iov[1].iov_len = n;
  n += f->beg;
  if ((rc = writev(f->fd, iov, 2)) == -1) {
    f->state = errno;
    return 0;
  }
  m = rc;
  if (n != m) abort();
  f->beg = 0;
  return count;
}
