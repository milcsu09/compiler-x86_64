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
	sub	rsp, 0
	mov	r10, 16
	mov	rdi, r10
	call	printi
.L0:
	xor	rax, rax
	add	rsp, 0
	pop	rbp
	ret

section .rodata
	printi_s: db "%ld", 10, 0

section .bss

section .note.GNU-stack

