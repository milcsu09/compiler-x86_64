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
	mov	r10b, byte [rbp-1]
	movzx	r10, r10b
	mov	r11, 2
	mov	cl, r11b
	shl	r10, cl
	mov	r11b, byte [rbp-2]
	movzx	r11, r11b
	mov	r12, 1
	mov	cl, r12b
	shl	r11, cl
	or	r10, r11
	mov	r11b, byte [rbp-3]
	movzx	r11, r11b
	or	r10, r11
	lea	r11, [rbp-4]
	mov	byte [r11], r10b
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
	mov	r10, 0
	lea	r11, [rbp-16]
	mov	qword [r11], r10
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
	jz	.L14
	mov	r10, 46
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
	mov	r10, qword [rbp-16]
	mov	r11, 1
	add	r10, r11
	lea	r11, [rbp-16]
	mov	qword [r11], r10
.L11:
	mov	r10, qword [rbp-16]
	mov	r11, 80
	cmp	r10, r11
	setl	r10b
	test	r10b, r10b
	jnz	.L10
.L12:
	mov	r10, 10
	mov	dil, r10b
	lea	r10, [rel putchar]
	call	r10
	xor	rax, rax
.L9:
	add	rsp, 16
	pop	rbp
	ret

	global	main
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 192
	mov	r10, 0
	lea	r11, [rbp-168]
	mov	qword [r11], r10
	jmp	.L18
.L17:
	mov	r10, 0
	lea	r11, [rbp-160]
	mov	r12, qword [rbp-168]
	add	r11, r12
	mov	byte [r11], r10b
	lea	r11, [rbp-80]
	mov	r12, qword [rbp-168]
	add	r11, r12
	mov	byte [r11], r10b
.L20:
	mov	r10, qword [rbp-168]
	mov	r11, 1
	add	r10, r11
	lea	r11, [rbp-168]
	mov	qword [r11], r10
.L18:
	mov	r10, qword [rbp-168]
	mov	r11, 80
	cmp	r10, r11
	setl	r10b
	test	r10b, r10b
	jnz	.L17
.L19:
	mov	r10, 1
	lea	r11, [rbp-80]
	mov	r12, 79
	add	r11, r12
	mov	byte [r11], r10b
	mov	r10, 0
	lea	r11, [rbp-176]
	mov	qword [r11], r10
	jmp	.L22
.L21:
	lea	r10, [rbp-80]
	mov	rdi, r10
	lea	r10, [rel print_cells]
	call	r10
	mov	r10, 0
	lea	r11, [rbp-168]
	mov	qword [r11], r10
	jmp	.L26
.L25:
	mov	r10, 0
	lea	r11, [rbp-177]
	mov	byte [r11], r10b
	lea	r10, [rbp-80]
	mov	r11, qword [rbp-168]
	add	r10, r11
	mov	r10b, byte [r10]
	lea	r11, [rbp-178]
	mov	byte [r11], r10b
	mov	r10, 0
	lea	r11, [rbp-179]
	mov	byte [r11], r10b
	mov	r10, qword [rbp-168]
	mov	r11, 0
	cmp	r10, r11
	setne	r10b
	test	r10b, r10b
	jz	.L29
	lea	r10, [rbp-80]
	mov	r11, qword [rbp-168]
	mov	r12, 1
	sub	r11, r12
	add	r10, r11
	mov	r10b, byte [r10]
	lea	r11, [rbp-177]
	mov	byte [r11], r10b
.L29:
	mov	r10, qword [rbp-168]
	mov	r11, 79
	cmp	r10, r11
	setne	r10b
	test	r10b, r10b
	jz	.L30
	lea	r10, [rbp-80]
	mov	r11, qword [rbp-168]
	mov	r12, 1
	add	r11, r12
	add	r10, r11
	mov	r10b, byte [r10]
	lea	r11, [rbp-179]
	mov	byte [r11], r10b
.L30:
	mov	r10b, byte [rbp-177]
	mov	dil, r10b
	mov	r10b, byte [rbp-178]
	mov	sil, r10b
	mov	r10b, byte [rbp-179]
	mov	dl, r10b
	lea	r10, [rel rule110]
	call	r10
	mov	r10b, al
	lea	r11, [rbp-160]
	mov	r12, qword [rbp-168]
	add	r11, r12
	mov	byte [r11], r10b
.L28:
	mov	r10, qword [rbp-168]
	mov	r11, 1
	add	r10, r11
	lea	r11, [rbp-168]
	mov	qword [r11], r10
.L26:
	mov	r10, qword [rbp-168]
	mov	r11, 80
	cmp	r10, r11
	setl	r10b
	test	r10b, r10b
	jnz	.L25
.L27:
	mov	r10, 0
	lea	r11, [rbp-168]
	mov	qword [r11], r10
	jmp	.L32
.L31:
	lea	r10, [rbp-160]
	mov	r11, qword [rbp-168]
	add	r10, r11
	mov	r10b, byte [r10]
	lea	r11, [rbp-80]
	mov	r12, qword [rbp-168]
	add	r11, r12
	mov	byte [r11], r10b
.L34:
	mov	r10, qword [rbp-168]
	mov	r11, 1
	add	r10, r11
	lea	r11, [rbp-168]
	mov	qword [r11], r10
.L32:
	mov	r10, qword [rbp-168]
	mov	r11, 80
	cmp	r10, r11
	setl	r10b
	test	r10b, r10b
	jnz	.L31
.L33:
.L24:
	mov	r10, qword [rbp-176]
	mov	r11, 1
	add	r10, r11
	lea	r11, [rbp-176]
	mov	qword [r11], r10
.L22:
	mov	r10, qword [rbp-176]
	mov	r11, 80
	cmp	r10, r11
	setl	r10b
	test	r10b, r10b
	jnz	.L21
.L23:
	mov	r10, 0
	mov	eax, r10d
	jmp	.L16
	xor	rax, rax
.L16:
	add	rsp, 192
	pop	rbp
	ret

section .rodata
	printi_s: db "%ld", 10, 0

section .note.GNU-stack

