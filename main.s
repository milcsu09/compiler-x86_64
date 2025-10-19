section .text
	extern	printf

printi:
	mov	rsi, rdi
	xor	eax, eax
	mov	edi, printi_s
	jmp	printf

	global fib_iterative
fib_iterative:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 32
	;
	mov	qword [rbp-8], rdi
	;
	mov	r10, 0
	lea	r11, [rbp-16]
	mov	qword [r11], r10
	mov	r10, 1
	lea	r11, [rbp-24]
	mov	qword [r11], r10
	jmp	.L2
.L1:
	mov	r10, qword [rbp-16]
	mov	r11, qword [rbp-24]
	add	r10, r11
	lea	r11, [rbp-32]
	mov	qword [r11], r10
	mov	r10, qword [rbp-24]
	lea	r11, [rbp-16]
	mov	qword [r11], r10
	mov	r10, qword [rbp-32]
	lea	r11, [rbp-24]
	mov	qword [r11], r10
	mov	r10, qword [rbp-8]
	mov	r11, 1
	sub	r10, r11
	lea	r11, [rbp-8]
	mov	qword [r11], r10
.L2:
	mov	r10, qword [rbp-8]
	mov	r11, 0
	cmp	r10, r11
	setg	r10b
	test	r10b, r10b
	jnz	.L1
	mov	r10, qword [rbp-16]
	mov	rax, r10
	jmp	.L0
	;
.L0:
	add	rsp, 32
	pop	rbp
	ret

	global fib_recursive
fib_recursive:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	;
	mov	qword [rbp-8], rdi
	;
	mov	r10, qword [rbp-8]
	mov	r11, 1
	cmp	r10, r11
	setle	r10b
	test	r10b, r10b
	jz	.L4
	mov	r10, qword [rbp-8]
	mov	rax, r10
	jmp	.L3
.L4:
	mov	r10, qword [rbp-8]
	mov	r11, 1
	sub	r10, r11
	mov	rdi, r10
	lea	r10, [rel fib_recursive]
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
	mov	r12, 2
	sub	r11, r12
	mov	rdi, r11
	lea	r11, [rel fib_recursive]
	push	r10
	push	r11
	push	r12
	push	r13
	push	r14
	push	r15
	call	r11
	pop	r15
	pop	r14
	pop	r13
	pop	r12
	pop	r11
	pop	r10
	mov	r11, rax
	add	r10, r11
	mov	rax, r10
	jmp	.L3
	;
.L3:
	add	rsp, 16
	pop	rbp
	ret

	global main
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	;
	;
	mov	r10, 0
	lea	r11, [rbp-8]
	mov	qword [r11], r10
	jmp	.L7
.L6:
	mov	r10, qword [rbp-8]
	mov	rdi, r10
	lea	r10, [rel fib_recursive]
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
	mov	r10, qword [rbp-8]
	mov	r11, 1
	add	r10, r11
	lea	r11, [rbp-8]
	mov	qword [r11], r10
.L7:
	mov	r10, qword [rbp-8]
	mov	r11, 92
	cmp	r10, r11
	setle	r10b
	test	r10b, r10b
	jnz	.L6
	mov	r10, 0
	mov	eax, r10d
	jmp	.L5
	;
.L5:
	add	rsp, 16
	pop	rbp
	ret

section .data
	printi_s: db "%ld", 10, 0

section .note.GNU-stack

