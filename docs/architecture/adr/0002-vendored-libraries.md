# 0002. Libraries vendored and compiled into the binary

- Status: Accepted
- Date: 2026-10-17

## Context

The server is a 32-bit binary. Most hosts are 64-bit and lack 32-bit
development packages, and admins copy one file to run a server. The zk_libcod
port needed JSON, Speex and TLS.

## Decision

Third-party code is vendored under `src/libcod` (`sqlite`, `yyjson`, `speex`
1.2.1, `mbedtls` 3.6.7) and `src/zlib`, built by the Makefile with the
server, behind `WITH_*` switches. Configuration lives in our own files
(`src/libcod/mbedtls/cod2rev_mbedtls_config.h`); vendored files are not
edited. MySQL is the exception: headers are vendored but `WITH_MYSQL=false`,
since it needs the system client library.

## Consequences

- No runtime dependencies beyond libc, libm, pthread, libdl and libstdc++ on
  Linux; static MinGW on Windows. CI needs no extra packages.
- Library updates are manual: replace the folder, rebuild both platforms,
  run the tests that use it (`scripts/http_test.py` for Mbed TLS).
- Build time and repository size grow.
