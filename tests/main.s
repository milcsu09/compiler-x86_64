section .text
	extern	printf

printi:
	mov	rsi, rdi
	xor	eax, eax
	mov	edi, printi_s
	jmp	printf

	global	main
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 32
	lea	r10, [rbp-32]
	add	r10, 0
	add	r10, 0
	mov	r11, 10
	mov	qword [r10], r11
	lea	r10, [rbp-32]
	add	r10, 0
	add	r10, 8
	mov	r11, 20
	mov	qword [r10], r11
	lea	r10, [rbp-32]
	add	r10, 16
	add	r10, 0
	mov	r11, 30
	mov	qword [r10], r11
	lea	r10, [rbp-32]
	add	r10, 16
	add	r10, 8
	mov	r11, 40
	mov	qword [r10], r11
	lea	r10, [rbp-32]
	add	r10, 0
	add	r10, 0
	mov	r10, qword [r10]
	mov	rdi, r10
	call	printi
	lea	r10, [rbp-32]
	add	r10, 0
	add	r10, 8
	mov	r10, qword [r10]
	mov	rdi, r10
	call	printi
	lea	r10, [rbp-32]
	add	r10, 16
	add	r10, 0
	mov	r10, qword [r10]
	mov	rdi, r10
	call	printi
	lea	r10, [rbp-32]
	add	r10, 16
	add	r10, 8
	mov	r10, qword [r10]
	mov	rdi, r10
	call	printi
	mov	r10, 0
	mov	eax, r10d
	jmp	.L0
.L0:
	add	rsp, 32
	pop	rbp
	ret

section .data
	printi_s: db "%ld", 10, 0

section .note.GNU-stack

