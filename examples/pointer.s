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
	lea	r8, [rbp-8]
	lea	r9, [rbp-16]
	mov	qword [r9], r8
	mov	r8, 10
	mov	r9, qword [rbp-16]
	mov	qword [r9], r8
	mov	r8, qword [rbp-8]
	mov	rdi, r8
	call	printi
	mov	r8, qword [rbp-16]
	mov	rdi, r8
	call	printi
	mov	r8, qword [rbp-16]
	mov	r8, qword [r8]
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

