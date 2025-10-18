section .text
	global	main
	extern	printf

printi:
	mov	rsi, rdi
	xor	eax, eax
	mov	edi, printi_s
	jmp	printf

main:
	; Enter
	push	rbp
	mov	rbp, rsp
	sub	rsp, 0
	; Program
	mov	r10, 0
	mov	r11, 0
	mov	byte [r11], r10b
	; Leave
	add	rsp, 0
	pop	rbp
	mov	eax, 0
	ret

section .data
	printi_s: db "%ld", 10, 0

section .note.GNU-stack

