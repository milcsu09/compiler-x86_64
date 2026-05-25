section .text
	extern	printf

printi:
	mov	rsi, rdi
	xor	eax, eax
	mov	rdi, printi_s
	jmp	printf

	global	reverse
reverse:
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
	mov	r10, 10
	imul	r9, r10
	mov	qword [r8], r9
	lea	r8, [rbp-16]
	mov	r9, qword [r8]
	mov	r10, qword [rbp-8]
	mov	r11, 10
	mov	rax, r10
	cqo
	idiv	r11
	mov	r10, rdx
	add	r9, r10
	mov	qword [r8], r9
	lea	r8, [rbp-8]
	mov	r9, qword [r8]
	mov	r10, 10
	mov	rax, r9
	cqo
	idiv	r10
	mov	r9, rax
	mov	qword [r8], r9
.L2:
	mov	r8, qword [rbp-8]
	test	r8, r8
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

	global	is_palindrome
is_palindrome:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	mov	qword [rbp-8], rdi
	mov	r8, qword [rbp-8]
	push	r8
	mov	r10, qword [rbp-8]
	mov	rdi, r10
	sub	rsp, 8
	lea	r10, [rel reverse]
	call	r10
	add	rsp, 8
	pop	r8
	mov	r9, rax
	cmp	r8, r9
	sete	r8b
	mov	al, r8b
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
	sub	rsp, 32
	mov	r8, 0
	lea	r9, [rbp-24]
	mov	qword [r9], r8
	mov	r8, 100
	lea	r9, [rbp-8]
	mov	qword [r9], r8
	jmp	.L7
.L6:
	mov	r8, 100
	lea	r9, [rbp-16]
	mov	qword [r9], r8
	jmp	.L11
.L10:
	mov	r8, qword [rbp-8]
	mov	r9, qword [rbp-16]
	imul	r8, r9
	lea	r9, [rbp-32]
	mov	qword [r9], r8
	mov	r10, qword [rbp-32]
	mov	rdi, r10
	lea	r10, [rel is_palindrome]
	call	r10
	mov	r8b, al
	test	r8b, r8b
	jz	.L15
	mov	r8, qword [rbp-32]
	mov	r9, qword [rbp-24]
	cmp	r8, r9
	setg	r8b
.L15:
	test	r8b, r8b
	jz	.L14
	mov	r8, qword [rbp-32]
	lea	r9, [rbp-24]
	mov	qword [r9], r8
.L14:
.L13:
	lea	r8, [rbp-16]
	mov	r9, qword [r8]
	mov	r10, 1
	add	r9, r10
	mov	qword [r8], r9
.L11:
	mov	r8, qword [rbp-16]
	mov	r9, 999
	cmp	r8, r9
	setle	r8b
	test	r8b, r8b
	jnz	.L10
.L12:
.L9:
	lea	r8, [rbp-8]
	mov	r9, qword [r8]
	mov	r10, 1
	add	r9, r10
	mov	qword [r8], r9
.L7:
	mov	r8, qword [rbp-8]
	mov	r9, 999
	cmp	r8, r9
	setle	r8b
	test	r8b, r8b
	jnz	.L6
.L8:
	mov	r8, qword [rbp-24]
	mov	rdi, r8
	call	printi
.L5:
	xor	rax, rax
	add	rsp, 32
	pop	rbp
	ret

section .rodata
	printi_s: db "%ld", 10, 0

section .bss

section .note.GNU-stack

