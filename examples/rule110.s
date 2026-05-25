section .text
	extern	printf

printi:
	mov	rsi, rdi
	xor	eax, eax
	mov	rdi, printi_s
	jmp	printf

	extern	putchar
	global	rule110
rule110:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	mov	byte [rbp-1], dil
	mov	byte [rbp-2], sil
	mov	byte [rbp-3], dl
	mov	r8b, byte [rbp-1]
	movzx	r8, r8b
	mov	r9, 2
	mov	cl, r9b
	shl	r8, cl
	mov	r9b, byte [rbp-2]
	movzx	r9, r9b
	mov	r10, 1
	mov	cl, r10b
	shl	r9, cl
	or	r8, r9
	mov	r9b, byte [rbp-3]
	movzx	r9, r9b
	or	r8, r9
	lea	r9, [rbp-4]
	mov	byte [r9], r8b
	mov	r8b, byte [rbp-4]
	movzx	r8, r8b
	mov	r9, 7
	cmp	r8, r9
	sete	r8b
	test	r8b, r8b
	jz	.L1
	mov	r8, 0
	mov	al, r8b
	jmp	.L0
.L1:
	mov	r8b, byte [rbp-4]
	movzx	r8, r8b
	mov	r9, 6
	cmp	r8, r9
	sete	r8b
	test	r8b, r8b
	jz	.L2
	mov	r8, 1
	mov	al, r8b
	jmp	.L0
.L2:
	mov	r8b, byte [rbp-4]
	movzx	r8, r8b
	mov	r9, 5
	cmp	r8, r9
	sete	r8b
	test	r8b, r8b
	jz	.L3
	mov	r8, 1
	mov	al, r8b
	jmp	.L0
.L3:
	mov	r8b, byte [rbp-4]
	movzx	r8, r8b
	mov	r9, 4
	cmp	r8, r9
	sete	r8b
	test	r8b, r8b
	jz	.L4
	mov	r8, 0
	mov	al, r8b
	jmp	.L0
.L4:
	mov	r8b, byte [rbp-4]
	movzx	r8, r8b
	mov	r9, 3
	cmp	r8, r9
	sete	r8b
	test	r8b, r8b
	jz	.L5
	mov	r8, 1
	mov	al, r8b
	jmp	.L0
.L5:
	mov	r8b, byte [rbp-4]
	movzx	r8, r8b
	mov	r9, 2
	cmp	r8, r9
	sete	r8b
	test	r8b, r8b
	jz	.L6
	mov	r8, 1
	mov	al, r8b
	jmp	.L0
.L6:
	mov	r8b, byte [rbp-4]
	movzx	r8, r8b
	mov	r9, 1
	cmp	r8, r9
	sete	r8b
	test	r8b, r8b
	jz	.L7
	mov	r8, 1
	mov	al, r8b
	jmp	.L0
.L7:
	mov	r8b, byte [rbp-4]
	movzx	r8, r8b
	mov	r9, 0
	cmp	r8, r9
	sete	r8b
	test	r8b, r8b
	jz	.L8
	mov	r8, 0
	mov	al, r8b
	jmp	.L0
.L8:
	xor	rax, rax
.L0:
	add	rsp, 16
	pop	rbp
	ret

	global	print_cells
print_cells:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	mov	qword [rbp-8], rdi
	mov	r8, 0
	lea	r9, [rbp-16]
	mov	qword [r9], r8
	jmp	.L11
.L10:
	mov	r8, qword [rbp-8]
	mov	r9, qword [rbp-16]
	add	r8, r9
	mov	r8b, byte [r8]
	movzx	r8, r8b
	mov	r9, 1
	cmp	r8, r9
	sete	r8b
	test	r8b, r8b
	jz	.L14
	mov	r10, 42
	mov	dil, r10b
	lea	r10, [rel putchar]
	call	r10
	jmp	.L15
.L14:
	mov	r10, 32
	mov	dil, r10b
	lea	r10, [rel putchar]
	call	r10
.L15:
.L13:
	lea	r8, [rbp-16]
	mov	r9, qword [r8]
	mov	r10, 1
	add	r9, r10
	mov	qword [r8], r9
.L11:
	mov	r8, qword [rbp-16]
	mov	r9, 80
	cmp	r8, r9
	setl	r8b
	test	r8b, r8b
	jnz	.L10
.L12:
	mov	r10, 10
	mov	dil, r10b
	lea	r10, [rel putchar]
	call	r10
.L9:
	xor	rax, rax
	add	rsp, 16
	pop	rbp
	ret

	global	main
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 192
	mov	r8, 0
	lea	r9, [rbp-168]
	mov	qword [r9], r8
	jmp	.L18
.L17:
	mov	r8, 0
	lea	r9, [rbp-160]
	mov	r10, qword [rbp-168]
	add	r9, r10
	mov	byte [r9], r8b
	lea	r9, [rbp-80]
	mov	r10, qword [rbp-168]
	add	r9, r10
	mov	byte [r9], r8b
.L20:
	lea	r8, [rbp-168]
	mov	r9, qword [r8]
	mov	r10, 1
	add	r9, r10
	mov	qword [r8], r9
.L18:
	mov	r8, qword [rbp-168]
	mov	r9, 80
	cmp	r8, r9
	setl	r8b
	test	r8b, r8b
	jnz	.L17
.L19:
	mov	r8, 1
	lea	r9, [rbp-80]
	mov	r10, 79
	add	r9, r10
	mov	byte [r9], r8b
	mov	r8, 0
	lea	r9, [rbp-176]
	mov	qword [r9], r8
	jmp	.L22
.L21:
	lea	r10, [rbp-80]
	mov	rdi, r10
	lea	r10, [rel print_cells]
	call	r10
	mov	r8, 0
	lea	r9, [rbp-168]
	mov	qword [r9], r8
	jmp	.L26
.L25:
	mov	r8, 0
	lea	r9, [rbp-177]
	mov	byte [r9], r8b
	lea	r8, [rbp-80]
	mov	r9, qword [rbp-168]
	add	r8, r9
	mov	r8b, byte [r8]
	lea	r9, [rbp-178]
	mov	byte [r9], r8b
	mov	r8, 0
	lea	r9, [rbp-179]
	mov	byte [r9], r8b
	mov	r8, qword [rbp-168]
	mov	r9, 0
	cmp	r8, r9
	setne	r8b
	test	r8b, r8b
	jz	.L29
	lea	r8, [rbp-80]
	mov	r9, qword [rbp-168]
	mov	r10, 1
	sub	r9, r10
	add	r8, r9
	mov	r8b, byte [r8]
	lea	r9, [rbp-177]
	mov	byte [r9], r8b
.L29:
	mov	r8, qword [rbp-168]
	mov	r9, 79
	cmp	r8, r9
	setne	r8b
	test	r8b, r8b
	jz	.L30
	lea	r8, [rbp-80]
	mov	r9, qword [rbp-168]
	mov	r10, 1
	add	r9, r10
	add	r8, r9
	mov	r8b, byte [r8]
	lea	r9, [rbp-179]
	mov	byte [r9], r8b
.L30:
	mov	r10b, byte [rbp-177]
	mov	dil, r10b
	mov	r10b, byte [rbp-178]
	mov	sil, r10b
	mov	r10b, byte [rbp-179]
	mov	dl, r10b
	lea	r10, [rel rule110]
	call	r10
	mov	r8b, al
	lea	r9, [rbp-160]
	mov	r10, qword [rbp-168]
	add	r9, r10
	mov	byte [r9], r8b
.L28:
	lea	r8, [rbp-168]
	mov	r9, qword [r8]
	mov	r10, 1
	add	r9, r10
	mov	qword [r8], r9
.L26:
	mov	r8, qword [rbp-168]
	mov	r9, 80
	cmp	r8, r9
	setl	r8b
	test	r8b, r8b
	jnz	.L25
.L27:
	mov	r8, 0
	lea	r9, [rbp-168]
	mov	qword [r9], r8
	jmp	.L32
.L31:
	lea	r8, [rbp-160]
	mov	r9, qword [rbp-168]
	add	r8, r9
	mov	r8b, byte [r8]
	lea	r9, [rbp-80]
	mov	r10, qword [rbp-168]
	add	r9, r10
	mov	byte [r9], r8b
.L34:
	lea	r8, [rbp-168]
	mov	r9, qword [r8]
	mov	r10, 1
	add	r9, r10
	mov	qword [r8], r9
.L32:
	mov	r8, qword [rbp-168]
	mov	r9, 80
	cmp	r8, r9
	setl	r8b
	test	r8b, r8b
	jnz	.L31
.L33:
.L24:
	lea	r8, [rbp-176]
	mov	r9, qword [r8]
	mov	r10, 1
	add	r9, r10
	mov	qword [r8], r9
.L22:
	mov	r8, qword [rbp-176]
	mov	r9, 80
	cmp	r8, r9
	setl	r8b
	test	r8b, r8b
	jnz	.L21
.L23:
.L16:
	xor	rax, rax
	add	rsp, 192
	pop	rbp
	ret

section .rodata
	printi_s: db "%ld", 10, 0

section .bss

section .note.GNU-stack

