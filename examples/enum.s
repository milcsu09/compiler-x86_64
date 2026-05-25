section .text
	extern	printf

printi:
	mov	rsi, rdi
	xor	eax, eax
	mov	rdi, printi_s
	jmp	printf

	extern	puts
	global	fruit_string
fruit_string:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	mov	qword [rbp-8], rdi
	mov	r8, qword [rbp-8]
	mov	r9, 0
	cmp	r8, r9
	sete	r8b
	test	r8b, r8b
	jz	.L1
	mov	r8, LS0
	mov	rax, r8
	jmp	.L0
.L1:
	mov	r8, qword [rbp-8]
	mov	r9, 1
	cmp	r8, r9
	sete	r8b
	test	r8b, r8b
	jz	.L2
	mov	r8, LS1
	mov	rax, r8
	jmp	.L0
.L2:
	mov	r8, qword [rbp-8]
	mov	r9, 2
	cmp	r8, r9
	sete	r8b
	test	r8b, r8b
	jz	.L3
	mov	r8, LS2
	mov	rax, r8
	jmp	.L0
.L3:
	mov	r8, LS3
	mov	rax, r8
	jmp	.L0
	xor	rax, rax
.L0:
	add	rsp, 16
	pop	rbp
	ret

	global	main
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 0
	push	rcx
	push	rdx
	push	rsi
	push	rdi
	push	r8
	push	r9
	mov	r10, 0
	mov	rdi, r10
	lea	r10, [rel fruit_string]
	call	r10
	pop	r9
	pop	r8
	pop	rdi
	pop	rsi
	pop	rdx
	pop	rcx
	mov	r10, rax
	mov	rdi, r10
	lea	r10, [rel puts]
	call	r10
	push	rcx
	push	rdx
	push	rsi
	push	rdi
	push	r8
	push	r9
	mov	r10, 1
	mov	rdi, r10
	lea	r10, [rel fruit_string]
	call	r10
	pop	r9
	pop	r8
	pop	rdi
	pop	rsi
	pop	rdx
	pop	rcx
	mov	r10, rax
	mov	rdi, r10
	lea	r10, [rel puts]
	call	r10
	push	rcx
	push	rdx
	push	rsi
	push	rdi
	push	r8
	push	r9
	mov	r10, 2
	mov	rdi, r10
	lea	r10, [rel fruit_string]
	call	r10
	pop	r9
	pop	r8
	pop	rdi
	pop	rsi
	pop	rdx
	pop	rcx
	mov	r10, rax
	mov	rdi, r10
	lea	r10, [rel puts]
	call	r10
	push	rcx
	push	rdx
	push	rsi
	push	rdi
	push	r8
	push	r9
	mov	r10, 1234
	mov	rdi, r10
	lea	r10, [rel fruit_string]
	call	r10
	pop	r9
	pop	r8
	pop	rdi
	pop	rsi
	pop	rdx
	pop	rcx
	mov	r10, rax
	mov	rdi, r10
	lea	r10, [rel puts]
	call	r10
.L4:
	xor	rax, rax
	add	rsp, 0
	pop	rbp
	ret

section .rodata
	printi_s: db "%ld", 10, 0
	LS0: db 65, 112, 112, 108, 101, 0
	LS1: db 80, 101, 97, 114, 0
	LS2: db 79, 114, 97, 110, 103, 101, 0
	LS3: db 73, 32, 100, 111, 110, 39, 116, 32, 107, 110, 111, 119, 32, 116, 104, 97, 116, 32, 102, 114, 117, 105, 116, 33, 0

section .bss

section .note.GNU-stack

