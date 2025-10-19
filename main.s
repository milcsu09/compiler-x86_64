section .text
	extern	printf

printi:
	mov	rsi, rdi
	xor	eax, eax
	mov	edi, printi_s
	jmp	printf

	global bubblesort
bubblesort:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 48
	;
	mov	qword [rbp-8], rdi
	mov	qword [rbp-16], rsi
	;
	mov	r10, 0
	lea	r11, [rbp-24]
	mov	qword [r11], r10
	jmp	.L2
.L1:
	mov	r10, 0
	lea	r11, [rbp-32]
	mov	qword [r11], r10
	jmp	.L4
.L3:
	mov	r10, qword [rbp-8]
	mov	r11, qword [rbp-32]
	imul	r11, 8
	add	r10, r11
	mov	r10, qword [r10]
	mov	r11, qword [rbp-8]
	mov	r12, qword [rbp-32]
	mov	r13, 1
	add	r12, r13
	imul	r12, 8
	add	r11, r12
	mov	r11, qword [r11]
	cmp	r10, r11
	setg	r10b
	test	r10b, r10b
	jz	.L5
	mov	r10, qword [rbp-8]
	mov	r11, qword [rbp-32]
	imul	r11, 8
	add	r10, r11
	mov	r10, qword [r10]
	lea	r11, [rbp-40]
	mov	qword [r11], r10
	mov	r10, qword [rbp-8]
	mov	r11, qword [rbp-32]
	mov	r12, 1
	add	r11, r12
	imul	r11, 8
	add	r10, r11
	mov	r10, qword [r10]
	mov	r11, qword [rbp-8]
	mov	r12, qword [rbp-32]
	imul	r12, 8
	add	r11, r12
	mov	qword [r11], r10
	mov	r10, qword [rbp-40]
	mov	r11, qword [rbp-8]
	mov	r12, qword [rbp-32]
	mov	r13, 1
	add	r12, r13
	imul	r12, 8
	add	r11, r12
	mov	qword [r11], r10
.L5:
	mov	r10, qword [rbp-32]
	mov	r11, 1
	add	r10, r11
	lea	r11, [rbp-32]
	mov	qword [r11], r10
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
	mov	r10, qword [rbp-24]
	mov	r11, 1
	add	r10, r11
	lea	r11, [rbp-24]
	mov	qword [r11], r10
.L2:
	mov	r10, qword [rbp-24]
	mov	r11, qword [rbp-16]
	cmp	r10, r11
	setl	r10b
	test	r10b, r10b
	jnz	.L1
	;
.L0:
	add	rsp, 48
	pop	rbp
	ret

	global main
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 128
	;
	;
	mov	r10, 43
	lea	r11, [rbp-120]
	mov	r12, 0
	imul	r12, 8
	add	r11, r12
	mov	qword [r11], r10
	mov	r10, 60
	lea	r11, [rbp-120]
	mov	r12, 1
	imul	r12, 8
	add	r11, r12
	mov	qword [r11], r10
	mov	r10, 54
	lea	r11, [rbp-120]
	mov	r12, 2
	imul	r12, 8
	add	r11, r12
	mov	qword [r11], r10
	mov	r10, 5
	lea	r11, [rbp-120]
	mov	r12, 3
	imul	r12, 8
	add	r11, r12
	mov	qword [r11], r10
	mov	r10, 0
	lea	r11, [rbp-120]
	mov	r12, 4
	imul	r12, 8
	add	r11, r12
	mov	qword [r11], r10
	mov	r10, 88
	lea	r11, [rbp-120]
	mov	r12, 5
	imul	r12, 8
	add	r11, r12
	mov	qword [r11], r10
	mov	r10, 91
	lea	r11, [rbp-120]
	mov	r12, 6
	imul	r12, 8
	add	r11, r12
	mov	qword [r11], r10
	mov	r10, 15
	lea	r11, [rbp-120]
	mov	r12, 7
	imul	r12, 8
	add	r11, r12
	mov	qword [r11], r10
	mov	r10, 80
	lea	r11, [rbp-120]
	mov	r12, 8
	imul	r12, 8
	add	r11, r12
	mov	qword [r11], r10
	mov	r10, 45
	lea	r11, [rbp-120]
	mov	r12, 9
	imul	r12, 8
	add	r11, r12
	mov	qword [r11], r10
	mov	r10, 26
	lea	r11, [rbp-120]
	mov	r12, 10
	imul	r12, 8
	add	r11, r12
	mov	qword [r11], r10
	mov	r10, 72
	lea	r11, [rbp-120]
	mov	r12, 11
	imul	r12, 8
	add	r11, r12
	mov	qword [r11], r10
	mov	r10, 37
	lea	r11, [rbp-120]
	mov	r12, 12
	imul	r12, 8
	add	r11, r12
	mov	qword [r11], r10
	mov	r10, 15
	lea	r11, [rbp-120]
	mov	r12, 13
	imul	r12, 8
	add	r11, r12
	mov	qword [r11], r10
	mov	r10, 69
	lea	r11, [rbp-120]
	mov	r12, 14
	imul	r12, 8
	add	r11, r12
	mov	qword [r11], r10
	lea	r10, [rbp-120]
	mov	rdi, r10
	mov	r10, 15
	mov	rsi, r10
	lea	r10, [rel bubblesort]
	push	r10
	push	r11
	push	r12
	push	r13
	push	r14
	push	r15
	call	r10
	pop	r15
	pop	r14
	pop	r13
	pop	r12
	pop	r11
	pop	r10
	mov	r10, 0
	lea	r11, [rbp-128]
	mov	qword [r11], r10
	jmp	.L8
.L7:
	lea	r10, [rbp-120]
	mov	r11, qword [rbp-128]
	imul	r11, 8
	add	r10, r11
	mov	r10, qword [r10]
	mov	rdi, r10
	call	printi
	mov	r10, qword [rbp-128]
	mov	r11, 1
	add	r10, r11
	lea	r11, [rbp-128]
	mov	qword [r11], r10
.L8:
	mov	r10, qword [rbp-128]
	mov	r11, 15
	cmp	r10, r11
	setl	r10b
	test	r10b, r10b
	jnz	.L7
	mov	r10, 0
	mov	eax, r10d
	jmp	.L6
	;
.L6:
	add	rsp, 128
	pop	rbp
	ret

section .data
	printi_s: db "%ld", 10, 0

section .note.GNU-stack

