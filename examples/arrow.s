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
	lea	r10, [rbp-16]
	add	r10, 0
	lea	r11, [rbp-8]
	mov	qword [r10], r11
	lea	r10, [rbp-24]
	add	r10, 0
	lea	r11, [rbp-16]
	mov	qword [r10], r11
	lea	r10, [rbp-32]
	lea	r11, [rbp-24]
	mov	qword [r10], r11
	mov	r10, qword [rbp-32]
	add	r10, 0
	mov	r10, qword [r10]
	add	r10, 0
	mov	r10, qword [r10]
	add	r10, 0
	mov	r11, 10
	mov	dword [r10], r11d
	mov	r10, qword [rbp-32]
	add	r10, 0
	mov	r10, qword [r10]
	add	r10, 0
	mov	r10, qword [r10]
	add	r10, 4
	mov	r11, 20
	mov	dword [r10], r11d
	lea	r10, [rbp-8]
	add	r10, 0
	mov	r10d, dword [r10]
	movsx	r10, r10d
	mov	rdi, r10
	call	printi
	lea	r10, [rbp-8]
	add	r10, 4
	mov	r10d, dword [r10]
	movsx	r10, r10d
	mov	rdi, r10
	call	printi
	mov	r10, 0
	mov	eax, r10d
	jmp	.L0
.L0:
	add	rsp, 32
	pop	rbp
	ret

section .rodata
	printi_s: db "%ld", 10, 0

section .note.GNU-stack

