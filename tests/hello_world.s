section .text
	extern	printf

printi:
	mov	rsi, rdi
	xor	eax, eax
	mov	edi, printi_s
	jmp	printf

	extern	puts
	global	main
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	lea	r10, [rbp-14]
	mov	r11, 0
	imul	r11, 1
	add	r10, r11
	mov	r11, 72
	mov	byte [r10], r11b
	lea	r10, [rbp-14]
	mov	r11, 1
	imul	r11, 1
	add	r10, r11
	mov	r11, 101
	mov	byte [r10], r11b
	lea	r10, [rbp-14]
	mov	r11, 2
	imul	r11, 1
	add	r10, r11
	mov	r11, 108
	mov	byte [r10], r11b
	lea	r10, [rbp-14]
	mov	r11, 3
	imul	r11, 1
	add	r10, r11
	mov	r11, 108
	mov	byte [r10], r11b
	lea	r10, [rbp-14]
	mov	r11, 4
	imul	r11, 1
	add	r10, r11
	mov	r11, 111
	mov	byte [r10], r11b
	lea	r10, [rbp-14]
	mov	r11, 5
	imul	r11, 1
	add	r10, r11
	mov	r11, 44
	mov	byte [r10], r11b
	lea	r10, [rbp-14]
	mov	r11, 6
	imul	r11, 1
	add	r10, r11
	mov	r11, 32
	mov	byte [r10], r11b
	lea	r10, [rbp-14]
	mov	r11, 7
	imul	r11, 1
	add	r10, r11
	mov	r11, 119
	mov	byte [r10], r11b
	lea	r10, [rbp-14]
	mov	r11, 8
	imul	r11, 1
	add	r10, r11
	mov	r11, 111
	mov	byte [r10], r11b
	lea	r10, [rbp-14]
	mov	r11, 9
	imul	r11, 1
	add	r10, r11
	mov	r11, 114
	mov	byte [r10], r11b
	lea	r10, [rbp-14]
	mov	r11, 10
	imul	r11, 1
	add	r10, r11
	mov	r11, 108
	mov	byte [r10], r11b
	lea	r10, [rbp-14]
	mov	r11, 11
	imul	r11, 1
	add	r10, r11
	mov	r11, 100
	mov	byte [r10], r11b
	lea	r10, [rbp-14]
	mov	r11, 12
	imul	r11, 1
	add	r10, r11
	mov	r11, 33
	mov	byte [r10], r11b
	lea	r10, [rbp-14]
	mov	r11, 13
	imul	r11, 1
	add	r10, r11
	mov	r11, 0
	mov	byte [r10], r11b
	lea	r10, [rbp-14]
	mov	rdi, r10
	lea	r10, [rel puts]
	call	r10
	mov	r10, 0
	mov	eax, r10d
	jmp	.L0
.L0:
	add	rsp, 16
	pop	rbp
	ret

section .data
	printi_s: db "%ld", 10, 0

section .note.GNU-stack

