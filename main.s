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
	sub	rsp, 48
	; Program
	mov	r10, 9
	mov	r11d, r10d
	lea	r10, [rbp-24]
	mov	r12, 0
	imul	r12, 4
	add	r10, r12
	mov	dword [r10], r11d
	mov	r10, 8
	mov	r11d, r10d
	lea	r10, [rbp-24]
	mov	r12, 1
	imul	r12, 4
	add	r10, r12
	mov	dword [r10], r11d
	mov	r10, 7
	mov	r11d, r10d
	lea	r10, [rbp-24]
	mov	r12, 2
	imul	r12, 4
	add	r10, r12
	mov	dword [r10], r11d
	mov	r10, 6
	mov	r11d, r10d
	lea	r10, [rbp-24]
	mov	r12, 3
	imul	r12, 4
	add	r10, r12
	mov	dword [r10], r11d
	mov	r10, 5
	mov	r11d, r10d
	lea	r10, [rbp-24]
	mov	r12, 4
	imul	r12, 4
	add	r10, r12
	mov	dword [r10], r11d
	mov	r10, 0
	lea	r11, [rbp-32]
	mov	qword [r11], r10
	jmp	.L1
.L0:
	mov	r10, 0
	lea	r11, [rbp-40]
	mov	qword [r11], r10
	jmp	.L3
.L2:
	lea	r10, [rbp-24]
	mov	r11, [rbp-40]
	imul	r11, 4
	add	r10, r11
	mov	r10d, [r10]
	lea	r11, [rbp-24]
	mov	r12, [rbp-40]
	mov	r13, 1
	add	r12, r13
	imul	r12, 4
	add	r11, r12
	mov	r11d, [r11]
	cmp	r10d, r11d
	setg	r10b
	test	r10b, r10b
	jz	.L4
	lea	r10, [rbp-24]
	mov	r11, [rbp-40]
	imul	r11, 4
	add	r10, r11
	mov	r10d, [r10]
	movsx	r11, r10d
	lea	r10, [rbp-48]
	mov	qword [r10], r11
	lea	r10, [rbp-24]
	mov	r11, [rbp-40]
	mov	r12, 1
	add	r11, r12
	imul	r11, 4
	add	r10, r11
	mov	r10d, [r10]
	lea	r11, [rbp-24]
	mov	r12, [rbp-40]
	imul	r12, 4
	add	r11, r12
	mov	dword [r11], r10d
	mov	r10, [rbp-48]
	mov	r11d, r10d
	lea	r10, [rbp-24]
	mov	r12, [rbp-40]
	mov	r13, 1
	add	r12, r13
	imul	r12, 4
	add	r10, r12
	mov	dword [r10], r11d
.L4:
	mov	r10, [rbp-40]
	mov	r11, 1
	add	r10, r11
	lea	r11, [rbp-40]
	mov	qword [r11], r10
.L3:
	mov	r10, [rbp-40]
	mov	r11, 5
	cmp	r10, r11
	setl	r10b
	test	r10b, r10b
	jnz	.L2
	mov	r10, [rbp-32]
	mov	r11, 1
	add	r10, r11
	lea	r11, [rbp-32]
	mov	qword [r11], r10
.L1:
	mov	r10, [rbp-32]
	mov	r11, 5
	cmp	r10, r11
	setl	r10b
	test	r10b, r10b
	jnz	.L0
	mov	r10, 0
	lea	r11, [rbp-32]
	mov	qword [r11], r10
	jmp	.L6
.L5:
	lea	r10, [rbp-24]
	mov	r11, [rbp-32]
	imul	r11, 4
	add	r10, r11
	mov	r10d, [r10]
	movsx	r11, r10d
	mov	rdi, r11
	call	printi
	mov	r10, [rbp-32]
	mov	r11, 1
	add	r10, r11
	lea	r11, [rbp-32]
	mov	qword [r11], r10
.L6:
	mov	r10, [rbp-32]
	mov	r11, 5
	cmp	r10, r11
	setl	r10b
	test	r10b, r10b
	jnz	.L5
	; Leave
	add	rsp, 48
	pop	rbp
	mov	eax, 0
	ret

section .data
	printi_s: db "%ld", 10, 0

section .note.GNU-stack

