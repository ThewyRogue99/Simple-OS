[org 0x7c00]

mov [BOOT_DRIVE], dl

mov bp, 0x8000
mov sp, bp

mov bx, 0x1000
mov dh, 3
mov dl, [BOOT_DRIVE]
call disk_load
mov al, [0x1001]
call print_hex
jmp $

%include "inout.asm"
%include "disk_load.asm"

BOOT_DRIVE: db 0

HELLO_STRING:
    db 'Hello, World!', 0
OS_INFO:
    db 'This is my new simple OS', 0

times 510 -($-$$) db 0

dw 0xaa55

times 256 dw 0x4F4C
times 256 dw 0x4242
