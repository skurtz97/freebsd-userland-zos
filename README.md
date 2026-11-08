# freebsd-userland-zos-port-src

FreeBSD `bin/sh` prepared for z/OS UNIX, and the Linux adapter used to
benchmark it against fsh.

This repository is the source for the `vendor/freebsd-sh` submodule
of `freebsd-userland-zos-port`. It is not installed by fsh and is not
part of the fsh binary. The name is deliberately broader than the current contents:
the goal is a FreeBSD userland port for z/OS, starting with `bin/sh`.

## Upstream pin

| Part | Value |
| ---- | ----- |
| Upstream | <https://github.com/freebsd/freebsd-src> |
| Branch | `release/15.1.0` |
| Commit | `96841ea08dcfa84b954a32dc5ae1a26c28966cf4` |

The imported subset is `bin/sh`, `bin/kill`, `bin/test`, and
`usr.bin/printf`. The parser, evaluator, and builtin implementations
are upstream; the Linux build adapter is the only local code.

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

FreeBSD's `filecomplete.h` is a libedit extension Linux's libedit does
not ship, so the benchmark build uses `-DNO_HISTORY`; line editing and
`fc`/`bind` are absent from that build. The non-interactive benchmark
workloads do not use them.

## Build

The build is driven from fsh:

```sh
./bench/build-refs.sh --only freebsd-sh
```

or through CMake:

```sh
cmake --build build --target refshells
```

The result is `build-refs/bin/freebsd-sh`.

## z/OS status

The z/OS port has not started in this repository yet. It is the place
for that work: spawnp-first process creation, EBCDIC/charset handling,
tag-aware redirection, and the rest of the FreeBSD `bin/sh` port belong
here, not in fsh's main tree.

## Licence

BSD-3-Clause / BSD-2-Clause, per file. See the SPDX headers in the
imported FreeBSD sources.
