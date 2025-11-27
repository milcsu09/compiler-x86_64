section .text
	extern	printf

printi:
	mov	rsi, rdi
	xor	eax, eax
	mov	rdi, printi_s
	jmp	printf

	extern	malloc
	extern	free
	global	main
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	mov	r10, 16
	mov	rdi, r10
	lea	r10, [rel malloc]
	call	r10
	mov	r10, rax
	lea	r11, [rbp-8]
	mov	qword [r11], r10
	mov	r10, qword [rbp-8]
	mov	rdi, r10
	call	printi
	mov	r10, 0
	lea	r11, [rbp-16]
	mov	qword [r11], r10
	jmp	.L2
.L1:
	mov	r10, qword [rbp-16]
	mov	r11, qword [rbp-16]
	imul	r10, r11
	mov	r11, qword [rbp-8]
	mov	r12, qword [rbp-16]
	imul	r12, 4
	add	r11, r12
	mov	dword [r11], r10d
.L4:
	mov	r10, qword [rbp-16]
	mov	r11, 1
	add	r10, r11
	lea	r11, [rbp-16]
	mov	qword [r11], r10
.L2:
	mov	r10, qword [rbp-16]
	mov	r11, 4
	cmp	r10, r11
	setl	r10b
	test	r10b, r10b
	jnz	.L1
.L3:
	mov	r10, 0
	lea	r11, [rbp-16]
	mov	qword [r11], r10
	jmp	.L6
.L5:
	mov	r10, qword [rbp-8]
	mov	r11, qword [rbp-16]
	imul	r11, 4
	add	r10, r11
	mov	r10d, dword [r10]
	movsx	r10, r10d
	mov	rdi, r10
	call	printi
.L8:
	mov	r10, qword [rbp-16]
	mov	r11, 1
	add	r10, r11
	lea	r11, [rbp-16]
	mov	qword [r11], r10
.L6:
	mov	r10, qword [rbp-16]
	mov	r11, 4
	cmp	r10, r11
	setl	r10b
	test	r10b, r10b
	jnz	.L5
.L7:
	mov	r10, qword [rbp-8]
	mov	rdi, r10
	lea	r10, [rel free]
	call	r10
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

