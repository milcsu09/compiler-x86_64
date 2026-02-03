section .text
	extern	printf

printi:
	mov	rsi, rdi
	xor	eax, eax
	mov	rdi, printi_s
	jmp	printf

	global	f1
f1:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	mov	qword [rbp-8], rdi
	mov	r10, qword [rbp-8]
	mov	rax, r10
	jmp	.L0
	xor	rax, rax
.L0:
	add	rsp, 16
	pop	rbp
	ret

	global	f2
f2:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	mov	qword [rbp-8], rdi
	mov	r10, qword [rbp-8]
	mov	r11, 1
	add	r10, r11
	mov	rax, r10
	jmp	.L1
	xor	rax, rax
.L1:
	add	rsp, 16
	pop	rbp
	ret

	global	f3
f3:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	mov	qword [rbp-8], rdi
	mov	r10, qword [rbp-8]
	mov	r11, 2
	imul	r10, r11
	mov	rax, r10
	jmp	.L2
	xor	rax, rax
.L2:
	add	rsp, 16
	pop	rbp
	ret

	global	f4
f4:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	mov	qword [rbp-8], rdi
	mov	r10, qword [rbp-8]
	mov	r11, qword [rbp-8]
	imul	r10, r11
	mov	rax, r10
	jmp	.L3
	xor	rax, rax
.L3:
	add	rsp, 16
	pop	rbp
	ret

	global	main
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 48
	lea	r10, [rel f1]
	lea	r11, [rbp-32]
	mov	r12, 0
	imul	r12, 8
	add	r11, r12
	mov	qword [r11], r10
	lea	r10, [rel f2]
	lea	r11, [rbp-32]
	mov	r12, 1
	imul	r12, 8
	add	r11, r12
	mov	qword [r11], r10
	lea	r10, [rel f3]
	lea	r11, [rbp-32]
	mov	r12, 2
	imul	r12, 8
	add	r11, r12
	mov	qword [r11], r10
	lea	r10, [rel f4]
	lea	r11, [rbp-32]
	mov	r12, 3
	imul	r12, 8
	add	r11, r12
	mov	qword [r11], r10
	mov	r10, 0
	lea	r11, [rbp-40]
	mov	qword [r11], r10
	jmp	.L6
.L5:
	mov	r10, 10
	mov	rdi, r10
	lea	r10, [rbp-32]
	mov	r11, qword [rbp-40]
	imul	r11, 8
	add	r10, r11
	mov	r10, qword [r10]
	call	r10
	mov	r10, rax
	mov	rdi, r10
	call	printi
.L8:
	mov	r10, qword [rbp-40]
	mov	r11, 1
	add	r10, r11
	lea	r11, [rbp-40]
	mov	qword [r11], r10
.L6:
	mov	r10, qword [rbp-40]
	mov	r11, 4
	cmp	r10, r11
	setl	r10b
	test	r10b, r10b
	jnz	.L5
.L7:
	xor	rax, rax
.L4:
	add	rsp, 48
	pop	rbp
	ret

section .rodata
	printi_s: db "%ld", 10, 0

section .note.GNU-stack

