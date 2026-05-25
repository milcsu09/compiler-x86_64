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
	sub	rsp, 32
	lea	r8, [rbp-16]
	lea	r9, [rbp-24]
	mov	qword [r9], r8
	mov	r8, LS0
	mov	r9, qword [rbp-24]
	add	r9, 0
	mov	qword [r9], r8
	mov	r8, 42
	mov	r9, qword [rbp-24]
	add	r9, 8
	mov	qword [r9], r8
	lea	r8, [rbp-16]
	add	r8, 0
	mov	r8, qword [r8]
	mov	rdi, r8
	call	printi
	lea	r8, [rbp-16]
	add	r8, 8
	mov	r8, qword [r8]
	mov	rdi, r8
	call	printi
.L0:
	xor	rax, rax
	add	rsp, 32
	pop	rbp
	ret

section .rodata
	printi_s: db "%ld", 10, 0
	LS0: db 74, 111, 104, 110, 32, 68, 111, 101, 0

section .bss

section .note.GNU-stack

