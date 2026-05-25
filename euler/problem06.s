section .text
	extern	printf

printi:
	mov	rsi, rdi
	xor	eax, eax
	mov	rdi, printi_s
	jmp	printf

	global	sum_square
sum_square:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	mov	qword [rbp-8], rdi
	mov	r8, 0
	lea	r9, [rbp-16]
	mov	qword [r9], r8
	jmp	.L2
.L1:
	lea	r8, [rbp-16]
	mov	r9, qword [r8]
	mov	r10, qword [rbp-8]
	mov	r11, qword [rbp-8]
	imul	r10, r11
	add	r9, r10
	mov	qword [r8], r9
	lea	r8, [rbp-8]
	mov	r9, qword [r8]
	mov	r10, 1
	sub	r9, r10
	mov	qword [r8], r9
.L2:
	mov	r8, qword [rbp-8]
	mov	r9, 0
	cmp	r8, r9
	setg	r8b
	test	r8b, r8b
	jnz	.L1
.L3:
	mov	r8, qword [rbp-16]
	mov	rax, r8
	jmp	.L0
	xor	rax, rax
.L0:
	add	rsp, 16
	pop	rbp
	ret

	global	square_sum
square_sum:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	mov	qword [rbp-8], rdi
	mov	r8, 0
	lea	r9, [rbp-16]
	mov	qword [r9], r8
	jmp	.L6
.L5:
	lea	r8, [rbp-16]
	mov	r9, qword [r8]
	mov	r10, qword [rbp-8]
	add	r9, r10
	mov	qword [r8], r9
	lea	r8, [rbp-8]
	mov	r9, qword [r8]
	mov	r10, 1
	sub	r9, r10
	mov	qword [r8], r9
.L6:
	mov	r8, qword [rbp-8]
	mov	r9, 0
	cmp	r8, r9
	setg	r8b
	test	r8b, r8b
	jnz	.L5
.L7:
	mov	r8, qword [rbp-16]
	mov	r9, qword [rbp-16]
	imul	r8, r9
	mov	rax, r8
	jmp	.L4
	xor	rax, rax
.L4:
	add	rsp, 16
	pop	rbp
	ret

	global	main
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 0
	mov	r10, 100
	mov	rdi, r10
	lea	r10, [rel square_sum]
	call	r10
	mov	r8, rax
	push	r8
	mov	r10, 100
	mov	rdi, r10
	sub	rsp, 8
	lea	r10, [rel sum_square]
	call	r10
	add	rsp, 8
	pop	r8
	mov	r9, rax
	sub	r8, r9
	mov	rdi, r8
	call	printi
.L8:
	xor	rax, rax
	add	rsp, 0
	pop	rbp
	ret

section .rodata
	printi_s: db "%ld", 10, 0

section .bss

section .note.GNU-stack

