#!/bin/sh
nasm ../Source/boot.asm -f bin -o boot_sect-x86.bin
cat boot_sect-x86.bin kernel.bin > os-image.bin
