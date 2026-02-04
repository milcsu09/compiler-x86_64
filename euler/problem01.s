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
	mov	r10, 0
	lea	r11, [rbp-8]
	mov	qword [r11], r10
	mov	r10, 0
	lea	r11, [rbp-16]
	mov	qword [r11], r10
	jmp	.L2
.L1:
	mov	r10, qword [rbp-16]
	mov	r11, 3
	mov	rax, r10
	cqo
	idiv	r11
	mov	r10, rdx
	mov	r11, 0
	cmp	r10, r11
	sete	r10b
	test	r10b, r10b
	jnz	.L6
	mov	r10, qword [rbp-16]
	mov	r11, 5
	mov	rax, r10
	cqo
	idiv	r11
	mov	r10, rdx
	mov	r11, 0
	cmp	r10, r11
	sete	r10b
.L6:
	test	r10b, r10b
	jz	.L5
	mov	r10, qword [rbp-8]
	mov	r11, qword [rbp-16]
	add	r10, r11
	lea	r11, [rbp-8]
	mov	qword [r11], r10
.L5:
.L4:
	mov	r10, qword [rbp-16]
	mov	r11, 1
	add	r10, r11
	lea	r11, [rbp-16]
	mov	qword [r11], r10
.L2:
	mov	r10, qword [rbp-16]
	mov	r11, 1000
	cmp	r10, r11
	setl	r10b
	test	r10b, r10b
	jnz	.L1
.L3:
	mov	r10, qword [rbp-8]
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

