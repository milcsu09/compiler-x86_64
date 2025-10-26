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
	lea	r10, [rbp-32]
	mov	r11, 0
	imul	r11, 8
	add	r10, r11
	add	r10, 0
	mov	r11, 10
	mov	dword [r10], r11d
	lea	r10, [rbp-32]
	mov	r11, 0
	imul	r11, 8
	add	r10, r11
	add	r10, 4
	mov	r11, 20
	mov	dword [r10], r11d
	lea	r10, [rbp-32]
	mov	r11, 1
	imul	r11, 8
	add	r10, r11
	add	r10, 0
	mov	r11, 30
	mov	dword [r10], r11d
	lea	r10, [rbp-32]
	mov	r11, 1
	imul	r11, 8
	add	r10, r11
	add	r10, 4
	mov	r11, 40
	mov	dword [r10], r11d
	lea	r10, [rbp-32]
	mov	r11, 2
	imul	r11, 8
	add	r10, r11
	add	r10, 0
	mov	r11, 50
	mov	dword [r10], r11d
	lea	r10, [rbp-32]
	mov	r11, 2
	imul	r11, 8
	add	r10, r11
	add	r10, 4
	mov	r11, 60
	mov	dword [r10], r11d
	lea	r10, [rbp-32]
	mov	r11, 3
	imul	r11, 8
	add	r10, r11
	add	r10, 0
	mov	r11, 70
	mov	dword [r10], r11d
	lea	r10, [rbp-32]
	mov	r11, 3
	imul	r11, 8
	add	r10, r11
	add	r10, 4
	mov	r11, 80
	mov	dword [r10], r11d
	lea	r10, [rbp-32]
	mov	r11, 0
	imul	r11, 8
	add	r10, r11
	add	r10, 0
	mov	r10d, dword [r10]
	movsx	r10, r10d
	mov	rdi, r10
	call	printi
	lea	r10, [rbp-32]
	mov	r11, 0
	imul	r11, 8
	add	r10, r11
	add	r10, 4
	mov	r10d, dword [r10]
	movsx	r10, r10d
	mov	rdi, r10
	call	printi
	lea	r10, [rbp-32]
	mov	r11, 1
	imul	r11, 8
	add	r10, r11
	add	r10, 0
	mov	r10d, dword [r10]
	movsx	r10, r10d
	mov	rdi, r10
	call	printi
	lea	r10, [rbp-32]
	mov	r11, 1
	imul	r11, 8
	add	r10, r11
	add	r10, 4
	mov	r10d, dword [r10]
	movsx	r10, r10d
	mov	rdi, r10
	call	printi
	lea	r10, [rbp-32]
	mov	r11, 2
	imul	r11, 8
	add	r10, r11
	add	r10, 0
	mov	r10d, dword [r10]
	movsx	r10, r10d
	mov	rdi, r10
	call	printi
	lea	r10, [rbp-32]
	mov	r11, 2
	imul	r11, 8
	add	r10, r11
	add	r10, 4
	mov	r10d, dword [r10]
	movsx	r10, r10d
	mov	rdi, r10
	call	printi
	lea	r10, [rbp-32]
	mov	r11, 3
	imul	r11, 8
	add	r10, r11
	add	r10, 0
	mov	r10d, dword [r10]
	movsx	r10, r10d
	mov	rdi, r10
	call	printi
	lea	r10, [rbp-32]
	mov	r11, 3
	imul	r11, 8
	add	r10, r11
	add	r10, 4
	mov	r10d, dword [r10]
	movsx	r10, r10d
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

