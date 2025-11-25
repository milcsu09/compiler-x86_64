section .text
	extern	printf

printi:
	mov	rsi, rdi
	xor	eax, eax
	mov	rdi, printi_s
	jmp	printf

	extern	printi
	global	main
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 0

	mov	r10, 34
	mov	r11, 35
	add	r10, r11
	mov	rdi, r10
	lea	r10, [rel printi]
	call	r10
	mov	r10, 0
	mov	eax, r10d
	jmp	.L0

.L0:
	add	rsp, 0
	pop	rbp
	ret

section .rodata
	printi_s: db "%ld", 10, 0

section .note.GNU-stack

