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
	sub	rsp, 32
	; Program
	lea	r10, [rbp-32]
	mov	r11, 0
	imul	r11, 1
	add	r10, r11
	mov	rdi, r10
	call	printi
	lea	r10, [rbp-32]
	mov	r11, 1
	imul	r11, 1
	add	r10, r11
	mov	rdi, r10
	call	printi
	lea	r10, [rbp-32]
	mov	r11, 2
	imul	r11, 1
	add	r10, r11
	mov	rdi, r10
	call	printi
	lea	r10, [rbp-32]
	mov	r11, 3
	imul	r11, 1
	add	r10, r11
	mov	rdi, r10
	call	printi
	; Leave
	add	rsp, 32
	pop	rbp
	mov	eax, 0
	ret

section .data
	printi_s: db "%ld", 10, 0

section .note.GNU-stack

