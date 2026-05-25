section .text
	extern	printf

printi:
	mov	rsi, rdi
	xor	eax, eax
	mov	rdi, printi_s
	jmp	printf

	global	main
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 64
	mov	r8, 10
	lea	r9, [rbp-16]
	add	r9, 0
	mov	qword [r9], r8
	mov	r8, 20
	lea	r9, [rbp-32]
	add	r9, 0
	mov	qword [r9], r8
	mov	r8, 30
	lea	r9, [rbp-48]
	add	r9, 0
	mov	qword [r9], r8
	lea	r8, [rbp-32]
	lea	r9, [rbp-16]
	add	r9, 8
	mov	qword [r9], r8
	lea	r8, [rbp-48]
	lea	r9, [rbp-32]
	add	r9, 8
	mov	qword [r9], r8
	mov	r8, 0
	lea	r9, [rbp-48]
	add	r9, 8
	mov	qword [r9], r8
	lea	r8, [rbp-16]
	lea	r9, [rbp-56]
	mov	qword [r9], r8
	jmp	.L2
.L1:
	mov	r8, qword [rbp-56]
	add	r8, 0
	mov	r8, qword [r8]
	mov	rdi, r8
	call	printi
.L4:
	mov	r8, qword [rbp-56]
	add	r8, 8
	mov	r8, qword [r8]
	lea	r9, [rbp-56]
	mov	qword [r9], r8
.L2:
	mov	r8, qword [rbp-56]
	test	r8, r8
	jnz	.L1
.L3:
.L0:
	xor	rax, rax
	add	rsp, 64
	pop	rbp
	ret

section .rodata
	printi_s: db "%ld", 10, 0

section .bss

section .note.GNU-stack

