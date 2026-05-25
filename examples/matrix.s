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
	mov	r8, 1
	lea	r9, [rbp-48]
	mov	r10, 0
	imul	r10, 24
	add	r9, r10
	mov	r10, 0
	imul	r10, 8
	add	r9, r10
	mov	qword [r9], r8
	mov	r8, 2
	lea	r9, [rbp-48]
	mov	r10, 0
	imul	r10, 24
	add	r9, r10
	mov	r10, 1
	imul	r10, 8
	add	r9, r10
	mov	qword [r9], r8
	mov	r8, 3
	lea	r9, [rbp-48]
	mov	r10, 0
	imul	r10, 24
	add	r9, r10
	mov	r10, 2
	imul	r10, 8
	add	r9, r10
	mov	qword [r9], r8
	mov	r8, 4
	lea	r9, [rbp-48]
	mov	r10, 1
	imul	r10, 24
	add	r9, r10
	mov	r10, 0
	imul	r10, 8
	add	r9, r10
	mov	qword [r9], r8
	mov	r8, 5
	lea	r9, [rbp-48]
	mov	r10, 1
	imul	r10, 24
	add	r9, r10
	mov	r10, 1
	imul	r10, 8
	add	r9, r10
	mov	qword [r9], r8
	mov	r8, 6
	lea	r9, [rbp-48]
	mov	r10, 1
	imul	r10, 24
	add	r9, r10
	mov	r10, 2
	imul	r10, 8
	add	r9, r10
	mov	qword [r9], r8
	lea	r8, [rbp-48]
	mov	r9, 0
	imul	r9, 24
	add	r8, r9
	mov	r9, 0
	imul	r9, 8
	add	r8, r9
	mov	r8, qword [r8]
	mov	rdi, r8
	call	printi
	lea	r8, [rbp-48]
	mov	r9, 0
	imul	r9, 24
	add	r8, r9
	mov	r9, 1
	imul	r9, 8
	add	r8, r9
	mov	r8, qword [r8]
	mov	rdi, r8
	call	printi
	lea	r8, [rbp-48]
	mov	r9, 0
	imul	r9, 24
	add	r8, r9
	mov	r9, 2
	imul	r9, 8
	add	r8, r9
	mov	r8, qword [r8]
	mov	rdi, r8
	call	printi
	lea	r8, [rbp-48]
	mov	r9, 1
	imul	r9, 24
	add	r8, r9
	mov	r9, 0
	imul	r9, 8
	add	r8, r9
	mov	r8, qword [r8]
	mov	rdi, r8
	call	printi
	lea	r8, [rbp-48]
	mov	r9, 1
	imul	r9, 24
	add	r8, r9
	mov	r9, 1
	imul	r9, 8
	add	r8, r9
	mov	r8, qword [r8]
	mov	rdi, r8
	call	printi
	lea	r8, [rbp-48]
	mov	r9, 1
	imul	r9, 24
	add	r8, r9
	mov	r9, 2
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

section .bss

section .note.GNU-stack

