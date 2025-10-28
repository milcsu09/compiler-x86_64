section .text
	extern	printf

printi:
	mov	rsi, rdi
	xor	eax, eax
	mov	edi, printi_s
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
	lea	r10, [rbp-4]
	mov	r11b, byte [rbp-1]
	movzx	r11, r11b
	mov	r12, 2
	mov	cl, r12b
	shl	r11, cl
	mov	r12b, byte [rbp-2]
	movzx	r12, r12b
	push	r10
	mov	r10, 1
	mov	cl, r10b
	shl	r12, cl
	pop	r10
	or	r11, r12
	mov	r12b, byte [rbp-3]
	movzx	r12, r12b
	or	r11, r12
	mov	byte [r10], r11b
	mov	r10b, byte [rbp-4]
	movzx	r10, r10b
	mov	r11, 7
	cmp	r10, r11
	sete	r10b
	test	r10b, r10b
	jz	.L1
	mov	r10, 0
	mov	al, r10b
	jmp	.L0
.L1:
	mov	r10b, byte [rbp-4]
	movzx	r10, r10b
	mov	r11, 6
	cmp	r10, r11
	sete	r10b
	test	r10b, r10b
	jz	.L2
	mov	r10, 1
	mov	al, r10b
	jmp	.L0
.L2:
	mov	r10b, byte [rbp-4]
	movzx	r10, r10b
	mov	r11, 5
	cmp	r10, r11
	sete	r10b
	test	r10b, r10b
	jz	.L3
	mov	r10, 1
	mov	al, r10b
	jmp	.L0
.L3:
	mov	r10b, byte [rbp-4]
	movzx	r10, r10b
	mov	r11, 4
	cmp	r10, r11
	sete	r10b
	test	r10b, r10b
	jz	.L4
	mov	r10, 0
	mov	al, r10b
	jmp	.L0
.L4:
	mov	r10b, byte [rbp-4]
	movzx	r10, r10b
	mov	r11, 3
	cmp	r10, r11
	sete	r10b
	test	r10b, r10b
	jz	.L5
	mov	r10, 1
	mov	al, r10b
	jmp	.L0
.L5:
	mov	r10b, byte [rbp-4]
	movzx	r10, r10b
	mov	r11, 2
	cmp	r10, r11
	sete	r10b
	test	r10b, r10b
	jz	.L6
	mov	r10, 1
	mov	al, r10b
	jmp	.L0
.L6:
	mov	r10b, byte [rbp-4]
	movzx	r10, r10b
	mov	r11, 1
	cmp	r10, r11
	sete	r10b
	test	r10b, r10b
	jz	.L7
	mov	r10, 1
	mov	al, r10b
	jmp	.L0
.L7:
	mov	r10b, byte [rbp-4]
	movzx	r10, r10b
	mov	r11, 0
	cmp	r10, r11
	sete	r10b
	test	r10b, r10b
	jz	.L8
	mov	r10, 0
	mov	al, r10b
	jmp	.L0
.L8:
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
	lea	r10, [rbp-16]
	mov	r11, 0
	mov	qword [r10], r11
	jmp	.L11
.L10:
	mov	r10, qword [rbp-8]
	mov	r11, qword [rbp-16]
	add	r10, r11
	mov	r10b, byte [r10]
	movzx	r10, r10b
	mov	r11, 1
	cmp	r10, r11
	sete	r10b
	test	r10b, r10b
	jz	.L12
	mov	r10, 42
	mov	dil, r10b
	lea	r10, [rel putchar]
	call	r10
	jmp	.L13
.L12:
	mov	r10, 32
	mov	dil, r10b
	lea	r10, [rel putchar]
	call	r10
.L13:
	lea	r10, [rbp-16]
	mov	r11, qword [rbp-16]
	mov	r12, 1
	add	r11, r12
	mov	qword [r10], r11
.L11:
	mov	r10, qword [rbp-16]
	mov	r11, 80
	cmp	r10, r11
	setl	r10b
	test	r10b, r10b
	jnz	.L10
	mov	r10, 10
	mov	dil, r10b
	lea	r10, [rel putchar]
	call	r10
.L9:
	add	rsp, 16
	pop	rbp
	ret

	global	main
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 192
	lea	r10, [rbp-168]
	mov	r11, 0
	mov	qword [r10], r11
	jmp	.L16
.L15:
	lea	r10, [rbp-80]
	mov	r11, qword [rbp-168]
	add	r10, r11
	lea	r11, [rbp-160]
	mov	r12, qword [rbp-168]
	add	r11, r12
	mov	r12, 0
	mov	byte [r11], r12b
	mov	byte [r10], r12b
	lea	r10, [rbp-168]
	mov	r11, qword [rbp-168]
	mov	r12, 1
	add	r11, r12
	mov	qword [r10], r11
.L16:
	mov	r10, qword [rbp-168]
	mov	r11, 80
	cmp	r10, r11
	setl	r10b
	test	r10b, r10b
	jnz	.L15
	lea	r10, [rbp-80]
	mov	r11, 79
	add	r10, r11
	mov	r11, 1
	mov	byte [r10], r11b
	lea	r10, [rbp-176]
	mov	r11, 0
	mov	qword [r10], r11
	jmp	.L18
.L17:
	lea	r10, [rbp-80]
	mov	rdi, r10
	lea	r10, [rel print_cells]
	call	r10
	lea	r10, [rbp-168]
	mov	r11, 0
	mov	qword [r10], r11
	jmp	.L20
.L19:
	lea	r10, [rbp-177]
	mov	r11, 0
	mov	byte [r10], r11b
	lea	r10, [rbp-178]
	lea	r11, [rbp-80]
	mov	r12, qword [rbp-168]
	add	r11, r12
	mov	r11b, byte [r11]
	mov	byte [r10], r11b
	lea	r10, [rbp-179]
	mov	r11, 0
	mov	byte [r10], r11b
	mov	r10, qword [rbp-168]
	mov	r11, 0
	cmp	r10, r11
	setne	r10b
	test	r10b, r10b
	jz	.L21
	lea	r10, [rbp-177]
	lea	r11, [rbp-80]
	mov	r12, qword [rbp-168]
	push	r10
	mov	r10, 1
	sub	r12, r10
	pop	r10
	add	r11, r12
	mov	r11b, byte [r11]
	mov	byte [r10], r11b
.L21:
	mov	r10, qword [rbp-168]
	mov	r11, 79
	cmp	r10, r11
	setne	r10b
	test	r10b, r10b
	jz	.L22
	lea	r10, [rbp-179]
	lea	r11, [rbp-80]
	mov	r12, qword [rbp-168]
	push	r10
	mov	r10, 1
	add	r12, r10
	pop	r10
	add	r11, r12
	mov	r11b, byte [r11]
	mov	byte [r10], r11b
.L22:
	lea	r10, [rbp-160]
	mov	r11, qword [rbp-168]
	add	r10, r11
	push	r10
	mov	r11b, byte [rbp-177]
	mov	dil, r11b
	mov	r11b, byte [rbp-178]
	mov	sil, r11b
	mov	r11b, byte [rbp-179]
	mov	dl, r11b
	sub	rsp, 8
	lea	r11, [rel rule110]
	call	r11
	add	rsp, 8
	pop	r10
	mov	r11b, al
	mov	byte [r10], r11b
	lea	r10, [rbp-168]
	mov	r11, qword [rbp-168]
	mov	r12, 1
	add	r11, r12
	mov	qword [r10], r11
.L20:
	mov	r10, qword [rbp-168]
	mov	r11, 80
	cmp	r10, r11
	setl	r10b
	test	r10b, r10b
	jnz	.L19
	lea	r10, [rbp-168]
	mov	r11, 0
	mov	qword [r10], r11
	jmp	.L24
.L23:
	lea	r10, [rbp-80]
	mov	r11, qword [rbp-168]
	add	r10, r11
	lea	r11, [rbp-160]
	mov	r12, qword [rbp-168]
	add	r11, r12
	mov	r11b, byte [r11]
	mov	byte [r10], r11b
	lea	r10, [rbp-168]
	mov	r11, qword [rbp-168]
	mov	r12, 1
	add	r11, r12
	mov	qword [r10], r11
.L24:
	mov	r10, qword [rbp-168]
	mov	r11, 80
	cmp	r10, r11
	setl	r10b
	test	r10b, r10b
	jnz	.L23
	lea	r10, [rbp-176]
	mov	r11, qword [rbp-176]
	mov	r12, 1
	add	r11, r12
	mov	qword [r10], r11
.L18:
	mov	r10, qword [rbp-176]
	mov	r11, 80
	cmp	r10, r11
	setl	r10b
	test	r10b, r10b
	jnz	.L17
	mov	r10, 0
	mov	eax, r10d
	jmp	.L14
.L14:
	add	rsp, 192
	pop	rbp
	ret

section .rodata
	printi_s: db "%ld", 10, 0

section .note.GNU-stack

