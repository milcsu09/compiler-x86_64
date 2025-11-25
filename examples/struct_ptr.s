section .text
	extern	printf

printi:
	mov	rsi, rdi
	xor	eax, eax
	mov	rdi, printi_s
	jmp	printf

	global	B_init
B_init:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16

	mov	qword [rbp-8], rdi
	mov	r10, 10
	mov	r11, qword [rbp-8]
	add	r11, 0
	add	r11, 0
	mov	qword [r11], r10
	mov	r10, 20
	mov	r11, qword [rbp-8]
	add	r11, 0
	add	r11, 8
	mov	qword [r11], r10
	mov	r10, 30
	mov	r11, qword [rbp-8]
	add	r11, 16
	add	r11, 0
	mov	qword [r11], r10
	mov	r10, 40
	mov	r11, qword [rbp-8]
	add	r11, 16
	add	r11, 8
	mov	qword [r11], r10

.L0:
	add	rsp, 16
	pop	rbp
	ret

	global	main
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 32

	lea	r10, [rbp-32]
	mov	rdi, r10
	lea	r10, [rel B_init]
	call	r10
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
	jmp	.L1

.L1:
	add	rsp, 32
	pop	rbp
	ret

section .rodata
	printi_s: db "%ld", 10, 0

section .note.GNU-stack

