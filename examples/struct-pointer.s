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
	lea	r10, [rbp-16]
	lea	r11, [rbp-24]
	mov	qword [r11], r10
	mov	r10, LS0
	mov	r11, qword [rbp-24]
	add	r11, 0
	mov	qword [r11], r10
	mov	r10, 42
	mov	r11, qword [rbp-24]
	add	r11, 8
	mov	qword [r11], r10
	lea	r10, [rbp-16]
	add	r10, 0
	mov	r10, qword [r10]
	mov	rdi, r10
	call	printi
	lea	r10, [rbp-16]
	add	r10, 8
	mov	r10, qword [r10]
	mov	rdi, r10
	call	printi
.L0:
	xor	rax, rax
	add	rsp, 32
	pop	rbp
	ret

section .rodata
	printi_s: db "%ld", 10, 0
	LS0: db 74, 111, 104, 110, 32, 68, 111, 101, 0

section .bss

section .note.GNU-stack

