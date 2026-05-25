section .text
	extern	printf

printi:
	mov	rsi, rdi
	xor	eax, eax
	mov	rdi, printi_s
	jmp	printf

	global	f1
f1:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	mov	qword [rbp-8], rdi
	mov	r8, qword [rbp-8]
	mov	rax, r8
	jmp	.L0
	xor	rax, rax
.L0:
	add	rsp, 16
	pop	rbp
	ret

	global	f2
f2:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	mov	qword [rbp-8], rdi
	mov	r8, qword [rbp-8]
	mov	r9, 1
	add	r8, r9
	mov	rax, r8
	jmp	.L1
	xor	rax, rax
.L1:
	add	rsp, 16
	pop	rbp
	ret

	global	f3
f3:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	mov	qword [rbp-8], rdi
	mov	r8, qword [rbp-8]
	mov	r9, 2
	imul	r8, r9
	mov	rax, r8
	jmp	.L2
	xor	rax, rax
.L2:
	add	rsp, 16
	pop	rbp
	ret

	global	f4
f4:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	mov	qword [rbp-8], rdi
	mov	r8, qword [rbp-8]
	mov	r9, qword [rbp-8]
	imul	r8, r9
	mov	rax, r8
	jmp	.L3
	xor	rax, rax
.L3:
	add	rsp, 16
	pop	rbp
	ret

	global	main
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 32
	lea	r8, [rel f1]
	lea	r9, [rbp-32]
	mov	r10, 0
	imul	r10, 8
	add	r9, r10
	mov	qword [r9], r8
	lea	r8, [rel f2]
	lea	r9, [rbp-32]
	mov	r10, 1
	imul	r10, 8
	add	r9, r10
	mov	qword [r9], r8
	lea	r8, [rel f3]
	lea	r9, [rbp-32]
	mov	r10, 2
	imul	r10, 8
	add	r9, r10
	mov	qword [r9], r8
	lea	r8, [rel f4]
	lea	r9, [rbp-32]
	mov	r10, 3
	imul	r10, 8
	add	r9, r10
	mov	qword [r9], r8
	mov	r10, 10
	mov	rdi, r10
	lea	r10, [rbp-32]
	mov	r11, 0
	imul	r11, 8
	add	r10, r11
	mov	r10, qword [r10]
	call	r10
	mov	r8, rax
	mov	rdi, r8
	call	printi
	mov	r10, 10
	mov	rdi, r10
	lea	r10, [rbp-32]
	mov	r11, 1
	imul	r11, 8
	add	r10, r11
	mov	r10, qword [r10]
	call	r10
	mov	r8, rax
	mov	rdi, r8
	call	printi
	mov	r10, 10
	mov	rdi, r10
	lea	r10, [rbp-32]
	mov	r11, 2
	imul	r11, 8
	add	r10, r11
	mov	r10, qword [r10]
	call	r10
	mov	r8, rax
	mov	rdi, r8
	call	printi
	mov	r10, 10
	mov	rdi, r10
	lea	r10, [rbp-32]
	mov	r11, 3
	imul	r11, 8
	add	r10, r11
	mov	r10, qword [r10]
	call	r10
	mov	r8, rax
	mov	rdi, r8
	call	printi
.L4:
	xor	rax, rax
	add	rsp, 32
	pop	rbp
	ret

section .rodata
	printi_s: db "%ld", 10, 0

section .bss

section .note.GNU-stack

