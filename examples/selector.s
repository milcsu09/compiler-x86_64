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
	mov	r10, 1
	lea	r11, [rbp-8]
	mov	qword [r11], r10
	mov	r10, 2
	lea	r11, [rbp-16]
	mov	qword [r11], r10
	mov	r10, 3
	lea	r11, [rbp-24]
	mov	qword [r11], r10
	mov	r10, qword [rbp-8]
	test	r10, r10
	jnz	.L2
	mov	r10, qword [rbp-16]
.L2:
	test	r10, r10
	jnz	.L1
	mov	r10, qword [rbp-24]
.L1:
	mov	rdi, r10
	call	printi
	mov	r10, qword [rbp-8]
	test	r10, r10
	jz	.L4
	mov	r10, qword [rbp-16]
.L4:
	test	r10, r10
	jz	.L3
	mov	r10, qword [rbp-24]
.L3:
	mov	rdi, r10
	call	printi
	mov	r10, 0
	lea	r11, [rbp-8]
	mov	qword [r11], r10
	mov	r10, 1
	lea	r11, [rbp-16]
	mov	qword [r11], r10
	mov	r10, 2
	lea	r11, [rbp-24]
	mov	qword [r11], r10
	mov	r10, qword [rbp-8]
	test	r10, r10
	jnz	.L6
	mov	r10, qword [rbp-16]
.L6:
	test	r10, r10
	jnz	.L5
	mov	r10, qword [rbp-24]
.L5:
	mov	rdi, r10
	call	printi
	mov	r10, qword [rbp-8]
	test	r10, r10
	jz	.L8
	mov	r10, qword [rbp-16]
.L8:
	test	r10, r10
	jz	.L7
	mov	r10, qword [rbp-24]
.L7:
	mov	rdi, r10
	call	printi
	mov	r10, 0
	mov	eax, r10d
	jmp	.L0
	xor	rax, rax
.L0:
	add	rsp, 32
	pop	rbp
	ret

section .rodata
	printi_s: db "%ld", 10, 0

section .bss

section .note.GNU-stack

