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
	sub	rsp, 64
	lea	r10, [rbp-16]
	add	r10, 0
	mov	r11, 10
	mov	dword [r10], r11d
	lea	r10, [rbp-32]
	add	r10, 0
	mov	r11, 20
	mov	dword [r10], r11d
	lea	r10, [rbp-48]
	add	r10, 0
	mov	r11, 30
	mov	dword [r10], r11d
	lea	r10, [rbp-16]
	add	r10, 8
	lea	r11, [rbp-32]
	mov	qword [r10], r11
	lea	r10, [rbp-32]
	add	r10, 8
	lea	r11, [rbp-48]
	mov	qword [r10], r11
	lea	r10, [rbp-48]
	add	r10, 8
	mov	r11, 0
	mov	qword [r10], r11
	lea	r10, [rbp-56]
	lea	r11, [rbp-16]
	mov	qword [r10], r11
	jmp	.L2
.L1:
	mov	r10, qword [rbp-56]
	add	r10, 0
	mov	r10d, dword [r10]
	movsx	r10, r10d
	mov	rdi, r10
	call	printi
	lea	r10, [rbp-56]
	mov	r11, qword [rbp-56]
	add	r11, 8
	mov	r11, qword [r11]
	mov	qword [r10], r11
.L2:
	mov	r10, qword [rbp-56]
	test	r10, r10
	jnz	.L1
	mov	r10, 0
	mov	eax, r10d
	jmp	.L0
.L0:
	add	rsp, 64
	pop	rbp
	ret

section .rodata
	printi_s: db "%ld", 10, 0

section .note.GNU-stack

