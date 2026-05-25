section .text
	extern	printf

printi:
	mov	rsi, rdi
	xor	eax, eax
	mov	rdi, printi_s
	jmp	printf

	extern	putchar
	global	main
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	mov	r8, LS0
	lea	r9, [rbp-8]
	mov	qword [r9], r8
	jmp	.L2
.L1:
	mov	r10, qword [rbp-8]
	mov	r10b, byte [r10]
	mov	dil, r10b
	lea	r10, [rel putchar]
	call	r10
.L4:
	lea	r8, [rbp-8]
	mov	r9, qword [r8]
	mov	r10, 1
	add	r9, r10
	mov	qword [r8], r9
.L2:
	mov	r8, qword [rbp-8]
	mov	r8b, byte [r8]
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
	LS0: db 72, 101, 108, 108, 111, 44, 32, 119, 111, 114, 108, 100, 33, 10, 0

section .bss

section .note.GNU-stack

