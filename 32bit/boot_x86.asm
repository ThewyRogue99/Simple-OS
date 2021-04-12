[bits 32]
BEGIN_PM:
	pusha
	mov ebx, MSG_PROT_MODE
	mov edx, 0xb8000 + 9 * LINE_LENGTH
	call print_string_pm

	call KERNEL_OFFSET	

	popa
	ret


MSG_PROT_MODE: db "Currently in x86 (No kernel)", 0
LINE_LENGTH equ 0xA0
