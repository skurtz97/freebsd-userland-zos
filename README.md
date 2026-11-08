# freebsd-userland-zos

FreeBSD userland prepared for z/OS UNIX. The tree is imported from
FreeBSD `release/15.1.0` and laid out like the FreeBSD source monorepo:
upstream programs under `bin/` and `usr.bin/`, upstream third-party
sources under `contrib/`, and their build glue under `lib/`.

This repository is the source for the `vendor/freebsd` submodule of
`freebsd-userland-zos-port`. It provides the pristine FreeBSD `bin/sh`
used as the benchmark reference and the FreeBSD `libedit` that fsh links
against for line editing and history. It is not installed by fsh.

The goal is a FreeBSD userland port for z/OS, starting with `bin/sh`.

## Layout

| Path | Contents |
| ---- | -------- |
| `bin/sh` | FreeBSD shell |
| `bin/kill`, `bin/test` | utilities built with the shell and used by its tests |
| `usr.bin/printf` | source of the shell's `printf` builtin |
| `contrib/libedit` | unmodified FreeBSD `contrib/libedit` |
| `lib/libedit` | FreeBSD build glue for `contrib/libedit` |
| `compat/` | local BSD/libc shims needed off FreeBSD |
| `patches/` | local changes, currently the Linux build adapter |

Only `compat/` and `patches/` are local code. Everything under `bin/`,
`usr.bin/`, `contrib/` and `lib/` comes from FreeBSD at the pin below.

## Upstream pin

| Part | Value |
| ---- | ----- |
| Upstream | <https://github.com/freebsd/freebsd-src> |
| Branch | `release/15.1.0` |
| Commit | `96841ea08dcfa84b954a32dc5ae1a26c28966cf4` |

The imported subtrees are `bin/sh`, `bin/kill`, `bin/test`,
`usr.bin/printf`, `contrib/libedit`, and `lib/libedit`. The parser,
evaluator, builtin implementations and libedit are upstream; the Linux
build adapter is the only local code.

`contrib/libedit` was originally a separate repository. Its history was
merged into this tree under `contrib/libedit`, so `git log --follow` on
an imported file still reaches the original libedit import.

## Linux adapter

`patches/0001-linux-build.patch` changes two things for a Linux host:

1. `mktokens` and `mkbuiltins` use a GNU-compatible `mktemp` template.
2. `expand.c` computes `d_namlen` with `strlen(d_name)`; Linux
   `struct dirent` has no FreeBSD `d_namlen` field.

`compat/` supplies the BSD/FreeBSD libc pieces Linux lacks:

| File | Origin |
| ---- | ------ |
| `compat/bsd.h` | fsh-written shim for FreeBSD/BSD typedefs and libc extensions |
| `compat/btemp.c` | fsh-written signal-name table, `sig2str`/`str2sig`, and `fwopen` via `fopencookie` |
| `compat/setmode.c` | OpenBSD `setmode.c` (same UCB BSD licence as FreeBSD's copy) |
| `compat/strlcpy.c` | OpenBSD `strlcpy`/`strlcat` implementation |

FreeBSD's `filecomplete.h` is a libedit extension that Linux's system
libedit does not ship. The benchmark build compiles `bin/sh` on its own
with `-DNO_HISTORY`, so line editing and `fc`/`bind` are absent from
that reference shell. The non-interactive benchmark workloads do not use
them. fsh's own CMake build links the vendored `contrib/libedit`, so it
keeps them.

## Build

The reference shell is built from fsh:

```sh
./bench/build-refs.sh --only freebsd-sh
```

or through CMake:

```sh
cmake --build build --target refshells
```

The result is `build-refs/bin/freebsd-sh`.

The FreeBSD `Makefile`s are kept in their upstream locations for
reference and for a FreeBSD `bmake` build. The Linux adapter and fsh's
CMake build do not use them.

## z/OS status

The z/OS port has not started in this repository yet. It is the place
for that work: spawnp-first process creation, EBCDIC/charset handling,
tag-aware redirection, and the rest of the FreeBSD `bin/sh` port belong
here, not in fsh's main tree.

## Licence

BSD-3-Clause / BSD-2-Clause, per file. See the SPDX headers in the
imported FreeBSD sources.
