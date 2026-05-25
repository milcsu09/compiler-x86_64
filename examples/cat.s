section .text
	extern	printf

printi:
	mov	rsi, rdi
	xor	eax, eax
	mov	rdi, printi_s
	jmp	printf

	extern	fopen
	extern	fclose
	extern	fgetc
	extern	putchar
	global	cat_file
cat_file:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 32
	mov	qword [rbp-8], rdi
	mov	r10, qword [rbp-8]
	mov	rdi, r10
	mov	r10, LS0
	mov	rsi, r10
	lea	r10, [rel fopen]
	call	r10
	mov	r8, rax
	lea	r9, [rbp-16]
	mov	qword [r9], r8
	mov	r8, qword [rbp-16]
	mov	r9, 0
	cmp	r8, r9
	sete	r8b
	test	r8b, r8b
	jz	.L1
	jmp	.L0
.L1:
	jmp	.L3
.L2:
	mov	r10b, byte [rbp-17]
	mov	dil, r10b
	lea	r10, [rel putchar]
	call	r10
.L3:
	mov	r10, qword [rbp-16]
	mov	rdi, r10
	lea	r10, [rel fgetc]
	call	r10
	mov	r8b, al
	lea	r9, [rbp-17]
	mov	byte [r9], r8b
	movzx	r8, r8b
	mov	r9, 255
	cmp	r8, r9
	setne	r8b
	test	r8b, r8b
	jnz	.L2
.L4:
	mov	r10, qword [rbp-16]
	mov	rdi, r10
	lea	r10, [rel fclose]
	call	r10
.L0:
	xor	rax, rax
	add	rsp, 32
	pop	rbp
	ret

	global	main
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 32
	mov	dword [rbp-4], edi
	mov	qword [rbp-16], rsi
	mov	r8d, dword [rbp-4]
	movsx	r8, r8d
	mov	r9, 2
	cmp	r8, r9
	setl	r8b
	test	r8b, r8b
	jz	.L6
	mov	r8, 1
	mov	eax, r8d
	jmp	.L5
.L6:
	mov	r8, 1
	lea	r9, [rbp-24]
	mov	qword [r9], r8
	jmp	.L8
.L7:
	mov	r10, qword [rbp-16]
	mov	r11, qword [rbp-24]
	imul	r11, 8
	add	r10, r11
	mov	r10, qword [r10]
	mov	rdi, r10
	lea	r10, [rel cat_file]
	call	r10
.L10:
	lea	r8, [rbp-24]
	mov	r9, qword [r8]
	mov	r10, 1
	add	r9, r10
	mov	qword [r8], r9
.L8:
	mov	r8, qword [rbp-24]
	mov	r9d, dword [rbp-4]
	movsx	r9, r9d
	cmp	r8, r9
	setl	r8b
	test	r8b, r8b
	jnz	.L7
.L9:
	mov	r8, 0
	mov	eax, r8d
	jmp	.L5
	xor	rax, rax
.L5:
	add	rsp, 32
	pop	rbp
	ret

section .rodata
	printi_s: db "%ld", 10, 0
	LS0: db 114, 0

section .bss

section .note.GNU-stack

