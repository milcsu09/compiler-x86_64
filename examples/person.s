section .text
	extern	printf

printi:
	mov	rsi, rdi
	xor	eax, eax
	mov	rdi, printi_s
	jmp	printf

	global	main
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	lea	r10, [rbp-16]
	mov	r11, 0
	imul	r11, 8
	add	r10, r11
	add	r10, 0
	mov	r11, 20
	mov	byte [r10], r11b
	lea	r10, [rbp-16]
	mov	r11, 0
	imul	r11, 8
	add	r10, r11
	add	r10, 4
	add	r10, 0
	mov	r11, 1111
	mov	dword [r10], r11d
	lea	r10, [rbp-16]
	mov	r11, 1
	imul	r11, 8
	add	r10, r11
	add	r10, 0
	mov	r11, 30
	mov	byte [r10], r11b
	lea	r10, [rbp-16]
	mov	r11, 1
	imul	r11, 8
	add	r10, r11
	add	r10, 4
	add	r10, 0
	mov	r11, 2222
	mov	dword [r10], r11d
	lea	r10, [rbp-16]
	mov	r11, 0
	imul	r11, 8
	add	r10, r11
	add	r10, 0
	mov	r10b, byte [r10]
	movzx	r10, r10b
	mov	rdi, r10
	call	printi
	lea	r10, [rbp-16]
	mov	r11, 0
	imul	r11, 8
	add	r10, r11
	add	r10, 4
	add	r10, 0
	mov	r10d, dword [r10]
	mov	rdi, r10
	call	printi
	lea	r10, [rbp-16]
	mov	r11, 1
	imul	r11, 8
	add	r10, r11
	add	r10, 0
	mov	r10b, byte [r10]
	movzx	r10, r10b
	mov	rdi, r10
	call	printi
	lea	r10, [rbp-16]
	mov	r11, 1
	imul	r11, 8
	add	r10, r11
	add	r10, 4
	add	r10, 0
	mov	r10d, dword [r10]
	mov	rdi, r10
	call	printi
	mov	r10, 0
	mov	eax, r10d
	jmp	.L0
.L0:
	add	rsp, 16
	pop	rbp
	ret

section .rodata
	printi_s: db "%ld", 10, 0

section .note.GNU-stack

