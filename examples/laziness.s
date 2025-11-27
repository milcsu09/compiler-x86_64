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
	mov	r10, qword [rbp-8]
	test	r10, r10
	jz	.L1
	mov	r10, qword [rbp-8]
	mov	r10d, dword [r10]
	movsx	r10, r10d
	mov	r11, 10
	cmp	r10, r11
	sete	r10b
	movzx	r10, r10b
.L1:
	mov	rdi, r10
	call	printi
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

section .note.GNU-stack

