#!/bin/bash
IMAGE_FILE=$1

openocd -f board/stm32f429disc1.cfg \
	-c "init" \
	-c "reset init" \
	-c "targets" \
	-c "flash probe 0" \
	-c "flash info 0" \
	-c "flash write_image erase ${IMAGE_FILE}" \
	-c "verify_image ${IMAGE_FILE}" \
	-c "reset run" \
	-c "shutdown"
