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
	sub	rsp, 32

	mov	r10, 10
	lea	r11, [rbp-32]
	mov	r12, 0
	imul	r12, 8
	add	r11, r12
	add	r11, 0
	mov	dword [r11], r10d
	mov	r10, 20
	lea	r11, [rbp-32]
	mov	r12, 0
	imul	r12, 8
	add	r11, r12
	add	r11, 4
	mov	dword [r11], r10d
	mov	r10, 30
	lea	r11, [rbp-32]
	mov	r12, 1
	imul	r12, 8
	add	r11, r12
	add	r11, 0
	mov	dword [r11], r10d
	mov	r10, 40
	lea	r11, [rbp-32]
	mov	r12, 1
	imul	r12, 8
	add	r11, r12
	add	r11, 4
	mov	dword [r11], r10d
	mov	r10, 50
	lea	r11, [rbp-32]
	mov	r12, 2
	imul	r12, 8
	add	r11, r12
	add	r11, 0
	mov	dword [r11], r10d
	mov	r10, 60
	lea	r11, [rbp-32]
	mov	r12, 2
	imul	r12, 8
	add	r11, r12
	add	r11, 4
	mov	dword [r11], r10d
	mov	r10, 70
	lea	r11, [rbp-32]
	mov	r12, 3
	imul	r12, 8
	add	r11, r12
	add	r11, 0
	mov	dword [r11], r10d
	mov	r10, 80
	lea	r11, [rbp-32]
	mov	r12, 3
	imul	r12, 8
	add	r11, r12
	add	r11, 4
	mov	dword [r11], r10d
	lea	r10, [rbp-32]
	mov	r11, 0
	imul	r11, 8
	add	r10, r11
	add	r10, 0
	mov	r10d, dword [r10]
	movsx	r10, r10d
	mov	rdi, r10
	call	printi
	lea	r10, [rbp-32]
	mov	r11, 0
	imul	r11, 8
	add	r10, r11
	add	r10, 4
	mov	r10d, dword [r10]
	movsx	r10, r10d
	mov	rdi, r10
	call	printi
	lea	r10, [rbp-32]
	mov	r11, 1
	imul	r11, 8
	add	r10, r11
	add	r10, 0
	mov	r10d, dword [r10]
	movsx	r10, r10d
	mov	rdi, r10
	call	printi
	lea	r10, [rbp-32]
	mov	r11, 1
	imul	r11, 8
	add	r10, r11
	add	r10, 4
	mov	r10d, dword [r10]
	movsx	r10, r10d
	mov	rdi, r10
	call	printi
	lea	r10, [rbp-32]
	mov	r11, 2
	imul	r11, 8
	add	r10, r11
	add	r10, 0
	mov	r10d, dword [r10]
	movsx	r10, r10d
	mov	rdi, r10
	call	printi
	lea	r10, [rbp-32]
	mov	r11, 2
	imul	r11, 8
	add	r10, r11
	add	r10, 4
	mov	r10d, dword [r10]
	movsx	r10, r10d
	mov	rdi, r10
	call	printi
	lea	r10, [rbp-32]
	mov	r11, 3
	imul	r11, 8
	add	r10, r11
	add	r10, 0
	mov	r10d, dword [r10]
	movsx	r10, r10d
	mov	rdi, r10
	call	printi
	lea	r10, [rbp-32]
	mov	r11, 3
	imul	r11, 8
	add	r10, r11
	add	r10, 4
	mov	r10d, dword [r10]
	movsx	r10, r10d
	mov	rdi, r10
	call	printi
	mov	r10, 0
	mov	eax, r10d
	jmp	.L0

.L0:
	add	rsp, 32
	pop	rbp
	ret

section .rodata
	printi_s: db "%ld", 10, 0

section .note.GNU-stack

