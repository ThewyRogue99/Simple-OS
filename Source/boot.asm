[org 0x7c00]
[bits 16]

mov [BOOT_DRIVE], dl

mov bp, 0x9000
mov sp, bp

call load_kernel

call switch_to_pm

jmp $

%include "../32bit/switch_to_x86.asm"
[bits 32]
%include "../32bit/inout32.asm"
%include "../32bit/gdt.asm"
%include "../32bit/boot_x86.asm"

[bits 16]
%include "../16bit/disk_load.asm"
%include "../16bit/inout.asm"

load_kernel :
	pusha
	mov bx , MSG_LOAD_KERNEL
	call print_string
	call print_newline
	mov bx , KERNEL_OFFSET ; Store in address
	mov dh , 17 ; read x sectors
	mov dl , [BOOT_DRIVE] ; Select boot drive

	call disk_load
	popa
	ret

;Variables
BOOT_DRIVE: db 0
MSG_LOAD_KERNEL: db "Loading kernel into memory. ", 0

LINE_LENGTH equ 0xA0
KERNEL_OFFSET equ 0x1000

times 510 -($-$$) db 0

dw 0xaa55
