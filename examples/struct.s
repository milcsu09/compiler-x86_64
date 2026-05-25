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
	sub	rsp, 48
	mov	r8, 32
	lea	r9, [rbp-48]
	add	r9, 0
	mov	qword [r9], r8
	mov	r8, LS0
	lea	r9, [rbp-48]
	add	r9, 8
	mov	qword [r9], r8
	mov	r8, 10
	lea	r9, [rbp-48]
	add	r9, 16
	mov	r10, 0
	imul	r10, 8
	add	r9, r10
	mov	qword [r9], r8
	mov	r8, 20
	lea	r9, [rbp-48]
	add	r9, 16
	mov	r10, 1
	imul	r10, 8
	add	r9, r10
	mov	qword [r9], r8
	mov	r8, 30
	lea	r9, [rbp-48]
	add	r9, 16
	mov	r10, 2
	imul	r10, 8
	add	r9, r10
	mov	qword [r9], r8
	mov	r8, 40
	lea	r9, [rbp-48]
	add	r9, 16
	mov	r10, 3
	imul	r10, 8
	add	r9, r10
	mov	qword [r9], r8
	lea	r8, [rbp-48]
	add	r8, 0
	mov	r8, qword [r8]
	mov	rdi, r8
	call	printi
	lea	r8, [rbp-48]
	add	r8, 8
	mov	r8, qword [r8]
	mov	rdi, r8
	call	printi
	lea	r8, [rbp-48]
	add	r8, 16
	mov	r9, 0
	imul	r9, 8
	add	r8, r9
	mov	r8, qword [r8]
	mov	rdi, r8
	call	printi
	lea	r8, [rbp-48]
	add	r8, 16
	mov	r9, 1
	imul	r9, 8
	add	r8, r9
	mov	r8, qword [r8]
	mov	rdi, r8
	call	printi
	lea	r8, [rbp-48]
	add	r8, 16
	mov	r9, 2
	imul	r9, 8
	add	r8, r9
	mov	r8, qword [r8]
	mov	rdi, r8
	call	printi
	lea	r8, [rbp-48]
	add	r8, 16
	mov	r9, 3
	imul	r9, 8
	add	r8, r9
	mov	r8, qword [r8]
	mov	rdi, r8
	call	printi
.L0:
	xor	rax, rax
	add	rsp, 48
	pop	rbp
	ret

section .rodata
	printi_s: db "%ld", 10, 0
	LS0: db 72, 101, 108, 108, 111, 44, 32, 119, 111, 114, 108, 100, 33, 0

section .bss

section .note.GNU-stack

