section .text
	extern	printf

printi:
	mov	rsi, rdi
	xor	eax, eax
	mov	edi, printi_s
	jmp	printf

	global square
square:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	;
	mov	qword [rbp-8], rdi
	;
	mov	r10, qword [rbp-8]
	mov	r11, qword [rbp-8]
	imul	r10, r11
	mov	rax, r10
	jmp	.L0
	;
.L0:
	add	rsp, 16
	pop	rbp
	ret

	global main
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	;
	mov	dword [rbp-4], edi
	mov	qword [rbp-16], rsi
	;
	mov	r10d, dword [rbp-4]
	movsx	r10, r10d
	mov	r11, 2
	cmp	r10, r11
	sete	r10b
	test	r10b, r10b
	jz	.L2
	mov	r10, 1
	mov	eax, r10d
	jmp	.L1
.L2:
	mov	r10, 0
	mov	eax, r10d
	jmp	.L1
	;
.L1:
	add	rsp, 16
	pop	rbp
	ret

section .data
	printi_s: db "%ld", 10, 0

section .note.GNU-stack

