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
	mov	r10, 24
	mov	rdi, r10
	lea	r10, [rel malloc]
	call	r10
	mov	r10, rax
	lea	r11, [rbp-16]
	mov	qword [r11], r10
	mov	r10, 8
	mov	r11, qword [rbp-8]
	imul	r10, r11
	mov	rdi, r10
	lea	r10, [rel malloc]
	call	r10
	mov	r10, rax
	mov	r11, qword [rbp-16]
	add	r11, 0
	mov	qword [r11], r10
	mov	r10, 0
	mov	r11, qword [rbp-16]
	add	r11, 8
	mov	qword [r11], r10
	mov	r10, qword [rbp-8]
	mov	r11, qword [rbp-16]
	add	r11, 16
	mov	qword [r11], r10
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
	test	r10b, r10b
	jz	.L3
	mov	r10, qword [rbp-8]
	add	r10, 16
	mov	r10, qword [r10]
	mov	r11, 2
	imul	r10, r11
	mov	r11, qword [rbp-8]
	add	r11, 16
	mov	qword [r11], r10
	mov	r10, qword [rbp-8]
	add	r10, 0
	mov	r10, qword [r10]
	mov	rdi, r10
	mov	r10, 8
	mov	r11, qword [rbp-8]
	add	r11, 16
	mov	r11, qword [r11]
	imul	r10, r11
	mov	rsi, r10
	lea	r10, [rel realloc]
	call	r10
	mov	r10, rax
	mov	r11, qword [rbp-8]
	add	r11, 0
	mov	qword [r11], r10
.L3:
	mov	r10, qword [rbp-16]
	mov	r11, qword [rbp-8]
	add	r11, 0
	mov	r11, qword [r11]
	mov	r12, qword [rbp-8]
	add	r12, 8
	mov	r12, qword [r12]
	imul	r12, 8
	add	r11, r12
	mov	qword [r11], r10
	mov	r10, qword [rbp-8]
	add	r10, 8
	mov	r10, qword [r10]
	mov	r11, 1
	add	r10, r11
	mov	r11, qword [rbp-8]
	add	r11, 8
	mov	qword [r11], r10

.L2:
	add	rsp, 16
	pop	rbp
	ret

	global	main
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16

	mov	r10, 4
	mov	rdi, r10
	lea	r10, [rel da_create]
	call	r10
	mov	r10, rax
	lea	r11, [rbp-8]
	mov	qword [r11], r10
	mov	r10, 0
	lea	r11, [rbp-16]
	mov	qword [r11], r10
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
.L8:
	mov	r10, qword [rbp-16]
	mov	r11, 1
	add	r10, r11
	lea	r11, [rbp-16]
	mov	qword [r11], r10
.L6:
	mov	r10, qword [rbp-16]
	mov	r11, 100
	cmp	r10, r11
	setl	r10b
	test	r10b, r10b
	jnz	.L5
.L7:
	mov	r10, 0
	lea	r11, [rbp-16]
	mov	qword [r11], r10
	jmp	.L10
.L9:
	mov	r10, qword [rbp-8]
	add	r10, 0
	mov	r10, qword [r10]
	mov	r11, qword [rbp-16]
	imul	r11, 8
	add	r10, r11
	mov	r10, qword [r10]
	mov	rdi, r10
	call	printi
.L12:
	mov	r10, qword [rbp-16]
	mov	r11, 1
	add	r10, r11
	lea	r11, [rbp-16]
	mov	qword [r11], r10
.L10:
	mov	r10, qword [rbp-16]
	mov	r11, 100
	cmp	r10, r11
	setl	r10b
	test	r10b, r10b
	jnz	.L9
.L11:
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

