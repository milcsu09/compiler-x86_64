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
	mov	r10, 10
	lea	r11, [rbp-16]
	add	r11, 0
	mov	qword [r11], r10
	mov	r10, 20
	lea	r11, [rbp-32]
	add	r11, 0
	mov	qword [r11], r10
	mov	r10, 30
	lea	r11, [rbp-48]
	add	r11, 0
	mov	qword [r11], r10
	lea	r10, [rbp-32]
	lea	r11, [rbp-16]
	add	r11, 8
	mov	qword [r11], r10
	lea	r10, [rbp-48]
	lea	r11, [rbp-32]
	add	r11, 8
	mov	qword [r11], r10
	mov	r10, 0
	lea	r11, [rbp-48]
	add	r11, 8
	mov	qword [r11], r10
	lea	r10, [rbp-16]
	lea	r11, [rbp-56]
	mov	qword [r11], r10
	jmp	.L2
.L1:
	mov	r10, qword [rbp-56]
	add	r10, 0
	mov	r10, qword [r10]
	mov	rdi, r10
	call	printi
.L4:
	mov	r10, qword [rbp-56]
	add	r10, 8
	mov	r10, qword [r10]
	lea	r11, [rbp-56]
	mov	qword [r11], r10
.L2:
	mov	r10, qword [rbp-56]
	test	r10, r10
	jnz	.L1
.L3:
.L0:
	xor	rax, rax
	add	rsp, 64
	pop	rbp
	ret

section .rodata
	printi_s: db "%ld", 10, 0

section .bss

section .note.GNU-stack

