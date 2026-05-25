section .text
	extern	printf

printi:
	mov	rsi, rdi
	xor	eax, eax
	mov	rdi, printi_s
	jmp	printf

	global	add
add:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	mov	qword [rbp-8], rdi
	mov	qword [rbp-16], rsi
	mov	r8, qword [rbp-8]
	mov	r9, qword [rbp-16]
	add	r8, r9
	mov	rax, r8
	jmp	.L0
	xor	rax, rax
.L0:
	add	rsp, 16
	pop	rbp
	ret

	global	main
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 0
	mov	r10, 10
	mov	rdi, r10
	mov	r10, 20
	mov	rsi, r10
	lea	r10, [rel add]
	call	r10
	mov	r8, rax
	mov	rdi, r8
	call	printi
.L1:
	xor	rax, rax
	add	rsp, 0
	pop	rbp
	ret

section .rodata
	printi_s: db "%ld", 10, 0

section .bss

section .note.GNU-stack

