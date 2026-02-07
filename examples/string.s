section .text
	extern	printf

printi:
	mov	rsi, rdi
	xor	eax, eax
	mov	rdi, printi_s
	jmp	printf

	extern	putchar
	global	main
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	mov	r10, LS0
	lea	r11, [rbp-8]
	mov	qword [r11], r10
	jmp	.L2
.L1:
	mov	r10, qword [rbp-8]
	mov	r10b, byte [r10]
	mov	dil, r10b
	lea	r10, [rel putchar]
	call	r10
.L4:
	mov	r10, qword [rbp-8]
	mov	r11, 1
	add	r10, r11
	lea	r11, [rbp-8]
	mov	qword [r11], r10
.L2:
	mov	r10, qword [rbp-8]
	mov	r10b, byte [r10]
	test	r10b, r10b
	jnz	.L1
.L3:
	mov	r10, 0
	mov	eax, r10d
	jmp	.L0
	xor	rax, rax
.L0:
	add	rsp, 16
	pop	rbp
	ret

section .rodata
	printi_s: db "%ld", 10, 0
	LS0: db 72, 101, 108, 108, 111, 44, 32, 119, 111, 114, 108, 100, 33, 10, 0

section .bss

section .note.GNU-stack

