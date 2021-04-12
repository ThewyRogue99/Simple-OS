[bits 32]

print_string_pm:
	pusha
	mov al, [ebx]
	cmp al, 0
	jne ifbranch_pm
	popa
	ret

ifbranch_pm:
	call print_char_pm
	add ebx, 1
	add edx, 2
	jmp print_string_pm + 1

print_hex_pm:
	pusha
	mov bl, al ; copy al to bl
	mov cl, 0 ; set boolean to check for second digit
	and al, 0xF0 ; this is to get the first 4 bits
	shr al, 4 ; shift bits to right for comparison
	cmp al, 0x09
	jle ifless_eq_pm
	jg ifgreater_pm
return_pm:
	add edx, 2
	mov al, bl ; recover al
	mov cl, 1
	and al, 0x0F
	cmp al, 0x09
	jle ifless_eq_pm
	jg ifgreater_pm

ifless_eq_pm:
	add al, 0x30
	call print_char_pm
	cmp cl, 0 ; check value if jump is necessary
	je return_pm
	popa
	ret

ifgreater_pm:
	add al, 0x37
	call print_char_pm
	cmp cl, 0 ; check value if jump is necessary
	je return_pm
	popa
	ret

print_char_pm:
	pusha
	mov ah, 0x02
	mov [edx], ax
	popa
	ret

clear_screen:
        pusha
	pushad
	mov ebx, 0xb8000
	mov al, 0
	mov ah, 0x00
	mov [ebx], ax
        add ebx, 1
        cmp ebx, 0xb87d0
	jne clear_screen + 7
	popad
	popa
	ret
