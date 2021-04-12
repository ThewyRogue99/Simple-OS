#!/bin/sh
gcc -m32 -fno-asynchronous-unwind-tables -ffreestanding -c ../Source/kernel.c -o ../Junk/kernel.o

gcc -m32 -fno-asynchronous-unwind-tables -ffreestanding -c ../Source/iolib.c -o ../Junk/iolib.o

nasm ../Source/kernel_entry.asm -f elf -o ../Junk/kernel_entry.o

ld -m elf_i386 -o ../Junk/kernel.elf -Ttext 0x1000 ../Junk/kernel_entry.o ../Junk/kernel.o ../Junk/iolib.o

objcopy -R .note -R .comment -S -O binary ../Junk/kernel.elf kernel.bin
