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
	sub	rsp, 0
	;
	;
	lea	r10, [rel square]
	mov	rdi, r10
	call	printi
	lea	r10, [rel square]
	mov	rdi, r10
	call	printi
	lea	r10, [rel square]
	mov	rdi, r10
	call	printi
	lea	r10, [rel square]
	mov	rdi, r10
	call	printi
	lea	r10, [rel square]
	mov	rdi, r10
	call	printi
	mov	r10, 0
	mov	eax, r10d
	jmp	.L1
	;
.L1:
	add	rsp, 0
	pop	rbp
	ret

section .data
	printi_s: db "%ld", 10, 0

section .note.GNU-stack

