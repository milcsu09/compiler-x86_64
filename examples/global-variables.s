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
	mov	r10, 10
	lea	r11, [rel a]
	mov	dword [r11], r10d
	mov	r10, 20
	lea	r11, [rel b]
	mov	dword [r11], r10d
	mov	r10, 30
	lea	r11, [rel c]
	mov	dword [r11], r10d
	mov	r10d, dword [rel a]
	movsx	r10, r10d
	mov	rdi, r10
	call	printi
	mov	r10d, dword [rel b]
	movsx	r10, r10d
	mov	rdi, r10
	call	printi
	mov	r10d, dword [rel c]
	movsx	r10, r10d
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
	c: resb 4
	b: resb 4
	a: resb 4

section .note.GNU-stack

