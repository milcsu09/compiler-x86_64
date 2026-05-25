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
	sub	rsp, 16
	mov	r8, 32767
	lea	r9, [rbp-2]
	mov	word [r9], r8w
	lea	r8, [rbp-2]
	mov	r8w, word [r8]
	movzx	r8, r8w
	mov	rdi, r8
	call	printi
	lea	r8, [rbp-2]
	mov	r8b, byte [r8]
	movzx	r8, r8b
	mov	rdi, r8
	call	printi
	lea	r8, [rbp-2]
	mov	rdi, r8
	call	printi
	lea	r8, [rbp-2]
	mov	rdi, r8
	call	printi
.L0:
	xor	rax, rax
	add	rsp, 16
	pop	rbp
	ret

section .rodata
	printi_s: db "%ld", 10, 0

section .bss

section .note.GNU-stack

