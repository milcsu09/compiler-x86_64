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
	mov	r10, qword [rbp-8]
	mov	r11, 0
	cmp	r10, r11
	sete	r10b
	test	r10b, r10b
	jz	.L1
	mov	r10, LS0
	mov	rax, r10
	jmp	.L0
.L1:
	mov	r10, qword [rbp-8]
	mov	r11, 1
	cmp	r10, r11
	sete	r10b
	test	r10b, r10b
	jz	.L2
	mov	r10, LS1
	mov	rax, r10
	jmp	.L0
.L2:
	mov	r10, qword [rbp-8]
	mov	r11, 2
	cmp	r10, r11
	sete	r10b
	test	r10b, r10b
	jz	.L3
	mov	r10, LS2
	mov	rax, r10
	jmp	.L0
.L3:
	mov	r10, LS3
	mov	rax, r10
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
	mov	r10, 0
	mov	rdi, r10
	lea	r10, [rel fruit_string]
	call	r10
	mov	r10, rax
	mov	rdi, r10
	lea	r10, [rel puts]
	call	r10
	mov	r10, 1
	mov	rdi, r10
	lea	r10, [rel fruit_string]
	call	r10
	mov	r10, rax
	mov	rdi, r10
	lea	r10, [rel puts]
	call	r10
	mov	r10, 2
	mov	rdi, r10
	lea	r10, [rel fruit_string]
	call	r10
	mov	r10, rax
	mov	rdi, r10
	lea	r10, [rel puts]
	call	r10
	mov	r10, 1234
	mov	rdi, r10
	lea	r10, [rel fruit_string]
	call	r10
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

