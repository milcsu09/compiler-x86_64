section .text
	extern	printf

printi:
	mov	rsi, rdi
	xor	eax, eax
	mov	rdi, printi_s
	jmp	printf

	global	bubble_sort
bubble_sort:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 48

	mov	qword [rbp-8], rdi
	mov	qword [rbp-16], rsi
	mov	r10, 0
	lea	r11, [rbp-24]
	mov	qword [r11], r10
	jmp	.L2
.L1:
	mov	r10, 0
	lea	r11, [rbp-32]
	mov	qword [r11], r10
	jmp	.L6
.L5:
	mov	r10, qword [rbp-8]
	mov	r11, qword [rbp-32]
	imul	r11, 4
	add	r10, r11
	mov	r10d, dword [r10]
	mov	r11, qword [rbp-8]
	mov	r12, qword [rbp-32]
	push	r10
	mov	r10, 1
	add	r12, r10
	pop	r10
	imul	r12, 4
	add	r11, r12
	mov	r11d, dword [r11]
	cmp	r10d, r11d
	setg	r10b
	test	r10b, r10b
	jz	.L9
	mov	r10, qword [rbp-8]
	mov	r11, qword [rbp-32]
	imul	r11, 4
	add	r10, r11
	mov	r10d, dword [r10]
	movsx	r10, r10d
	lea	r11, [rbp-40]
	mov	qword [r11], r10
	mov	r10, qword [rbp-8]
	mov	r11, qword [rbp-32]
	mov	r12, 1
	add	r11, r12
	imul	r11, 4
	add	r10, r11
	mov	r10d, dword [r10]
	mov	r11, qword [rbp-8]
	mov	r12, qword [rbp-32]
	imul	r12, 4
	add	r11, r12
	mov	dword [r11], r10d
	mov	r10, qword [rbp-40]
	mov	r11, qword [rbp-8]
	mov	r12, qword [rbp-32]
	push	r10
	mov	r10, 1
	add	r12, r10
	pop	r10
	imul	r12, 4
	add	r11, r12
	mov	dword [r11], r10d
.L9:
.L8:
	mov	r10, qword [rbp-32]
	mov	r11, 1
	add	r10, r11
	lea	r11, [rbp-32]
	mov	qword [r11], r10
.L6:
	mov	r10, qword [rbp-32]
	mov	r11, qword [rbp-16]
	mov	r12, qword [rbp-24]
	sub	r11, r12
	mov	r12, 1
	sub	r11, r12
	cmp	r10, r11
	setl	r10b
	test	r10b, r10b
	jnz	.L5
.L7:
.L4:
	mov	r10, qword [rbp-24]
	mov	r11, 1
	add	r10, r11
	lea	r11, [rbp-24]
	mov	qword [r11], r10
.L2:
	mov	r10, qword [rbp-24]
	mov	r11, qword [rbp-16]
	cmp	r10, r11
	setl	r10b
	test	r10b, r10b
	jnz	.L1
.L3:

.L0:
	add	rsp, 48
	pop	rbp
	ret

	global	main
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 32

	mov	r10, 42
	lea	r11, [rbp-20]
	mov	r12, 0
	imul	r12, 4
	add	r11, r12
	mov	dword [r11], r10d
	mov	r10, 27
	lea	r11, [rbp-20]
	mov	r12, 1
	imul	r12, 4
	add	r11, r12
	mov	dword [r11], r10d
	mov	r10, 16
	lea	r11, [rbp-20]
	mov	r12, 2
	imul	r12, 4
	add	r11, r12
	mov	dword [r11], r10d
	mov	r10, 5
	lea	r11, [rbp-20]
	mov	r12, 3
	imul	r12, 4
	add	r11, r12
	mov	dword [r11], r10d
	mov	r10, 30
	lea	r11, [rbp-20]
	mov	r12, 4
	imul	r12, 4
	add	r11, r12
	mov	dword [r11], r10d
	lea	r10, [rbp-20]
	mov	rdi, r10
	mov	r10, 5
	mov	rsi, r10
	lea	r10, [rel bubble_sort]
	call	r10
	mov	r10, 0
	lea	r11, [rbp-32]
	mov	qword [r11], r10
	jmp	.L12
.L11:
	lea	r10, [rbp-20]
	mov	r11, qword [rbp-32]
	imul	r11, 4
	add	r10, r11
	mov	r10d, dword [r10]
	movsx	r10, r10d
	mov	rdi, r10
	call	printi
.L14:
	mov	r10, qword [rbp-32]
	mov	r11, 1
	add	r10, r11
	lea	r11, [rbp-32]
	mov	qword [r11], r10
.L12:
	mov	r10, qword [rbp-32]
	mov	r11, 5
	cmp	r10, r11
	setl	r10b
	test	r10b, r10b
	jnz	.L11
.L13:
	mov	r10, 0
	mov	eax, r10d
	jmp	.L10

.L10:
	add	rsp, 32
	pop	rbp
	ret

section .rodata
	printi_s: db "%ld", 10, 0

section .note.GNU-stack

