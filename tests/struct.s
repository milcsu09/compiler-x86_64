section .text
	extern	printf

printi:
	mov	rsi, rdi
	xor	eax, eax
	mov	edi, printi_s
	jmp	printf

	global	main
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 32
	lea	r10, [rbp-24]
	add	r10, 0
	mov	r11, 10
	mov	byte [r10], r11b
	lea	r10, [rbp-24]
	add	r10, 2
	mov	r11, 20
	mov	word [r10], r11w
	lea	r10, [rbp-24]
	add	r10, 4
	mov	r11, 30
	mov	dword [r10], r11d
	lea	r10, [rbp-24]
	add	r10, 8
	mov	r11, 40
	mov	qword [r10], r11
	lea	r10, [rbp-24]
	add	r10, 16
	mov	r11, 0
	imul	r11, 1
	add	r10, r11
	mov	r11, 1
	mov	byte [r10], r11b
	lea	r10, [rbp-24]
	add	r10, 16
	mov	r11, 1
	imul	r11, 1
	add	r10, r11
	mov	r11, 2
	mov	byte [r10], r11b
	lea	r10, [rbp-24]
	add	r10, 16
	mov	r11, 2
	imul	r11, 1
	add	r10, r11
	mov	r11, 3
	mov	byte [r10], r11b
	lea	r10, [rbp-24]
	add	r10, 16
	mov	r11, 3
	imul	r11, 1
	add	r10, r11
	mov	r11, 4
	mov	byte [r10], r11b
	lea	r10, [rbp-24]
	add	r10, 0
	mov	r10b, byte [r10]
	movsx	r10, r10b
	mov	rdi, r10
	call	printi
	lea	r10, [rbp-24]
	add	r10, 2
	mov	r10w, word [r10]
	movsx	r10, r10w
	mov	rdi, r10
	call	printi
	lea	r10, [rbp-24]
	add	r10, 4
	mov	r10d, dword [r10]
	movsx	r10, r10d
	mov	rdi, r10
	call	printi
	lea	r10, [rbp-24]
	add	r10, 8
	mov	r10, qword [r10]
	mov	rdi, r10
	call	printi
	lea	r10, [rbp-24]
	add	r10, 16
	mov	r11, 0
	imul	r11, 1
	add	r10, r11
	mov	r10b, byte [r10]
	movzx	r10, r10b
	mov	rdi, r10
	call	printi
	lea	r10, [rbp-24]
	add	r10, 16
	mov	r11, 1
	imul	r11, 1
	add	r10, r11
	mov	r10b, byte [r10]
	movzx	r10, r10b
	mov	rdi, r10
	call	printi
	lea	r10, [rbp-24]
	add	r10, 16
	mov	r11, 2
	imul	r11, 1
	add	r10, r11
	mov	r10b, byte [r10]
	movzx	r10, r10b
	mov	rdi, r10
	call	printi
	lea	r10, [rbp-24]
	add	r10, 16
	mov	r11, 3
	imul	r11, 1
	add	r10, r11
	mov	r10b, byte [r10]
	movzx	r10, r10b
	mov	rdi, r10
	call	printi
	mov	r10, 0
	mov	eax, r10d
	jmp	.L0
.L0:
	add	rsp, 32
	pop	rbp
	ret

section .data
	printi_s: db "%ld", 10, 0

section .note.GNU-stack

