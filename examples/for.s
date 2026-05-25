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
	sub	rsp, 16
	mov	r8, 0
	lea	r9, [rbp-8]
	mov	qword [r9], r8
	jmp	.L2
.L1:
	mov	r8, qword [rbp-8]
	mov	rdi, r8
	call	printi
.L4:
	lea	r8, [rbp-8]
	mov	r9, qword [r8]
	mov	r10, 1
	add	r9, r10
	mov	qword [r8], r9
.L2:
	mov	r8, qword [rbp-8]
	mov	r9, 10
	cmp	r8, r9
	setl	r8b
	test	r8b, r8b
	jnz	.L1
.L3:
.L0:
	xor	rax, rax
	add	rsp, 16
	pop	rbp
	ret

section .rodata
	printi_s: db "%ld", 10, 0

section .bss

section .note.GNU-stack

