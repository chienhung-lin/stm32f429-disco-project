#!/usr/bin/env bash
set -euxo pipefail
TARGET=$1

qemu-system-gnuarmeclipse \
	-cpu cortex-m4 \
	-machine STM32F429I-Discovery \
	-gdb tcp::3333 \
	-nographic \
	-image "${TARGET}"
