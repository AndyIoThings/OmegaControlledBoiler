#!/bin/sh
set -a
BINARY_DIR=binary
BUILD_DIR=build

case "${1:-}" in
  linux)
    CC=g++
    CFLAGS=-O0
    LDFLAGS="-lssl -lcrypto"
    ;;
  mips)
    CC="${STAGING_DIR}/toolchain-mipsel_24kc_gcc-7.3.0_musl/bin/mipsel-openwrt-linux-musl-gcc-7.3.0"
    CFLAGS="-O0 -DSERVER_VERSION=${VERSION} -I${STAGING_DIR}/target-mipsel_24kc_musl/include -I${STAGING_DIR}/target-mipsel_24kc_musl/usr/include"
    LDFLAGS="-L${STAGING_DIR}/target-mipsel_24kc_musl/lib -L${STAGING_DIR}/target-mipsel_24kc_musl/usr/lib -lssl -lcrypto"
    ;;
  clean)
    make clean
    exit 0
    ;;
  *)
    echo "Usage: $0 {linux|mips|clean}" >&2
    exit 1
    ;;
esac

SERVER_NAME="http_server"
BINARY="./${BINARY_DIR}/${SERVER_NAME}"
HTTP_SERVER_ZIP="${SERVER_NAME}_${VERSION}.zip"
TARGET=all
set +a
echo ${CC}
make --trace