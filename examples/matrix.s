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
	mov	r10, 1
	lea	r11, [rbp-48]
	mov	r12, 0
	imul	r12, 24
	add	r11, r12
	mov	r12, 0
	imul	r12, 8
	add	r11, r12
	mov	qword [r11], r10
	mov	r10, 2
	lea	r11, [rbp-48]
	mov	r12, 0
	imul	r12, 24
	add	r11, r12
	mov	r12, 1
	imul	r12, 8
	add	r11, r12
	mov	qword [r11], r10
	mov	r10, 3
	lea	r11, [rbp-48]
	mov	r12, 0
	imul	r12, 24
	add	r11, r12
	mov	r12, 2
	imul	r12, 8
	add	r11, r12
	mov	qword [r11], r10
	mov	r10, 4
	lea	r11, [rbp-48]
	mov	r12, 1
	imul	r12, 24
	add	r11, r12
	mov	r12, 0
	imul	r12, 8
	add	r11, r12
	mov	qword [r11], r10
	mov	r10, 5
	lea	r11, [rbp-48]
	mov	r12, 1
	imul	r12, 24
	add	r11, r12
	mov	r12, 1
	imul	r12, 8
	add	r11, r12
	mov	qword [r11], r10
	mov	r10, 6
	lea	r11, [rbp-48]
	mov	r12, 1
	imul	r12, 24
	add	r11, r12
	mov	r12, 2
	imul	r12, 8
	add	r11, r12
	mov	qword [r11], r10
	lea	r10, [rbp-48]
	mov	r11, 0
	imul	r11, 24
	add	r10, r11
	mov	r11, 0
	imul	r11, 8
	add	r10, r11
	mov	r10, qword [r10]
	mov	rdi, r10
	call	printi
	lea	r10, [rbp-48]
	mov	r11, 0
	imul	r11, 24
	add	r10, r11
	mov	r11, 1
	imul	r11, 8
	add	r10, r11
	mov	r10, qword [r10]
	mov	rdi, r10
	call	printi
	lea	r10, [rbp-48]
	mov	r11, 0
	imul	r11, 24
	add	r10, r11
	mov	r11, 2
	imul	r11, 8
	add	r10, r11
	mov	r10, qword [r10]
	mov	rdi, r10
	call	printi
	lea	r10, [rbp-48]
	mov	r11, 1
	imul	r11, 24
	add	r10, r11
	mov	r11, 0
	imul	r11, 8
	add	r10, r11
	mov	r10, qword [r10]
	mov	rdi, r10
	call	printi
	lea	r10, [rbp-48]
	mov	r11, 1
	imul	r11, 24
	add	r10, r11
	mov	r11, 1
	imul	r11, 8
	add	r10, r11
	mov	r10, qword [r10]
	mov	rdi, r10
	call	printi
	lea	r10, [rbp-48]
	mov	r11, 1
	imul	r11, 24
	add	r10, r11
	mov	r11, 2
	imul	r11, 8
	add	r10, r11
	mov	r10, qword [r10]
	mov	rdi, r10
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

