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
	mov	r8, 1
	lea	r9, [rbp-8]
	mov	qword [r9], r8
	jmp	.L2
.L1:
	mov	r8, qword [rbp-8]
	lea	r9, [rbp-16]
	mov	qword [r9], r8
	jmp	.L6
.L5:
	mov	r8, 1000
	mov	r9, qword [rbp-8]
	sub	r8, r9
	mov	r9, qword [rbp-16]
	sub	r8, r9
	lea	r9, [rbp-24]
	mov	qword [r9], r8
	mov	r8, qword [rbp-8]
	mov	r9, qword [rbp-8]
	imul	r8, r9
	mov	r9, qword [rbp-16]
	mov	r10, qword [rbp-16]
	imul	r9, r10
	add	r8, r9
	mov	r9, qword [rbp-24]
	mov	r10, qword [rbp-24]
	imul	r9, r10
	cmp	r8, r9
	sete	r8b
	test	r8b, r8b
	jz	.L9
	mov	r8, qword [rbp-8]
	mov	r9, qword [rbp-16]
	imul	r8, r9
	mov	r9, qword [rbp-24]
	imul	r8, r9
	mov	rdi, r8
	call	printi
.L9:
.L8:
	lea	r8, [rbp-16]
	mov	r9, qword [r8]
	mov	r10, 1
	add	r9, r10
	mov	qword [r8], r9
.L6:
	mov	r8, qword [rbp-16]
	mov	r9, 1000
	cmp	r8, r9
	setl	r8b
	test	r8b, r8b
	jnz	.L5
.L7:
.L4:
	lea	r8, [rbp-8]
	mov	r9, qword [r8]
	mov	r10, 1
	add	r9, r10
	mov	qword [r8], r9
.L2:
	mov	r8, qword [rbp-8]
	mov	r9, 1000
	cmp	r8, r9
	setl	r8b
	test	r8b, r8b
	jnz	.L1
.L3:
.L0:
	xor	rax, rax
	add	rsp, 32
	pop	rbp
	ret

section .rodata
	printi_s: db "%ld", 10, 0

section .bss

section .note.GNU-stack

