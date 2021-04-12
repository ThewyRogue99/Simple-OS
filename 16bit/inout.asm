print_string:
    pusha
    mov al, [bx]
    cmp al, 0
    jne ifbranch
    popa
    ret

ifbranch:
	call print_char
	add bx, 1
	jmp print_string + 1

print_char:
    pusha
    mov ah, 0x0e
    int 0x10
    popa
    ret

print_newline:
    pusha
    mov ah, 0x0e
    mov al, 0xa
    int 0x10
    mov al, 0xd
    int 0x10
    popa
    ret
