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
	mov	r10, 1
	lea	r11, [rbp-8]
	mov	qword [r11], r10
	jmp	.L2
.L1:
	mov	r10, qword [rbp-8]
	lea	r11, [rbp-16]
	mov	qword [r11], r10
	jmp	.L6
.L5:
	mov	r10, 1000
	mov	r11, qword [rbp-8]
	sub	r10, r11
	mov	r11, qword [rbp-16]
	sub	r10, r11
	lea	r11, [rbp-24]
	mov	qword [r11], r10
	mov	r10, qword [rbp-8]
	mov	r11, qword [rbp-8]
	imul	r10, r11
	mov	r11, qword [rbp-16]
	mov	r12, qword [rbp-16]
	imul	r11, r12
	add	r10, r11
	mov	r11, qword [rbp-24]
	mov	r12, qword [rbp-24]
	imul	r11, r12
	cmp	r10, r11
	sete	r10b
	test	r10b, r10b
	jz	.L9
	mov	r10, qword [rbp-8]
	mov	r11, qword [rbp-16]
	imul	r10, r11
	mov	r11, qword [rbp-24]
	imul	r10, r11
	mov	rdi, r10
	call	printi
.L9:
.L8:
	mov	r10, qword [rbp-16]
	mov	r11, 1
	add	r10, r11
	lea	r11, [rbp-16]
	mov	qword [r11], r10
.L6:
	mov	r10, qword [rbp-16]
	mov	r11, 1000
	cmp	r10, r11
	setl	r10b
	test	r10b, r10b
	jnz	.L5
.L7:
.L4:
	mov	r10, qword [rbp-8]
	mov	r11, 1
	add	r10, r11
	lea	r11, [rbp-8]
	mov	qword [r11], r10
.L2:
	mov	r10, qword [rbp-8]
	mov	r11, 1000
	cmp	r10, r11
	setl	r10b
	test	r10b, r10b
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

