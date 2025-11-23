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
	lea	r10, [rbp-8]
	push	r10
	mov	r11, 16
	mov	rdi, r11
	sub	rsp, 8
	lea	r11, [rel malloc]
	call	r11
	add	rsp, 8
	pop	r10
	mov	r11, rax
	mov	qword [r10], r11
	mov	r10, qword [rbp-8]
	mov	rdi, r10
	call	printi
	lea	r10, [rbp-16]
	mov	r11, 0
	mov	qword [r10], r11
	jmp	.L2
.L1:
	mov	r10, qword [rbp-8]
	mov	r11, qword [rbp-16]
	imul	r11, 4
	add	r10, r11
	mov	r11, qword [rbp-16]
	mov	r12, qword [rbp-16]
	imul	r11d, r12d
	mov	dword [r10], r11d
	lea	r10, [rbp-16]
	mov	r11, qword [rbp-16]
	mov	r12, 1
	add	r11d, r12d
	movsx	r11, r11d
	mov	qword [r10], r11
.L2:
	mov	r10, qword [rbp-16]
	mov	r11, 4
	cmp	r10d, r11d
	setl	r10b
	movzx	r10, r10b
	test	r10, r10
	jnz	.L1
	lea	r10, [rbp-16]
	mov	r11, 0
	mov	qword [r10], r11
	jmp	.L4
.L3:
	mov	r10, qword [rbp-8]
	mov	r11, qword [rbp-16]
	imul	r11, 4
	add	r10, r11
	mov	r10d, dword [r10]
	movsx	r10, r10d
	mov	rdi, r10
	call	printi
	lea	r10, [rbp-16]
	mov	r11, qword [rbp-16]
	mov	r12, 1
	add	r11d, r12d
	movsx	r11, r11d
	mov	qword [r10], r11
.L4:
	mov	r10, qword [rbp-16]
	mov	r11, 4
	cmp	r10d, r11d
	setl	r10b
	movzx	r10, r10b
	test	r10, r10
	jnz	.L3
	mov	r10, qword [rbp-8]
	mov	rdi, r10
	lea	r10, [rel free]
	call	r10
	mov	r10, 0
	mov	eax, r10d
	jmp	.L0
.L0:
	add	rsp, 16
	pop	rbp
	ret

section .rodata
	printi_s: db "%ld", 10, 0

section .note.GNU-stack

