section .text
	extern	printf

printi:
	mov	rsi, rdi
	xor	eax, eax
	mov	edi, printi_s
	jmp	printf

	extern	puts
	extern	isspace
	extern	isdigit
	global	lexer_advance
lexer_advance:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	mov	qword [rbp-8], rdi
	mov	r10, qword [rbp-8]
	add	r10, 0
	mov	r11, qword [rbp-8]
	add	r11, 0
	mov	r11, qword [r11]
	mov	r12, 1
	add	r11, r12
	mov	qword [r10], r11
.L0:
	add	rsp, 16
	pop	rbp
	ret

	global	lexer_next
lexer_next:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 32
	mov	qword [rbp-8], rdi
	mov	qword [rbp-16], rsi
	jmp	.L3
.L2:
	mov	r10, qword [rbp-8]
	mov	rdi, r10
	lea	r10, [rel lexer_advance]
	call	r10
.L3:
	mov	r10, qword [rbp-8]
	add	r10, 0
	mov	r10, qword [r10]
	mov	r10b, byte [r10]
	movzx	r10d, r10b
	mov	edi, r10d
	lea	r10, [rel isspace]
	call	r10
	mov	r10d, eax
	test	r10, r10
	jnz	.L2
	mov	r10, qword [rbp-8]
	add	r10, 0
	mov	r10, qword [r10]
	mov	r10b, byte [r10]
	movzx	r10d, r10b
	mov	edi, r10d
	lea	r10, [rel isdigit]
	call	r10
	mov	r10d, eax
	test	r10, r10
	jz	.L4
	lea	r10, [rbp-24]
	mov	r11, 0
	mov	qword [r10], r11
	jmp	.L6
.L5:
	lea	r10, [rbp-24]
	mov	r11, qword [rbp-24]
	mov	r12, 10
	imul	r11, r12
	mov	qword [r10], r11
	lea	r10, [rbp-24]
	mov	r11, qword [rbp-24]
	mov	r12, qword [rbp-8]
	add	r12, 0
	mov	r12, qword [r12]
	mov	r12b, byte [r12]
	movzx	r12, r12b
	add	r11, r12
	mov	r12, 48
	sub	r11, r12
	mov	qword [r10], r11
	mov	r10, qword [rbp-8]
	mov	rdi, r10
	lea	r10, [rel lexer_advance]
	call	r10
.L6:
	mov	r10, qword [rbp-8]
	add	r10, 0
	mov	r10, qword [r10]
	mov	r10b, byte [r10]
	movzx	r10d, r10b
	mov	edi, r10d
	lea	r10, [rel isdigit]
	call	r10
	mov	r10d, eax
	test	r10, r10
	jnz	.L5
	mov	r10, qword [rbp-16]
	add	r10, 0
	mov	r11, 1
	mov	dword [r10], r11d
	mov	r10, qword [rbp-16]
	add	r10, 8
	mov	r11, qword [rbp-24]
	mov	qword [r10], r11
	jmp	.L1
.L4:
	mov	r10, qword [rbp-8]
	add	r10, 0
	mov	r10, qword [r10]
	mov	r10b, byte [r10]
	movzx	r10, r10b
	mov	r11, 43
	cmp	r10, r11
	sete	r10b
	movzx	r10, r10b
	test	r10, r10
	jz	.L7
	mov	r10, qword [rbp-16]
	add	r10, 0
	mov	r11, 2
	mov	dword [r10], r11d
	mov	r10, qword [rbp-8]
	mov	rdi, r10
	lea	r10, [rel lexer_advance]
	call	r10
	jmp	.L1
.L7:
	mov	r10, qword [rbp-8]
	add	r10, 0
	mov	r10, qword [r10]
	mov	r10b, byte [r10]
	movzx	r10, r10b
	mov	r11, 45
	cmp	r10, r11
	sete	r10b
	movzx	r10, r10b
	test	r10, r10
	jz	.L8
	mov	r10, qword [rbp-16]
	add	r10, 0
	mov	r11, 3
	mov	dword [r10], r11d
	mov	r10, qword [rbp-8]
	mov	rdi, r10
	lea	r10, [rel lexer_advance]
	call	r10
	jmp	.L1
.L8:
	mov	r10, qword [rbp-8]
	add	r10, 0
	mov	r10, qword [r10]
	mov	r10b, byte [r10]
	movzx	r10, r10b
	mov	r11, 42
	cmp	r10, r11
	sete	r10b
	movzx	r10, r10b
	test	r10, r10
	jz	.L9
	mov	r10, qword [rbp-16]
	add	r10, 0
	mov	r11, 4
	mov	dword [r10], r11d
	mov	r10, qword [rbp-8]
	mov	rdi, r10
	lea	r10, [rel lexer_advance]
	call	r10
	jmp	.L1
.L9:
	mov	r10, qword [rbp-8]
	add	r10, 0
	mov	r10, qword [r10]
	mov	r10b, byte [r10]
	movzx	r10, r10b
	mov	r11, 47
	cmp	r10, r11
	sete	r10b
	movzx	r10, r10b
	test	r10, r10
	jz	.L10
	mov	r10, qword [rbp-16]
	add	r10, 0
	mov	r11, 5
	mov	dword [r10], r11d
	mov	r10, qword [rbp-8]
	mov	rdi, r10
	lea	r10, [rel lexer_advance]
	call	r10
	jmp	.L1
.L10:
	mov	r10, qword [rbp-8]
	add	r10, 0
	mov	r10, qword [r10]
	mov	r10b, byte [r10]
	movzx	r10, r10b
	mov	r11, 46
	cmp	r10, r11
	sete	r10b
	movzx	r10, r10b
	test	r10, r10
	jz	.L11
	mov	r10, qword [rbp-16]
	add	r10, 0
	mov	r11, 6
	mov	dword [r10], r11d
	mov	r10, qword [rbp-8]
	mov	rdi, r10
	lea	r10, [rel lexer_advance]
	call	r10
	jmp	.L1
.L11:
	mov	r10, qword [rbp-16]
	add	r10, 0
	mov	r11, 0
	mov	dword [r10], r11d
.L1:
	add	rsp, 32
	pop	rbp
	ret

	global	main
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 608
	lea	r10, [rbp-8]
	add	r10, 0
	mov	r11, LS0
	mov	qword [r10], r11
	lea	r10, [rbp-24]
	add	r10, 0
	mov	r11, 4294967295
	mov	dword [r10], r11d
	lea	r10, [rbp-544]
	mov	r11, 0
	mov	qword [r10], r11
	jmp	.L14
.L13:
	lea	r10, [rbp-8]
	mov	rdi, r10
	lea	r10, [rbp-24]
	mov	rsi, r10
	lea	r10, [rel lexer_next]
	call	r10
	lea	r10, [rbp-24]
	add	r10, 0
	mov	r10d, dword [r10]
	mov	r11, 1
	cmp	r10, r11
	sete	r10b
	movzx	r10, r10b
	test	r10, r10
	jz	.L15
	lea	r10, [rbp-536]
	mov	r11, qword [rbp-544]
	imul	r11, 8
	add	r10, r11
	lea	r11, [rbp-24]
	add	r11, 8
	mov	r11, qword [r11]
	mov	qword [r10], r11
	lea	r10, [rbp-544]
	mov	r11, qword [rbp-544]
	mov	r12, 1
	add	r11, r12
	mov	qword [r10], r11
	jmp	.L16
.L15:
	lea	r10, [rbp-24]
	add	r10, 0
	mov	r10d, dword [r10]
	mov	r11, 2
	cmp	r10, r11
	sete	r10b
	movzx	r10, r10b
	test	r10, r10
	jz	.L17
	lea	r10, [rbp-552]
	lea	r11, [rbp-536]
	mov	r12, qword [rbp-544]
	push	r10
	mov	r10, 1
	sub	r12, r10
	pop	r10
	imul	r12, 8
	add	r11, r12
	mov	r11, qword [r11]
	mov	qword [r10], r11
	lea	r10, [rbp-560]
	lea	r11, [rbp-536]
	mov	r12, qword [rbp-544]
	push	r10
	mov	r10, 2
	sub	r12, r10
	pop	r10
	imul	r12, 8
	add	r11, r12
	mov	r11, qword [r11]
	mov	qword [r10], r11
	lea	r10, [rbp-544]
	mov	r11, qword [rbp-544]
	mov	r12, 2
	sub	r11, r12
	mov	qword [r10], r11
	lea	r10, [rbp-536]
	mov	r11, qword [rbp-544]
	imul	r11, 8
	add	r10, r11
	mov	r11, qword [rbp-552]
	mov	r12, qword [rbp-560]
	add	r11, r12
	mov	qword [r10], r11
	lea	r10, [rbp-544]
	mov	r11, qword [rbp-544]
	mov	r12, 1
	add	r11, r12
	mov	qword [r10], r11
	jmp	.L18
.L17:
	lea	r10, [rbp-24]
	add	r10, 0
	mov	r10d, dword [r10]
	mov	r11, 3
	cmp	r10, r11
	sete	r10b
	movzx	r10, r10b
	test	r10, r10
	jz	.L19
	lea	r10, [rbp-568]
	lea	r11, [rbp-536]
	mov	r12, qword [rbp-544]
	push	r10
	mov	r10, 1
	sub	r12, r10
	pop	r10
	imul	r12, 8
	add	r11, r12
	mov	r11, qword [r11]
	mov	qword [r10], r11
	lea	r10, [rbp-576]
	lea	r11, [rbp-536]
	mov	r12, qword [rbp-544]
	push	r10
	mov	r10, 2
	sub	r12, r10
	pop	r10
	imul	r12, 8
	add	r11, r12
	mov	r11, qword [r11]
	mov	qword [r10], r11
	lea	r10, [rbp-544]
	mov	r11, qword [rbp-544]
	mov	r12, 2
	sub	r11, r12
	mov	qword [r10], r11
	lea	r10, [rbp-536]
	mov	r11, qword [rbp-544]
	imul	r11, 8
	add	r10, r11
	mov	r11, qword [rbp-568]
	mov	r12, qword [rbp-576]
	sub	r11, r12
	mov	qword [r10], r11
	lea	r10, [rbp-544]
	mov	r11, qword [rbp-544]
	mov	r12, 1
	add	r11, r12
	mov	qword [r10], r11
	jmp	.L20
.L19:
	lea	r10, [rbp-24]
	add	r10, 0
	mov	r10d, dword [r10]
	mov	r11, 4
	cmp	r10, r11
	sete	r10b
	movzx	r10, r10b
	test	r10, r10
	jz	.L21
	lea	r10, [rbp-584]
	lea	r11, [rbp-536]
	mov	r12, qword [rbp-544]
	push	r10
	mov	r10, 1
	sub	r12, r10
	pop	r10
	imul	r12, 8
	add	r11, r12
	mov	r11, qword [r11]
	mov	qword [r10], r11
	lea	r10, [rbp-592]
	lea	r11, [rbp-536]
	mov	r12, qword [rbp-544]
	push	r10
	mov	r10, 2
	sub	r12, r10
	pop	r10
	imul	r12, 8
	add	r11, r12
	mov	r11, qword [r11]
	mov	qword [r10], r11
	lea	r10, [rbp-544]
	mov	r11, qword [rbp-544]
	mov	r12, 2
	sub	r11, r12
	mov	qword [r10], r11
	lea	r10, [rbp-536]
	mov	r11, qword [rbp-544]
	imul	r11, 8
	add	r10, r11
	mov	r11, qword [rbp-584]
	mov	r12, qword [rbp-592]
	imul	r11, r12
	mov	qword [r10], r11
	lea	r10, [rbp-544]
	mov	r11, qword [rbp-544]
	mov	r12, 1
	add	r11, r12
	mov	qword [r10], r11
	jmp	.L22
.L21:
	lea	r10, [rbp-24]
	add	r10, 0
	mov	r10d, dword [r10]
	mov	r11, 5
	cmp	r10, r11
	sete	r10b
	movzx	r10, r10b
	test	r10, r10
	jz	.L23
	lea	r10, [rbp-600]
	lea	r11, [rbp-536]
	mov	r12, qword [rbp-544]
	push	r10
	mov	r10, 1
	sub	r12, r10
	pop	r10
	imul	r12, 8
	add	r11, r12
	mov	r11, qword [r11]
	mov	qword [r10], r11
	lea	r10, [rbp-608]
	lea	r11, [rbp-536]
	mov	r12, qword [rbp-544]
	push	r10
	mov	r10, 2
	sub	r12, r10
	pop	r10
	imul	r12, 8
	add	r11, r12
	mov	r11, qword [r11]
	mov	qword [r10], r11
	lea	r10, [rbp-544]
	mov	r11, qword [rbp-544]
	mov	r12, 2
	sub	r11, r12
	mov	qword [r10], r11
	lea	r10, [rbp-536]
	mov	r11, qword [rbp-544]
	imul	r11, 8
	add	r10, r11
	mov	r11, qword [rbp-600]
	mov	r12, qword [rbp-608]
	mov	rax, r11
	cqo
	idiv	r12
	mov	r11, rax
	mov	qword [r10], r11
	lea	r10, [rbp-544]
	mov	r11, qword [rbp-544]
	mov	r12, 1
	add	r11, r12
	mov	qword [r10], r11
	jmp	.L24
.L23:
	lea	r10, [rbp-24]
	add	r10, 0
	mov	r10d, dword [r10]
	mov	r11, 6
	cmp	r10, r11
	sete	r10b
	movzx	r10, r10b
	test	r10, r10
	jz	.L25
	lea	r10, [rbp-536]
	mov	r11, qword [rbp-544]
	mov	r12, 1
	sub	r11, r12
	imul	r11, 8
	add	r10, r11
	mov	r10, qword [r10]
	mov	rdi, r10
	call	printi
	lea	r10, [rbp-544]
	mov	r11, qword [rbp-544]
	mov	r12, 1
	sub	r11, r12
	mov	qword [r10], r11
.L25:
.L24:
.L22:
.L20:
.L18:
.L16:
.L14:
	lea	r10, [rbp-24]
	add	r10, 0
	mov	r10d, dword [r10]
	mov	r11, 0
	cmp	r10, r11
	setne	r10b
	movzx	r10, r10b
	test	r10, r10
	jnz	.L13
	mov	r10, 0
	mov	eax, r10d
	jmp	.L12
.L12:
	add	rsp, 608
	pop	rbp
	ret

section .rodata
	printi_s: db "%ld", 10, 0
	LS0: db 49, 49, 32, 51, 32, 42, 32, 49, 32, 43, 32, 51, 53, 32, 43, 32, 46, 0

section .note.GNU-stack

