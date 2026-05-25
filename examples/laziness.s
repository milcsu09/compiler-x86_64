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
	mov	r8, qword [rbp-8]
	test	r8, r8
	jz	.L1
	mov	r8, qword [rbp-8]
	mov	r8, qword [r8]
	mov	r9, 10
	cmp	r8, r9
	sete	r8b
	movzx	r8, r8b
.L1:
	mov	rdi, r8
	call	printi
.L0:
	xor	rax, rax
	add	rsp, 16
	pop	rbp
	ret

section .rodata
	printi_s: db "%ld", 10, 0

section .bss

section .note.GNU-stack

