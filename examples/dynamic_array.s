section .text
	extern	printf

printi:
	mov	rsi, rdi
	xor	eax, eax
	mov	rdi, printi_s
	jmp	printf

	extern	malloc
	extern	realloc
	extern	free
	global	da_create
da_create:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	mov	qword [rbp-8], rdi
	lea	r10, [rbp-16]
	push	r10
	mov	r11, 24
	mov	rdi, r11
	sub	rsp, 8
	lea	r11, [rel malloc]
	call	r11
	add	rsp, 8
	pop	r10
	mov	r11, rax
	mov	qword [r10], r11
	mov	r10, qword [rbp-16]
	add	r10, 0
	push	r10
	mov	r11, 8
	mov	r12, qword [rbp-8]
	imul	r11, r12
	mov	rdi, r11
	sub	rsp, 8
	lea	r11, [rel malloc]
	call	r11
	add	rsp, 8
	pop	r10
	mov	r11, rax
	mov	qword [r10], r11
	mov	r10, qword [rbp-16]
	add	r10, 8
	mov	r11, 0
	mov	qword [r10], r11
	mov	r10, qword [rbp-16]
	add	r10, 16
	mov	r11, qword [rbp-8]
	mov	qword [r10], r11
	mov	r10, qword [rbp-16]
	mov	rax, r10
	jmp	.L0
.L0:
	add	rsp, 16
	pop	rbp
	ret

	global	da_destroy
da_destroy:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	mov	qword [rbp-8], rdi
	mov	r10, qword [rbp-8]
	add	r10, 0
	mov	r10, qword [r10]
	mov	rdi, r10
	lea	r10, [rel free]
	call	r10
	mov	r10, qword [rbp-8]
	mov	rdi, r10
	lea	r10, [rel free]
	call	r10
.L1:
	add	rsp, 16
	pop	rbp
	ret

	global	da_append
da_append:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	mov	qword [rbp-8], rdi
	mov	qword [rbp-16], rsi
	mov	r10, qword [rbp-8]
	add	r10, 8
	mov	r10, qword [r10]
	mov	r11, qword [rbp-8]
	add	r11, 16
	mov	r11, qword [r11]
	cmp	r10, r11
	setge	r10b
	movzx	r10, r10b
	test	r10, r10
	jz	.L3
	mov	r10, qword [rbp-8]
	add	r10, 16
	mov	r11, qword [rbp-8]
	add	r11, 16
	mov	r11, qword [r11]
	mov	r12, 2
	imul	r11, r12
	mov	qword [r10], r11
	mov	r10, qword [rbp-8]
	add	r10, 0
	push	r10
	mov	r11, qword [rbp-8]
	add	r11, 0
	mov	r11, qword [r11]
	mov	rdi, r11
	mov	r11, 8
	mov	r12, qword [rbp-8]
	add	r12, 16
	mov	r12, qword [r12]
	imul	r11, r12
	mov	rsi, r11
	sub	rsp, 8
	lea	r11, [rel realloc]
	call	r11
	add	rsp, 8
	pop	r10
	mov	r11, rax
	mov	qword [r10], r11
.L3:
	mov	r10, qword [rbp-8]
	add	r10, 0
	mov	r10, qword [r10]
	mov	r11, qword [rbp-8]
	add	r11, 8
	mov	r11, qword [r11]
	imul	r11, 8
	add	r10, r11
	mov	r11, qword [rbp-16]
	mov	qword [r10], r11
	mov	r10, qword [rbp-8]
	add	r10, 8
	mov	r11, qword [rbp-8]
	add	r11, 8
	mov	r11, qword [r11]
	mov	r12, 1
	add	r11, r12
	mov	qword [r10], r11
.L2:
	add	rsp, 16
	pop	rbp
	ret

	global	main
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	lea	r10, [rbp-8]
	push	r10
	mov	r11, 4
	mov	rdi, r11
	sub	rsp, 8
	lea	r11, [rel da_create]
	call	r11
	add	rsp, 8
	pop	r10
	mov	r11, rax
	mov	qword [r10], r11
	lea	r10, [rbp-16]
	mov	r11, 0
	mov	qword [r10], r11
	jmp	.L6
.L5:
	mov	r10, qword [rbp-8]
	mov	rdi, r10
	mov	r10, qword [rbp-16]
	mov	r11, qword [rbp-16]
	imul	r10, r11
	mov	rsi, r10
	lea	r10, [rel da_append]
	call	r10
	lea	r10, [rbp-16]
	mov	r11, qword [rbp-16]
	mov	r12, 1
	add	r11, r12
	mov	qword [r10], r11
.L6:
	mov	r10, qword [rbp-16]
	mov	r11, 100
	cmp	r10, r11
	setl	r10b
	movzx	r10, r10b
	test	r10, r10
	jnz	.L5
	lea	r10, [rbp-16]
	mov	r11, 0
	mov	qword [r10], r11
	jmp	.L8
.L7:
	mov	r10, qword [rbp-8]
	add	r10, 0
	mov	r10, qword [r10]
	mov	r11, qword [rbp-16]
	imul	r11, 8
	add	r10, r11
	mov	r10, qword [r10]
	mov	rdi, r10
	call	printi
	lea	r10, [rbp-16]
	mov	r11, qword [rbp-16]
	mov	r12, 1
	add	r11, r12
	mov	qword [r10], r11
.L8:
	mov	r10, qword [rbp-16]
	mov	r11, 100
	cmp	r10, r11
	setl	r10b
	movzx	r10, r10b
	test	r10, r10
	jnz	.L7
	mov	r10, qword [rbp-8]
	mov	rdi, r10
	lea	r10, [rel da_destroy]
	call	r10
	mov	r10, 0
	mov	eax, r10d
	jmp	.L4
.L4:
	add	rsp, 16
	pop	rbp
	ret

section .rodata
	printi_s: db "%ld", 10, 0

section .note.GNU-stack

