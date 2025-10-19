section .text
	extern	printf

printi:
	mov	rsi, rdi
	xor	eax, eax
	mov	edi, printi_s
	jmp	printf

	global identity
identity:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	;
	mov	qword [rbp-8], rdi
	;
	mov	r10, qword [rbp-8]
	mov	rax, r10
	jmp	.L0
	;
.L0:
	add	rsp, 16
	pop	rbp
	ret

	global successor
successor:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	;
	mov	qword [rbp-8], rdi
	;
	mov	r10, qword [rbp-8]
	mov	r11, 1
	add	r10, r11
	mov	rax, r10
	jmp	.L1
	;
.L1:
	add	rsp, 16
	pop	rbp
	ret

	global double
double:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	;
	mov	qword [rbp-8], rdi
	;
	mov	r10, qword [rbp-8]
	mov	r11, 2
	imul	r10, r11
	mov	rax, r10
	jmp	.L2
	;
.L2:
	add	rsp, 16
	pop	rbp
	ret

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
	jmp	.L3
	;
.L3:
	add	rsp, 16
	pop	rbp
	ret

	global factorial
factorial:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	;
	mov	qword [rbp-8], rdi
	;
	mov	r10, qword [rbp-8]
	mov	r11, 1
	sub	r10, r11
	lea	r11, [rbp-16]
	mov	qword [r11], r10
	jmp	.L6
.L5:
	mov	r10, qword [rbp-8]
	mov	r11, qword [rbp-16]
	imul	r10, r11
	lea	r11, [rbp-8]
	mov	qword [r11], r10
	mov	r10, qword [rbp-16]
	mov	r11, 1
	sub	r10, r11
	lea	r11, [rbp-16]
	mov	qword [r11], r10
.L6:
	mov	r10, qword [rbp-16]
	mov	r11, 1
	cmp	r10, r11
	setg	r10b
	test	r10b, r10b
	jnz	.L5
	mov	r10, qword [rbp-8]
	mov	rax, r10
	jmp	.L4
	;
.L4:
	add	rsp, 16
	pop	rbp
	ret

	global power
power:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 32
	;
	mov	qword [rbp-8], rdi
	;
	mov	r10, qword [rbp-8]
	lea	r11, [rbp-16]
	mov	qword [r11], r10
	mov	r10, 1
	lea	r11, [rbp-24]
	mov	qword [r11], r10
	jmp	.L9
.L8:
	mov	r10, qword [rbp-8]
	mov	r11, qword [rbp-16]
	imul	r10, r11
	lea	r11, [rbp-8]
	mov	qword [r11], r10
	mov	r10, qword [rbp-24]
	mov	r11, 1
	add	r10, r11
	lea	r11, [rbp-24]
	mov	qword [r11], r10
.L9:
	mov	r10, qword [rbp-24]
	mov	r11, qword [rbp-16]
	cmp	r10, r11
	setl	r10b
	test	r10b, r10b
	jnz	.L8
	mov	r10, qword [rbp-8]
	mov	rax, r10
	jmp	.L7
	;
.L7:
	add	rsp, 32
	pop	rbp
	ret

	global main
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 64
	;
	;
	lea	r10, [rel identity]
	lea	r11, [rbp-48]
	mov	r12, 0
	imul	r12, 8
	add	r11, r12
	mov	qword [r11], r10
	lea	r10, [rel successor]
	lea	r11, [rbp-48]
	mov	r12, 1
	imul	r12, 8
	add	r11, r12
	mov	qword [r11], r10
	lea	r10, [rel double]
	lea	r11, [rbp-48]
	mov	r12, 2
	imul	r12, 8
	add	r11, r12
	mov	qword [r11], r10
	lea	r10, [rel square]
	lea	r11, [rbp-48]
	mov	r12, 3
	imul	r12, 8
	add	r11, r12
	mov	qword [r11], r10
	lea	r10, [rel factorial]
	lea	r11, [rbp-48]
	mov	r12, 4
	imul	r12, 8
	add	r11, r12
	mov	qword [r11], r10
	lea	r10, [rel power]
	lea	r11, [rbp-48]
	mov	r12, 5
	imul	r12, 8
	add	r11, r12
	mov	qword [r11], r10
	mov	r10, 5
	lea	r11, [rbp-56]
	mov	qword [r11], r10
	mov	r10, 0
	lea	r11, [rbp-64]
	mov	qword [r11], r10
	jmp	.L12
.L11:
	mov	r10, qword [rbp-56]
	mov	rdi, r10
	lea	r10, [rbp-48]
	mov	r11, qword [rbp-64]
	imul	r11, 8
	add	r10, r11
	mov	r10, qword [r10]
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
	mov	rdi, r10
	call	printi
	mov	r10, qword [rbp-64]
	mov	r11, 1
	add	r10, r11
	lea	r11, [rbp-64]
	mov	qword [r11], r10
.L12:
	mov	r10, qword [rbp-64]
	mov	r11, 6
	cmp	r10, r11
	setl	r10b
	test	r10b, r10b
	jnz	.L11
	mov	r10, 0
	mov	eax, r10d
	jmp	.L10
	;
.L10:
	add	rsp, 64
	pop	rbp
	ret

section .data
	printi_s: db "%ld", 10, 0

section .note.GNU-stack

