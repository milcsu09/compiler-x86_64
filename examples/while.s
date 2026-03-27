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
	sub	rsp, 16
	mov	r10, 0
	lea	r11, [rbp-8]
	mov	qword [r11], r10
	jmp	.L2
.L1:
	mov	r10, qword [rbp-8]
	mov	rdi, r10
	call	printi
	mov	r10, qword [rbp-8]
	mov	r11, 1
	add	r10, r11
	lea	r11, [rbp-8]
	mov	qword [r11], r10
.L2:
	mov	r10, qword [rbp-8]
	mov	r11, 10
	cmp	r10, r11
	setl	r10b
	test	r10b, r10b
	jnz	.L1
.L3:
.L0:
	xor	rax, rax
	add	rsp, 16
	pop	rbp
	ret

section .rodata
	printi_s: db "%ld", 10, 0

section .bss

section .note.GNU-stack

