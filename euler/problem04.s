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
	mov	r10, 0
	lea	r11, [rbp-16]
	mov	qword [r11], r10
	jmp	.L2
.L1:
	mov	r10, qword [rbp-16]
	mov	r11, 10
	imul	r10, r11
	lea	r11, [rbp-16]
	mov	qword [r11], r10
	mov	r10, qword [rbp-16]
	mov	r11, qword [rbp-8]
	mov	r12, 10
	mov	rax, r11
	cqo
	idiv	r12
	mov	r11, rdx
	add	r10, r11
	lea	r11, [rbp-16]
	mov	qword [r11], r10
	mov	r10, qword [rbp-8]
	mov	r11, 10
	mov	rax, r10
	cqo
	idiv	r11
	mov	r10, rax
	lea	r11, [rbp-8]
	mov	qword [r11], r10
.L2:
	mov	r10, qword [rbp-8]
	test	r10, r10
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

	global	is_palindrome
is_palindrome:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	mov	qword [rbp-8], rdi
	mov	r10, qword [rbp-8]
	push	r10
	mov	r11, qword [rbp-8]
	mov	rdi, r11
	sub	rsp, 8
	lea	r11, [rel reverse]
	call	r11
	add	rsp, 8
	pop	r10
	mov	r11, rax
	cmp	r10, r11
	sete	r10b
	mov	al, r10b
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
	mov	r10, 0
	lea	r11, [rbp-24]
	mov	qword [r11], r10
	mov	r10, 100
	lea	r11, [rbp-8]
	mov	qword [r11], r10
	jmp	.L7
.L6:
	mov	r10, 100
	lea	r11, [rbp-16]
	mov	qword [r11], r10
	jmp	.L11
.L10:
	mov	r10, qword [rbp-8]
	mov	r11, qword [rbp-16]
	imul	r10, r11
	lea	r11, [rbp-32]
	mov	qword [r11], r10
	mov	r10, qword [rbp-32]
	mov	rdi, r10
	lea	r10, [rel is_palindrome]
	call	r10
	mov	r10b, al
	test	r10b, r10b
	jz	.L15
	mov	r10, qword [rbp-32]
	mov	r11, qword [rbp-24]
	cmp	r10, r11
	setg	r10b
.L15:
	test	r10b, r10b
	jz	.L14
	mov	r10, qword [rbp-32]
	lea	r11, [rbp-24]
	mov	qword [r11], r10
.L14:
.L13:
	mov	r10, qword [rbp-16]
	mov	r11, 1
	add	r10, r11
	lea	r11, [rbp-16]
	mov	qword [r11], r10
.L11:
	mov	r10, qword [rbp-16]
	mov	r11, 999
	cmp	r10, r11
	setle	r10b
	test	r10b, r10b
	jnz	.L10
.L12:
.L9:
	mov	r10, qword [rbp-8]
	mov	r11, 1
	add	r10, r11
	lea	r11, [rbp-8]
	mov	qword [r11], r10
.L7:
	mov	r10, qword [rbp-8]
	mov	r11, 999
	cmp	r10, r11
	setle	r10b
	test	r10b, r10b
	jnz	.L6
.L8:
	mov	r10, qword [rbp-24]
	mov	rdi, r10
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

