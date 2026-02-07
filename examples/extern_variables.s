section .text
	extern	printf

printi:
	mov	rsi, rdi
	xor	eax, eax
	mov	rdi, printi_s
	jmp	printf

	extern	environ
	extern	stdin
	extern	stdout
	extern	stderr
	extern	puts
	extern	fputs
	global	main
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	mov	r10, 0
	lea	r11, [rbp-8]
	mov	qword [r11], r10
	jmp	.L2
.L1:
	mov	r10, qword [rel environ]
	mov	r11, qword [rbp-8]
	imul	r11, 8
	add	r10, r11
	mov	r10, qword [r10]
	mov	rdi, r10
	lea	r10, [rel puts]
	call	r10
.L4:
	mov	r10, qword [rbp-8]
	mov	r11, 1
	add	r10, r11
	lea	r11, [rbp-8]
	mov	qword [r11], r10
.L2:
	mov	r10, qword [rel environ]
	mov	r11, qword [rbp-8]
	imul	r11, 8
	add	r10, r11
	mov	r10, qword [r10]
	test	r10, r10
	jnz	.L1
.L3:
	mov	r10, qword [rel stdin]
	mov	rdi, r10
	call	printi
	mov	r10, qword [rel stdout]
	mov	rdi, r10
	call	printi
	mov	r10, qword [rel stderr]
	mov	rdi, r10
	call	printi
	mov	r10, LS0
	mov	rdi, r10
	mov	r10, qword [rel stderr]
	mov	rsi, r10
	lea	r10, [rel fputs]
	call	r10
.L0:
	xor	rax, rax
	add	rsp, 16
	pop	rbp
	ret

section .rodata
	printi_s: db "%ld", 10, 0
	LS0: db 72, 101, 108, 108, 111, 33, 10, 0

section .bss

section .note.GNU-stack

