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
	mov	r10, 0
	lea	r11, [rbp-16]
	mov	qword [r11], r10
	jmp	.L2
.L1:
	mov	r10, qword [rbp-16]
	mov	r11, qword [rbp-8]
	mov	r12, qword [rbp-8]
	imul	r11, r12
	add	r10, r11
	lea	r11, [rbp-16]
	mov	qword [r11], r10
	mov	r10, qword [rbp-8]
	mov	r11, 1
	sub	r10, r11
	lea	r11, [rbp-8]
	mov	qword [r11], r10
.L2:
	mov	r10, qword [rbp-8]
	mov	r11, 0
	cmp	r10, r11
	setg	r10b
	test	r10b, r10b
	jnz	.L1
.L3:
	mov	r10, qword [rbp-16]
	mov	rax, r10
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
	mov	r10, 0
	lea	r11, [rbp-16]
	mov	qword [r11], r10
	jmp	.L6
.L5:
	mov	r10, qword [rbp-16]
	mov	r11, qword [rbp-8]
	add	r10, r11
	lea	r11, [rbp-16]
	mov	qword [r11], r10
	mov	r10, qword [rbp-8]
	mov	r11, 1
	sub	r10, r11
	lea	r11, [rbp-8]
	mov	qword [r11], r10
.L6:
	mov	r10, qword [rbp-8]
	mov	r11, 0
	cmp	r10, r11
	setg	r10b
	test	r10b, r10b
	jnz	.L5
.L7:
	mov	r10, qword [rbp-16]
	mov	r11, qword [rbp-16]
	imul	r10, r11
	mov	rax, r10
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
	mov	r10, rax
	push	r10
	mov	r11, 100
	mov	rdi, r11
	sub	rsp, 8
	lea	r11, [rel sum_square]
	call	r11
	add	rsp, 8
	pop	r10
	mov	r11, rax
	sub	r10, r11
	mov	rdi, r10
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

