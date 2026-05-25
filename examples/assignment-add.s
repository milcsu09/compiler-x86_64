section .text
	extern	printf

printi:
	mov	rsi, rdi
	xor	eax, eax
	mov	rdi, printi_s
	jmp	printf

	extern	puts
	global	f
f:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 0
	mov	r10, LS0
	mov	rdi, r10
	lea	r10, [rel puts]
	call	r10
	lea	r8, [rel a]
	mov	rax, r8
	jmp	.L0
	xor	rax, rax
.L0:
	add	rsp, 0
	pop	rbp
	ret

	global	main
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 0
	mov	r8, 5
	lea	r9, [rel a]
	mov	qword [r9], r8
	lea	r10, [rel f]
	call	r10
	mov	r8, rax
	mov	r9, qword [r8]
	mov	r10, 15
	add	r9, r10
	mov	qword [r8], r9
	mov	r8, qword [rel a]
	mov	rdi, r8
	call	printi
.L1:
	xor	rax, rax
	add	rsp, 0
	pop	rbp
	ret

section .rodata
	printi_s: db "%ld", 10, 0
	LS0: db 67, 97, 108, 108, 101, 100, 32, 39, 102, 40, 41, 39, 0

section .bss
	a: resb 8

section .note.GNU-stack

