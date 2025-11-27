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
	mov	r10, 20
	lea	r11, [rbp-16]
	mov	r12, 0
	imul	r12, 8
	add	r11, r12
	add	r11, 0
	mov	byte [r11], r10b
	mov	r10, 1111
	lea	r11, [rbp-16]
	mov	r12, 0
	imul	r12, 8
	add	r11, r12
	add	r11, 4
	add	r11, 0
	mov	dword [r11], r10d
	mov	r10, 30
	lea	r11, [rbp-16]
	mov	r12, 1
	imul	r12, 8
	add	r11, r12
	add	r11, 0
	mov	byte [r11], r10b
	mov	r10, 2222
	lea	r11, [rbp-16]
	mov	r12, 1
	imul	r12, 8
	add	r11, r12
	add	r11, 4
	add	r11, 0
	mov	dword [r11], r10d
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
	xor	rax, rax
.L0:
	add	rsp, 16
	pop	rbp
	ret

section .rodata
	printi_s: db "%ld", 10, 0

section .note.GNU-stack

