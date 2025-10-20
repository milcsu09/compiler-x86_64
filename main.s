section .text
	extern	printf

printi:
	mov	rsi, rdi
	xor	eax, eax
	mov	edi, printi_s
	jmp	printf

	global is_even
is_even:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	;
	mov	qword [rbp-8], rdi
	;
	mov	r10, qword [rbp-8]
	mov	r11, 2
	mov	r12, qword [rbp-8]
	mov	r13, 2
	mov	rax, r12
	cqo
	idiv	r13
	mov	r12, rax
	imul	r11, r12
	sub	r10, r11
	mov	r11, 0
	cmp	r10, r11
	sete	r10b
	mov	al, r10b
	jmp	.L0
	;
.L0:
	add	rsp, 16
	pop	rbp
	ret

	global main
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 32
	;
	;
	mov	r10, 1
	lea	r11, [rbp-8]
	mov	qword [r11], r10
	mov	r10, 2
	lea	r11, [rbp-16]
	mov	qword [r11], r10
	mov	r10, 0
	lea	r11, [rbp-24]
	mov	qword [r11], r10
	jmp	.L3
.L2:
	mov	r10, qword [rbp-8]
	mov	rdi, r10
	lea	r10, [rel is_even]
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
	mov	r10b, al
	test	r10b, r10b
	jz	.L4
	mov	r10, qword [rbp-24]
	mov	r11, qword [rbp-8]
	add	r10, r11
	lea	r11, [rbp-24]
	mov	qword [r11], r10
.L4:
	mov	r10, qword [rbp-8]
	mov	r11, qword [rbp-16]
	add	r10, r11
	lea	r11, [rbp-32]
	mov	qword [r11], r10
	mov	r10, qword [rbp-16]
	lea	r11, [rbp-8]
	mov	qword [r11], r10
	mov	r10, qword [rbp-32]
	lea	r11, [rbp-16]
	mov	qword [r11], r10
.L3:
	mov	r10, qword [rbp-8]
	mov	r11, 4000000
	cmp	r10, r11
	setl	r10b
	test	r10b, r10b
	jnz	.L2
	mov	r10, qword [rbp-24]
	mov	rdi, r10
	call	printi
	mov	r10, 0
	mov	eax, r10d
	jmp	.L1
	;
.L1:
	add	rsp, 32
	pop	rbp
	ret

section .data
	printi_s: db "%ld", 10, 0

section .note.GNU-stack

