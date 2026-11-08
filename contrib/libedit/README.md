# freebsd-userland-zos-port-libedit

FreeBSD's `contrib/libedit`, vendored for the z/OS userland port and
used by fsh for line editing and history.

This is the exact libedit source FreeBSD `bin/sh` builds against at the
FreeBSD `release/15.1.0` pin. It includes FreeBSD's file-completion
extension (`filecomplete.h` / `filecomplete.c`), which the portable
NetBSD mirror and most system libedit packages do not.

## Upstream pin

| Part | Value |
| ---- | ----- |
| Upstream | <https://github.com/freebsd/freebsd-src> |
| Branch | `release/15.1.0` |
| Commit | `96841ea08dcfa84b954a32dc5ae1a26c28966cf4` |
| Subtree | `contrib/libedit` |

## Status

The source is imported unchanged. The FreeBSD `Makefile` is kept for
reference; the fsh build uses CMake. Portability shims for Linux and
z/OS, and the generated `vi.h`/`emacs.h`/`common.h`/`fcns.h`/`func.h`/
`help.h` files, belong in the build integration.

## Licence

BSD-3-Clause / BSD-2-Clause, per file. See `ChangeLog`, `editline.3`
and the SPDX headers.
