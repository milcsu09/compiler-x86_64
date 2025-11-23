section .text
	extern	printf

printi:
	mov	rsi, rdi
	xor	eax, eax
	mov	rdi, printi_s
	jmp	printf

	global	stack_push
stack_push:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	mov	qword [rbp-8], rdi
	mov	qword [rbp-16], rsi
	mov	r10, qword [rbp-8]
	add	r10, 0
	mov	r11, qword [rbp-8]
	add	r11, 512
	mov	r11, qword [r11]
	imul	r11, 8
	add	r10, r11
	mov	r11, qword [rbp-16]
	mov	qword [r10], r11
	mov	r10, qword [rbp-8]
	add	r10, 512
	mov	r11, qword [rbp-8]
	add	r11, 512
	mov	r11, qword [r11]
	mov	r12, 1
	add	r11, r12
	mov	qword [r10], r11
.L0:
	add	rsp, 16
	pop	rbp
	ret

	global	stack_pop
stack_pop:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	mov	qword [rbp-8], rdi
	mov	r10, qword [rbp-8]
	add	r10, 512
	mov	r11, qword [rbp-8]
	add	r11, 512
	mov	r11, qword [r11]
	mov	r12, 1
	sub	r11, r12
	mov	qword [r10], r11
	mov	r10, qword [rbp-8]
	add	r10, 0
	mov	r11, qword [rbp-8]
	add	r11, 512
	mov	r11, qword [r11]
	imul	r11, 8
	add	r10, r11
	mov	r10, qword [r10]
	mov	rax, r10
	jmp	.L1
.L1:
	add	rsp, 16
	pop	rbp
	ret

	global	main
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 528
	lea	r10, [rbp-520]
	add	r10, 512
	mov	r11, 0
	mov	qword [r10], r11
	lea	r10, [rbp-520]
	mov	rdi, r10
	mov	r10, 34
	mov	rsi, r10
	lea	r10, [rel stack_push]
	call	r10
	lea	r10, [rbp-520]
	mov	rdi, r10
	mov	r10, 35
	mov	rsi, r10
	lea	r10, [rel stack_push]
	call	r10
	lea	r10, [rbp-520]
	mov	rdi, r10
	lea	r10, [rel stack_pop]
	call	r10
	mov	r10, rax
	push	r10
	lea	r11, [rbp-520]
	mov	rdi, r11
	sub	rsp, 8
	lea	r11, [rel stack_pop]
	call	r11
	add	rsp, 8
	pop	r10
	mov	r11, rax
	add	r10, r11
	mov	rdi, r10
	call	printi
	mov	r10, 0
	mov	eax, r10d
	jmp	.L2
.L2:
	add	rsp, 528
	pop	rbp
	ret

section .rodata
	printi_s: db "%ld", 10, 0

section .note.GNU-stack

