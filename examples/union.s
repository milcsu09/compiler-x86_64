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
	mov	r10, 32767
	lea	r11, [rbp-2]
	mov	word [r11], r10w
	lea	r10, [rbp-2]
	mov	r10w, word [r10]
	movzx	r10, r10w
	mov	rdi, r10
	call	printi
	lea	r10, [rbp-2]
	mov	r10b, byte [r10]
	movzx	r10, r10b
	mov	rdi, r10
	call	printi
	lea	r10, [rbp-2]
	mov	rdi, r10
	call	printi
	lea	r10, [rbp-2]
	mov	rdi, r10
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

