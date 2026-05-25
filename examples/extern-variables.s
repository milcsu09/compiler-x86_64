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
	mov	r8, 0
	lea	r9, [rbp-8]
	mov	qword [r9], r8
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
	lea	r8, [rbp-8]
	mov	r9, qword [r8]
	mov	r10, 1
	add	r9, r10
	mov	qword [r8], r9
.L2:
	mov	r8, qword [rel environ]
	mov	r9, qword [rbp-8]
	imul	r9, 8
	add	r8, r9
	mov	r8, qword [r8]
	test	r8, r8
	jnz	.L1
.L3:
	mov	r8, qword [rel stdin]
	mov	rdi, r8
	call	printi
	mov	r8, qword [rel stdout]
	mov	rdi, r8
	call	printi
	mov	r8, qword [rel stderr]
	mov	rdi, r8
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

