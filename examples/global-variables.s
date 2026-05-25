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
	mov	r8, 10
	lea	r9, [rel a]
	mov	dword [r9], r8d
	mov	r8, 20
	lea	r9, [rel b]
	mov	dword [r9], r8d
	mov	r8, 30
	lea	r9, [rel c]
	mov	dword [r9], r8d
	mov	r8d, dword [rel a]
	movsx	r8, r8d
	mov	rdi, r8
	call	printi
	mov	r8d, dword [rel b]
	movsx	r8, r8d
	mov	rdi, r8
	call	printi
	mov	r8d, dword [rel c]
	movsx	r8, r8d
	mov	rdi, r8
	call	printi
.L0:
	xor	rax, rax
	add	rsp, 0
	pop	rbp
	ret

section .rodata
	printi_s: db "%ld", 10, 0

section .bss
	c: resb 4
	b: resb 4
	a: resb 4

section .note.GNU-stack

