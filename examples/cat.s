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
	lea	r10, [rbp-16]
	push	r10
	mov	r11, qword [rbp-8]
	mov	rdi, r11
	mov	r11, LS0
	mov	rsi, r11
	sub	rsp, 8
	lea	r11, [rel fopen]
	call	r11
	add	rsp, 8
	pop	r10
	mov	r11, rax
	mov	qword [r10], r11
	mov	r10, qword [rbp-16]
	mov	r11, 0
	cmp	r10, r11
	sete	r10b
	movzx	r10, r10b
	test	r10, r10
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
	lea	r10, [rbp-17]
	push	r10
	mov	r11, qword [rbp-16]
	mov	rdi, r11
	sub	rsp, 8
	lea	r11, [rel fgetc]
	call	r11
	add	rsp, 8
	pop	r10
	mov	r11b, al
	mov	byte [r10], r11b
	movzx	r11, r11b
	mov	r10, 255
	cmp	r11, r10
	setne	r11b
	movzx	r11, r11b
	test	r11, r11
	jnz	.L2
.L4:
	mov	r10, qword [rbp-16]
	mov	rdi, r10
	lea	r10, [rel fclose]
	call	r10
.L0:
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
	mov	r10d, dword [rbp-4]
	movsx	r10, r10d
	mov	r11, 2
	cmp	r10, r11
	setl	r10b
	movzx	r10, r10b
	test	r10, r10
	jz	.L6
	mov	r10, 1
	mov	eax, r10d
	jmp	.L5
.L6:
	lea	r10, [rbp-24]
	mov	r11, 1
	mov	qword [r10], r11
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
	lea	r10, [rbp-24]
	mov	r11, qword [rbp-24]
	mov	r12, 1
	add	r11, r12
	mov	qword [r10], r11
.L8:
	mov	r10, qword [rbp-24]
	mov	r11d, dword [rbp-4]
	movsx	r11, r11d
	cmp	r10, r11
	setl	r10b
	movzx	r10, r10b
	test	r10, r10
	jnz	.L7
.L9:
	mov	r10, 0
	mov	eax, r10d
	jmp	.L5
.L5:
	add	rsp, 32
	pop	rbp
	ret

section .rodata
	printi_s: db "%ld", 10, 0
	LS0: db 114, 0

section .note.GNU-stack

