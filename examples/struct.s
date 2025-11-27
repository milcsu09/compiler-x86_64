section .text
	extern	printf

printi:
	mov	rsi, rdi
	xor	eax, eax
	mov	rdi, printi_s
	jmp	printf

	global	main
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 32
	mov	r10, 10
	lea	r11, [rbp-24]
	add	r11, 0
	mov	byte [r11], r10b
	mov	r10, 20
	lea	r11, [rbp-24]
	add	r11, 2
	mov	word [r11], r10w
	mov	r10, 30
	lea	r11, [rbp-24]
	add	r11, 4
	mov	dword [r11], r10d
	mov	r10, 40
	lea	r11, [rbp-24]
	add	r11, 8
	mov	qword [r11], r10
	mov	r10, 1
	lea	r11, [rbp-24]
	add	r11, 16
	mov	r12, 0
	add	r11, r12
	mov	byte [r11], r10b
	mov	r10, 2
	lea	r11, [rbp-24]
	add	r11, 16
	mov	r12, 1
	add	r11, r12
	mov	byte [r11], r10b
	mov	r10, 3
	lea	r11, [rbp-24]
	add	r11, 16
	mov	r12, 2
	add	r11, r12
	mov	byte [r11], r10b
	mov	r10, 4
	lea	r11, [rbp-24]
	add	r11, 16
	mov	r12, 3
	add	r11, r12
	mov	byte [r11], r10b
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
	add	r10, r11
	mov	r10b, byte [r10]
	movzx	r10, r10b
	mov	rdi, r10
	call	printi
	lea	r10, [rbp-24]
	add	r10, 16
	mov	r11, 1
	add	r10, r11
	mov	r10b, byte [r10]
	movzx	r10, r10b
	mov	rdi, r10
	call	printi
	lea	r10, [rbp-24]
	add	r10, 16
	mov	r11, 2
	add	r10, r11
	mov	r10b, byte [r10]
	movzx	r10, r10b
	mov	rdi, r10
	call	printi
	lea	r10, [rbp-24]
	add	r10, 16
	mov	r11, 3
	add	r10, r11
	mov	r10b, byte [r10]
	movzx	r10, r10b
	mov	rdi, r10
	call	printi
	mov	r10, 0
	mov	eax, r10d
	jmp	.L0
	xor	rax, rax
.L0:
	add	rsp, 32
	pop	rbp
	ret

section .rodata
	printi_s: db "%ld", 10, 0

section .note.GNU-stack

