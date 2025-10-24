section .text
	extern	printf

printi:
	mov	rsi, rdi
	xor	eax, eax
	mov	edi, printi_s
	jmp	printf

	global bubble_sort
bubble_sort:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 48
	mov	qword [rbp-8], rdi
	mov	qword [rbp-16], rsi
	lea	r10, [rbp-24]
	mov	r11, 0
	mov	qword [r10], r11
	jmp	.L2
.L1:
	lea	r10, [rbp-32]
	mov	r11, 0
	mov	qword [r10], r11
	jmp	.L4
.L3:
	mov	r10, qword [rbp-8]
	mov	r11, qword [rbp-32]
	imul	r11, 4
	add	r10, r11
	mov	r10d, dword [r10]
	mov	r11, qword [rbp-8]
	mov	r12, qword [rbp-32]
	mov	r13, 1
	add	r12, r13
	imul	r12, 4
	add	r11, r12
	mov	r11d, dword [r11]
	cmp	r10d, r11d
	setg	r10b
	test	r10b, r10b
	jz	.L5
	lea	r10, [rbp-40]
	mov	r11, qword [rbp-8]
	mov	r12, qword [rbp-32]
	imul	r12, 4
	add	r11, r12
	mov	r11d, dword [r11]
	movsx	r11, r11d
	mov	qword [r10], r11
	mov	r10, qword [rbp-8]
	mov	r11, qword [rbp-32]
	imul	r11, 4
	add	r10, r11
	mov	r11, qword [rbp-8]
	mov	r12, qword [rbp-32]
	mov	r13, 1
	add	r12, r13
	imul	r12, 4
	add	r11, r12
	mov	r11d, dword [r11]
	mov	dword [r10], r11d
	mov	r10, qword [rbp-8]
	mov	r11, qword [rbp-32]
	mov	r12, 1
	add	r11, r12
	imul	r11, 4
	add	r10, r11
	mov	r11, qword [rbp-40]
	mov	dword [r10], r11d
.L5:
	lea	r10, [rbp-32]
	mov	r11, qword [rbp-32]
	mov	r12, 1
	add	r11, r12
	mov	qword [r10], r11
.L4:
	mov	r10, qword [rbp-32]
	mov	r11, qword [rbp-16]
	mov	r12, qword [rbp-24]
	sub	r11, r12
	mov	r12, 1
	sub	r11, r12
	cmp	r10, r11
	setl	r10b
	test	r10b, r10b
	jnz	.L3
	lea	r10, [rbp-24]
	mov	r11, qword [rbp-24]
	mov	r12, 1
	add	r11, r12
	mov	qword [r10], r11
.L2:
	mov	r10, qword [rbp-24]
	mov	r11, qword [rbp-16]
	cmp	r10, r11
	setl	r10b
	test	r10b, r10b
	jnz	.L1
.L0:
	add	rsp, 48
	pop	rbp
	ret

	global main
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 32
	lea	r10, [rbp-20]
	mov	r11, 0
	imul	r11, 4
	add	r10, r11
	mov	r11, 42
	mov	dword [r10], r11d
	lea	r10, [rbp-20]
	mov	r11, 1
	imul	r11, 4
	add	r10, r11
	mov	r11, 27
	mov	dword [r10], r11d
	lea	r10, [rbp-20]
	mov	r11, 2
	imul	r11, 4
	add	r10, r11
	mov	r11, 16
	mov	dword [r10], r11d
	lea	r10, [rbp-20]
	mov	r11, 3
	imul	r11, 4
	add	r10, r11
	mov	r11, 5
	mov	dword [r10], r11d
	lea	r10, [rbp-20]
	mov	r11, 4
	imul	r11, 4
	add	r10, r11
	mov	r11, 30
	mov	dword [r10], r11d
	lea	r10, [rbp-20]
	mov	rdi, r10
	mov	r10, 5
	mov	rsi, r10
	lea	r10, [rel bubble_sort]
	call	r10
	lea	r10, [rbp-32]
	mov	r11, 0
	mov	qword [r10], r11
	jmp	.L8
.L7:
	lea	r10, [rbp-20]
	mov	r11, qword [rbp-32]
	imul	r11, 4
	add	r10, r11
	mov	r10d, dword [r10]
	movsx	r10, r10d
	mov	rdi, r10
	call	printi
	lea	r10, [rbp-32]
	mov	r11, qword [rbp-32]
	mov	r12, 1
	add	r11, r12
	mov	qword [r10], r11
.L8:
	mov	r10, qword [rbp-32]
	mov	r11, 5
	cmp	r10, r11
	setl	r10b
	test	r10b, r10b
	jnz	.L7
	mov	r10, 0
	mov	eax, r10d
	jmp	.L6
.L6:
	add	rsp, 32
	pop	rbp
	ret

section .data
	printi_s: db "%ld", 10, 0

section .note.GNU-stack

