section .text
	extern	printf

printi:
	mov	rsi, rdi
	xor	eax, eax
	mov	edi, printi_s
	jmp	printf

	global square
square:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	;
	mov	qword [rbp-8], rdi
	;
	mov	r10, qword [rbp-8]
	mov	r11, qword [rbp-8]
	imul	r10, r11
	mov	rax, r10
	jmp	.L0
	;
.L0:
	add	rsp, 16
	pop	rbp
	ret

	global map
map:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 32
	;
	mov	qword [rbp-8], rdi
	mov	qword [rbp-16], rsi
	mov	qword [rbp-24], rdx
	;
	jmp	.L3
.L2:
	mov	r10, qword [rbp-8]
	mov	r11, qword [rbp-16]
	imul	r11, 8
	add	r10, r11
	mov	r10, qword [r10]
	mov	rdi, r10
	mov	r10, qword [rbp-24]
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
	mov	r10, rax
	mov	r11, qword [rbp-8]
	mov	r12, qword [rbp-16]
	imul	r12, 8
	add	r11, r12
	mov	qword [r11], r10
	mov	r10, qword [rbp-16]
	mov	r11, 1
	sub	r10, r11
	lea	r11, [rbp-16]
	mov	qword [r11], r10
.L3:
	mov	r10, qword [rbp-16]
	mov	r11, 0
	cmp	r10, r11
	setge	r10b
	test	r10b, r10b
	jnz	.L2
	;
.L1:
	add	rsp, 32
	pop	rbp
	ret

	global main
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 96
	;
	;
	mov	r10, 0
	lea	r11, [rbp-88]
	mov	qword [r11], r10
	jmp	.L6
.L5:
	mov	r10, qword [rbp-88]
	mov	r11, 1
	add	r10, r11
	lea	r11, [rbp-80]
	mov	r12, qword [rbp-88]
	imul	r12, 8
	add	r11, r12
	mov	qword [r11], r10
	mov	r10, qword [rbp-88]
	mov	r11, 1
	add	r10, r11
	lea	r11, [rbp-88]
	mov	qword [r11], r10
.L6:
	mov	r10, qword [rbp-88]
	mov	r11, 10
	cmp	r10, r11
	setl	r10b
	test	r10b, r10b
	jnz	.L5
	lea	r10, [rbp-80]
	mov	rdi, r10
	mov	r10, 10
	mov	rsi, r10
	lea	r10, [rel square]
	mov	rdx, r10
	lea	r10, [rel map]
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
	lea	r11, [rbp-88]
	mov	qword [r11], r10
	jmp	.L8
.L7:
	lea	r10, [rbp-80]
	mov	r11, qword [rbp-88]
	imul	r11, 8
	add	r10, r11
	mov	r10, qword [r10]
	mov	rdi, r10
	call	printi
	mov	r10, qword [rbp-88]
	mov	r11, 1
	add	r10, r11
	lea	r11, [rbp-88]
	mov	qword [r11], r10
.L8:
	mov	r10, qword [rbp-88]
	mov	r11, 10
	cmp	r10, r11
	setl	r10b
	test	r10b, r10b
	jnz	.L7
	mov	r10, 0
	mov	eax, r10d
	jmp	.L4
	;
.L4:
	add	rsp, 96
	pop	rbp
	ret

section .data
	printi_s: db "%ld", 10, 0

section .note.GNU-stack

