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

	lea	r10, [rbp-8]
	lea	r11, [rbp-16]
	add	r11, 0
	mov	qword [r11], r10
	lea	r10, [rbp-16]
	lea	r11, [rbp-24]
	add	r11, 0
	mov	qword [r11], r10
	lea	r10, [rbp-24]
	lea	r11, [rbp-32]
	mov	qword [r11], r10
	mov	r10, 10
	mov	r11, qword [rbp-32]
	add	r11, 0
	mov	r11, qword [r11]
	add	r11, 0
	mov	r11, qword [r11]
	add	r11, 0
	mov	dword [r11], r10d
	mov	r10, 20
	mov	r11, qword [rbp-32]
	add	r11, 0
	mov	r11, qword [r11]
	add	r11, 0
	mov	r11, qword [r11]
	add	r11, 4
	mov	dword [r11], r10d
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

