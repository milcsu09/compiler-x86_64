section .text
	extern	printf

printi:
	mov	rsi, rdi
	xor	eax, eax
	mov	rdi, printi_s
	jmp	printf

	global	mandelbrot
mandelbrot:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 80
	mov	qword [rbp-8], rdi
	mov	qword [rbp-16], rsi
	mov	qword [rbp-24], rdx
	lea	r10, [rbp-32]
	mov	r11, 0
	mov	qword [r10], r11
	lea	r10, [rbp-40]
	mov	r11, 0
	mov	qword [r10], r11
	lea	r10, [rbp-48]
	mov	r11, 0
	mov	qword [r10], r11
	jmp	.L2
.L1:
	lea	r10, [rbp-56]
	mov	r11, qword [rbp-32]
	mov	r12, qword [rbp-32]
	imul	r11, r12
	mov	r12, 16777216
	mov	rax, r11
	cqo
	idiv	r12
	mov	r11, rax
	mov	qword [r10], r11
	lea	r10, [rbp-64]
	mov	r11, qword [rbp-40]
	mov	r12, qword [rbp-40]
	imul	r11, r12
	mov	r12, 16777216
	mov	rax, r11
	cqo
	idiv	r12
	mov	r11, rax
	mov	qword [r10], r11
	mov	r10, qword [rbp-56]
	mov	r11, qword [rbp-64]
	add	r10, r11
	mov	r11, 4
	mov	r12, 16777216
	imul	r11, r12
	cmp	r10, r11
	setg	r10b
	movzx	r10, r10b
	test	r10, r10
	jz	.L4
	jmp	.L3
.L4:
	lea	r10, [rbp-72]
	mov	r11, 2
	mov	r12, qword [rbp-32]
	imul	r11, r12
	mov	r12, qword [rbp-40]
	imul	r11, r12
	mov	r12, 16777216
	mov	rax, r11
	cqo
	idiv	r12
	mov	r11, rax
	mov	r12, qword [rbp-16]
	add	r11, r12
	mov	qword [r10], r11
	lea	r10, [rbp-80]
	mov	r11, qword [rbp-56]
	mov	r12, qword [rbp-64]
	sub	r11, r12
	mov	r12, qword [rbp-8]
	add	r11, r12
	mov	qword [r10], r11
	lea	r10, [rbp-32]
	mov	r11, qword [rbp-80]
	mov	qword [r10], r11
	lea	r10, [rbp-40]
	mov	r11, qword [rbp-72]
	mov	qword [r10], r11
	lea	r10, [rbp-48]
	mov	r11, qword [rbp-48]
	mov	r12, 1
	add	r11, r12
	mov	qword [r10], r11
.L2:
	mov	r10, qword [rbp-48]
	mov	r11, qword [rbp-24]
	cmp	r10, r11
	setl	r10b
	movzx	r10, r10b
	test	r10, r10
	jnz	.L1
.L3:
	mov	r10, qword [rbp-48]
	mov	rax, r10
	jmp	.L0
.L0:
	add	rsp, 80
	pop	rbp
	ret

	extern	putchar
	global	main
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 96
	lea	r10, [rbp-8]
	mov	r11, 33554432
	neg	r11
	mov	qword [r10], r11
	lea	r10, [rbp-16]
	mov	r11, 16777216
	mov	qword [r10], r11
	lea	r10, [rbp-24]
	mov	r11, 25165824
	neg	r11
	mov	qword [r10], r11
	lea	r10, [rbp-32]
	mov	r11, 25165824
	mov	qword [r10], r11
	lea	r10, [rbp-40]
	mov	r11, qword [rbp-16]
	mov	r12, qword [rbp-8]
	sub	r11, r12
	mov	r12, 80
	mov	rax, r11
	cqo
	idiv	r12
	mov	r11, rax
	mov	qword [r10], r11
	lea	r10, [rbp-48]
	mov	r11, qword [rbp-32]
	mov	r12, qword [rbp-24]
	sub	r11, r12
	mov	r12, 40
	mov	rax, r11
	cqo
	idiv	r12
	mov	r11, rax
	mov	qword [r10], r11
	lea	r10, [rbp-56]
	mov	r11, LS0
	mov	qword [r10], r11
	lea	r10, [rbp-64]
	mov	r11, 0
	mov	qword [r10], r11
	jmp	.L7
.L6:
	lea	r10, [rbp-80]
	mov	r11, qword [rbp-32]
	mov	r12, qword [rbp-64]
	push	r10
	mov	r10, qword [rbp-48]
	imul	r12, r10
	pop	r10
	sub	r11, r12
	mov	qword [r10], r11
	lea	r10, [rbp-72]
	mov	r11, 0
	mov	qword [r10], r11
	jmp	.L11
.L10:
	lea	r10, [rbp-88]
	mov	r11, qword [rbp-8]
	mov	r12, qword [rbp-72]
	push	r10
	mov	r10, qword [rbp-40]
	imul	r12, r10
	pop	r10
	add	r11, r12
	mov	qword [r10], r11
	lea	r10, [rbp-96]
	push	r10
	mov	r11, qword [rbp-88]
	mov	rdi, r11
	mov	r11, qword [rbp-80]
	mov	rsi, r11
	mov	r11, 15
	mov	rdx, r11
	sub	rsp, 8
	lea	r11, [rel mandelbrot]
	call	r11
	add	rsp, 8
	pop	r10
	mov	r11, rax
	mov	qword [r10], r11
	mov	r10, qword [rbp-56]
	mov	r11, qword [rbp-96]
	mov	r12, 9
	imul	r11, r12
	mov	r12, 15
	mov	rax, r11
	cqo
	idiv	r12
	mov	r11, rax
	add	r10, r11
	mov	r10b, byte [r10]
	mov	dil, r10b
	lea	r10, [rel putchar]
	call	r10
.L13:
	lea	r10, [rbp-72]
	mov	r11, qword [rbp-72]
	mov	r12, 1
	add	r11, r12
	mov	qword [r10], r11
.L11:
	mov	r10, qword [rbp-72]
	mov	r11, 80
	cmp	r10, r11
	setl	r10b
	movzx	r10, r10b
	test	r10, r10
	jnz	.L10
.L12:
	mov	r10, 10
	mov	dil, r10b
	lea	r10, [rel putchar]
	call	r10
.L9:
	lea	r10, [rbp-64]
	mov	r11, qword [rbp-64]
	mov	r12, 1
	add	r11, r12
	mov	qword [r10], r11
.L7:
	mov	r10, qword [rbp-64]
	mov	r11, 40
	cmp	r10, r11
	setl	r10b
	movzx	r10, r10b
	test	r10, r10
	jnz	.L6
.L8:
	mov	r10, 0
	mov	eax, r10d
	jmp	.L5
.L5:
	add	rsp, 96
	pop	rbp
	ret

section .rodata
	printi_s: db "%ld", 10, 0
	LS0: db 32, 46, 58, 45, 61, 43, 42, 35, 37, 64, 0

section .note.GNU-stack

