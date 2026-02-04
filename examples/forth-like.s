section .text
	extern	printf

printi:
	mov	rsi, rdi
	xor	eax, eax
	mov	rdi, printi_s
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
	mov	r10, qword [r10]
	mov	r11, 1
	add	r10, r11
	mov	r11, qword [rbp-8]
	add	r11, 0
	mov	qword [r11], r10
.L0:
	xor	rax, rax
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
	test	r10d, r10d
	jnz	.L2
.L4:
	mov	r10, qword [rbp-8]
	add	r10, 0
	mov	r10, qword [r10]
	mov	r10b, byte [r10]
	movzx	r10d, r10b
	mov	edi, r10d
	lea	r10, [rel isdigit]
	call	r10
	mov	r10d, eax
	test	r10d, r10d
	jz	.L5
	mov	r10, 0
	lea	r11, [rbp-24]
	mov	qword [r11], r10
	jmp	.L7
.L6:
	mov	r10, qword [rbp-24]
	mov	r11, 10
	imul	r10, r11
	lea	r11, [rbp-24]
	mov	qword [r11], r10
	mov	r10, qword [rbp-24]
	mov	r11, qword [rbp-8]
	add	r11, 0
	mov	r11, qword [r11]
	mov	r11b, byte [r11]
	movzx	r11, r11b
	add	r10, r11
	mov	r11, 48
	sub	r10, r11
	lea	r11, [rbp-24]
	mov	qword [r11], r10
	mov	r10, qword [rbp-8]
	mov	rdi, r10
	lea	r10, [rel lexer_advance]
	call	r10
.L7:
	mov	r10, qword [rbp-8]
	add	r10, 0
	mov	r10, qword [r10]
	mov	r10b, byte [r10]
	movzx	r10d, r10b
	mov	edi, r10d
	lea	r10, [rel isdigit]
	call	r10
	mov	r10d, eax
	test	r10d, r10d
	jnz	.L6
.L8:
	mov	r10, 1
	mov	r11, qword [rbp-16]
	add	r11, 0
	mov	dword [r11], r10d
	mov	r10, qword [rbp-24]
	mov	r11, qword [rbp-16]
	add	r11, 8
	mov	qword [r11], r10
	jmp	.L1
.L5:
	mov	r10, qword [rbp-8]
	add	r10, 0
	mov	r10, qword [r10]
	mov	r10b, byte [r10]
	movzx	r10, r10b
	mov	r11, 43
	cmp	r10, r11
	sete	r10b
	test	r10b, r10b
	jz	.L9
	mov	r10, 2
	mov	r11, qword [rbp-16]
	add	r11, 0
	mov	dword [r11], r10d
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
	mov	r11, 45
	cmp	r10, r11
	sete	r10b
	test	r10b, r10b
	jz	.L10
	mov	r10, 3
	mov	r11, qword [rbp-16]
	add	r11, 0
	mov	dword [r11], r10d
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
	mov	r11, 42
	cmp	r10, r11
	sete	r10b
	test	r10b, r10b
	jz	.L11
	mov	r10, 4
	mov	r11, qword [rbp-16]
	add	r11, 0
	mov	dword [r11], r10d
	mov	r10, qword [rbp-8]
	mov	rdi, r10
	lea	r10, [rel lexer_advance]
	call	r10
	jmp	.L1
.L11:
	mov	r10, qword [rbp-8]
	add	r10, 0
	mov	r10, qword [r10]
	mov	r10b, byte [r10]
	movzx	r10, r10b
	mov	r11, 47
	cmp	r10, r11
	sete	r10b
	test	r10b, r10b
	jz	.L12
	mov	r10, 5
	mov	r11, qword [rbp-16]
	add	r11, 0
	mov	dword [r11], r10d
	mov	r10, qword [rbp-8]
	mov	rdi, r10
	lea	r10, [rel lexer_advance]
	call	r10
	jmp	.L1
.L12:
	mov	r10, qword [rbp-8]
	add	r10, 0
	mov	r10, qword [r10]
	mov	r10b, byte [r10]
	movzx	r10, r10b
	mov	r11, 46
	cmp	r10, r11
	sete	r10b
	test	r10b, r10b
	jz	.L13
	mov	r10, 6
	mov	r11, qword [rbp-16]
	add	r11, 0
	mov	dword [r11], r10d
	mov	r10, qword [rbp-8]
	mov	rdi, r10
	lea	r10, [rel lexer_advance]
	call	r10
	jmp	.L1
.L13:
	mov	r10, 0
	mov	r11, qword [rbp-16]
	add	r11, 0
	mov	dword [r11], r10d
.L1:
	xor	rax, rax
	add	rsp, 32
	pop	rbp
	ret

	global	main
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 608
	mov	r10, LS0
	lea	r11, [rbp-8]
	add	r11, 0
	mov	qword [r11], r10
	mov	r10, 4294967295
	lea	r11, [rbp-24]
	add	r11, 0
	mov	dword [r11], r10d
	mov	r10, 0
	lea	r11, [rbp-544]
	mov	qword [r11], r10
	jmp	.L16
.L15:
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
	test	r10b, r10b
	jz	.L18
	lea	r10, [rbp-24]
	add	r10, 8
	mov	r10, qword [r10]
	lea	r11, [rbp-536]
	mov	r12, qword [rbp-544]
	imul	r12, 8
	add	r11, r12
	mov	qword [r11], r10
	mov	r10, qword [rbp-544]
	mov	r11, 1
	add	r10, r11
	lea	r11, [rbp-544]
	mov	qword [r11], r10
	jmp	.L19
.L18:
	lea	r10, [rbp-24]
	add	r10, 0
	mov	r10d, dword [r10]
	mov	r11, 2
	cmp	r10, r11
	sete	r10b
	test	r10b, r10b
	jz	.L20
	lea	r10, [rbp-536]
	mov	r11, qword [rbp-544]
	mov	r12, 1
	sub	r11, r12
	imul	r11, 8
	add	r10, r11
	mov	r10, qword [r10]
	lea	r11, [rbp-552]
	mov	qword [r11], r10
	lea	r10, [rbp-536]
	mov	r11, qword [rbp-544]
	mov	r12, 2
	sub	r11, r12
	imul	r11, 8
	add	r10, r11
	mov	r10, qword [r10]
	lea	r11, [rbp-560]
	mov	qword [r11], r10
	mov	r10, qword [rbp-544]
	mov	r11, 2
	sub	r10, r11
	lea	r11, [rbp-544]
	mov	qword [r11], r10
	mov	r10, qword [rbp-552]
	mov	r11, qword [rbp-560]
	add	r10, r11
	lea	r11, [rbp-536]
	mov	r12, qword [rbp-544]
	imul	r12, 8
	add	r11, r12
	mov	qword [r11], r10
	mov	r10, qword [rbp-544]
	mov	r11, 1
	add	r10, r11
	lea	r11, [rbp-544]
	mov	qword [r11], r10
	jmp	.L21
.L20:
	lea	r10, [rbp-24]
	add	r10, 0
	mov	r10d, dword [r10]
	mov	r11, 3
	cmp	r10, r11
	sete	r10b
	test	r10b, r10b
	jz	.L22
	lea	r10, [rbp-536]
	mov	r11, qword [rbp-544]
	mov	r12, 1
	sub	r11, r12
	imul	r11, 8
	add	r10, r11
	mov	r10, qword [r10]
	lea	r11, [rbp-568]
	mov	qword [r11], r10
	lea	r10, [rbp-536]
	mov	r11, qword [rbp-544]
	mov	r12, 2
	sub	r11, r12
	imul	r11, 8
	add	r10, r11
	mov	r10, qword [r10]
	lea	r11, [rbp-576]
	mov	qword [r11], r10
	mov	r10, qword [rbp-544]
	mov	r11, 2
	sub	r10, r11
	lea	r11, [rbp-544]
	mov	qword [r11], r10
	mov	r10, qword [rbp-568]
	mov	r11, qword [rbp-576]
	sub	r10, r11
	lea	r11, [rbp-536]
	mov	r12, qword [rbp-544]
	imul	r12, 8
	add	r11, r12
	mov	qword [r11], r10
	mov	r10, qword [rbp-544]
	mov	r11, 1
	add	r10, r11
	lea	r11, [rbp-544]
	mov	qword [r11], r10
	jmp	.L23
.L22:
	lea	r10, [rbp-24]
	add	r10, 0
	mov	r10d, dword [r10]
	mov	r11, 4
	cmp	r10, r11
	sete	r10b
	test	r10b, r10b
	jz	.L24
	lea	r10, [rbp-536]
	mov	r11, qword [rbp-544]
	mov	r12, 1
	sub	r11, r12
	imul	r11, 8
	add	r10, r11
	mov	r10, qword [r10]
	lea	r11, [rbp-584]
	mov	qword [r11], r10
	lea	r10, [rbp-536]
	mov	r11, qword [rbp-544]
	mov	r12, 2
	sub	r11, r12
	imul	r11, 8
	add	r10, r11
	mov	r10, qword [r10]
	lea	r11, [rbp-592]
	mov	qword [r11], r10
	mov	r10, qword [rbp-544]
	mov	r11, 2
	sub	r10, r11
	lea	r11, [rbp-544]
	mov	qword [r11], r10
	mov	r10, qword [rbp-584]
	mov	r11, qword [rbp-592]
	imul	r10, r11
	lea	r11, [rbp-536]
	mov	r12, qword [rbp-544]
	imul	r12, 8
	add	r11, r12
	mov	qword [r11], r10
	mov	r10, qword [rbp-544]
	mov	r11, 1
	add	r10, r11
	lea	r11, [rbp-544]
	mov	qword [r11], r10
	jmp	.L25
.L24:
	lea	r10, [rbp-24]
	add	r10, 0
	mov	r10d, dword [r10]
	mov	r11, 5
	cmp	r10, r11
	sete	r10b
	test	r10b, r10b
	jz	.L26
	lea	r10, [rbp-536]
	mov	r11, qword [rbp-544]
	mov	r12, 1
	sub	r11, r12
	imul	r11, 8
	add	r10, r11
	mov	r10, qword [r10]
	lea	r11, [rbp-600]
	mov	qword [r11], r10
	lea	r10, [rbp-536]
	mov	r11, qword [rbp-544]
	mov	r12, 2
	sub	r11, r12
	imul	r11, 8
	add	r10, r11
	mov	r10, qword [r10]
	lea	r11, [rbp-608]
	mov	qword [r11], r10
	mov	r10, qword [rbp-544]
	mov	r11, 2
	sub	r10, r11
	lea	r11, [rbp-544]
	mov	qword [r11], r10
	mov	r10, qword [rbp-600]
	mov	r11, qword [rbp-608]
	mov	rax, r10
	cqo
	idiv	r11
	mov	r10, rax
	lea	r11, [rbp-536]
	mov	r12, qword [rbp-544]
	imul	r12, 8
	add	r11, r12
	mov	qword [r11], r10
	mov	r10, qword [rbp-544]
	mov	r11, 1
	add	r10, r11
	lea	r11, [rbp-544]
	mov	qword [r11], r10
	jmp	.L27
.L26:
	lea	r10, [rbp-24]
	add	r10, 0
	mov	r10d, dword [r10]
	mov	r11, 6
	cmp	r10, r11
	sete	r10b
	test	r10b, r10b
	jz	.L28
	lea	r10, [rbp-536]
	mov	r11, qword [rbp-544]
	mov	r12, 1
	sub	r11, r12
	imul	r11, 8
	add	r10, r11
	mov	r10, qword [r10]
	mov	rdi, r10
	call	printi
	mov	r10, qword [rbp-544]
	mov	r11, 1
	sub	r10, r11
	lea	r11, [rbp-544]
	mov	qword [r11], r10
.L28:
.L27:
.L25:
.L23:
.L21:
.L19:
.L16:
	lea	r10, [rbp-24]
	add	r10, 0
	mov	r10d, dword [r10]
	mov	r11, 0
	cmp	r10, r11
	setne	r10b
	test	r10b, r10b
	jnz	.L15
.L17:
	mov	r10, 0
	mov	eax, r10d
	jmp	.L14
	xor	rax, rax
.L14:
	add	rsp, 608
	pop	rbp
	ret

section .rodata
	printi_s: db "%ld", 10, 0
	LS0: db 49, 49, 32, 51, 32, 42, 32, 49, 32, 43, 32, 51, 53, 32, 43, 32, 46, 0

section .note.GNU-stack

