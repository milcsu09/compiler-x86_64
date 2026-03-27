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
	mov	r10, 10
	lea	r11, [rbp-8]
	mov	qword [r11], r10
	mov	r10, 15
	lea	r11, [rbp-8]
	mov	qword [r11], r10
	mov	r10, qword [rbp-8]
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

