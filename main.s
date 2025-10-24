section .text
	extern	printf

printi:
	mov	rsi, rdi
	xor	eax, eax
	mov	edi, printi_s
	jmp	printf

	global take_many
take_many:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 80
	mov	byte [rbp-1], dil
	mov	word [rbp-4], si
	mov	dword [rbp-8], edx
	mov	qword [rbp-16], rcx
	mov	dword [rbp-20], r8d
	mov	word [rbp-22], r9w
	mov	r10b, byte [rbp+16]
	mov	byte [rbp-23], r10b
	mov	r10w, word [rbp+24]
	mov	word [rbp-26], r10w
	mov	r10d, dword [rbp+32]
	mov	dword [rbp-32], r10d
	mov	r10, qword [rbp+40]
	mov	qword [rbp-40], r10
	mov	r10d, dword [rbp+48]
	mov	dword [rbp-44], r10d
	mov	r10w, word [rbp+56]
	mov	word [rbp-46], r10w
	mov	r10b, byte [rbp+64]
	mov	byte [rbp-47], r10b
	mov	r10w, word [rbp+72]
	mov	word [rbp-50], r10w
	mov	r10d, dword [rbp+80]
	mov	dword [rbp-56], r10d
	mov	r10, qword [rbp+88]
	mov	qword [rbp-64], r10
	mov	r10d, dword [rbp+96]
	mov	dword [rbp-68], r10d
	mov	r10w, word [rbp+104]
	mov	word [rbp-70], r10w
	mov	r10b, byte [rbp-1]
	movsx	r10, r10b
	mov	rdi, r10
	call	printi
	mov	r10w, word [rbp-4]
	movsx	r10, r10w
	mov	rdi, r10
	call	printi
	mov	r10d, dword [rbp-8]
	movsx	r10, r10d
	mov	rdi, r10
	call	printi
	mov	r10, qword [rbp-16]
	mov	rdi, r10
	call	printi
	mov	r10d, dword [rbp-20]
	movsx	r10, r10d
	mov	rdi, r10
	call	printi
	mov	r10w, word [rbp-22]
	movsx	r10, r10w
	mov	rdi, r10
	call	printi
	mov	r10b, byte [rbp-23]
	movsx	r10, r10b
	mov	rdi, r10
	call	printi
	mov	r10w, word [rbp-26]
	movsx	r10, r10w
	mov	rdi, r10
	call	printi
	mov	r10d, dword [rbp-32]
	movsx	r10, r10d
	mov	rdi, r10
	call	printi
	mov	r10, qword [rbp-40]
	mov	rdi, r10
	call	printi
	mov	r10d, dword [rbp-44]
	movsx	r10, r10d
	mov	rdi, r10
	call	printi
	mov	r10w, word [rbp-46]
	movsx	r10, r10w
	mov	rdi, r10
	call	printi
	mov	r10b, byte [rbp-47]
	movsx	r10, r10b
	mov	rdi, r10
	call	printi
	mov	r10w, word [rbp-50]
	movsx	r10, r10w
	mov	rdi, r10
	call	printi
	mov	r10d, dword [rbp-56]
	movsx	r10, r10d
	mov	rdi, r10
	call	printi
	mov	r10, qword [rbp-64]
	mov	rdi, r10
	call	printi
	mov	r10d, dword [rbp-68]
	movsx	r10, r10d
	mov	rdi, r10
	call	printi
	mov	r10w, word [rbp-70]
	movsx	r10, r10w
	mov	rdi, r10
	call	printi
	mov	r10w, word [rbp-70]
	movsx	r10d, r10w
	mov	eax, r10d
	jmp	.L0
.L0:
	add	rsp, 80
	pop	rbp
	ret

	global fib
fib:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	mov	qword [rbp-8], rdi
	mov	r10, qword [rbp-8]
	mov	r11, 1
	cmp	r10, r11
	setle	r10b
	test	r10b, r10b
	jz	.L2
	mov	r10, qword [rbp-8]
	mov	rax, r10
	jmp	.L1
.L2:
	mov	r10, qword [rbp-8]
	mov	r11, 1
	sub	r10, r11
	mov	rdi, r10
	lea	r10, [rel fib]
	call	r10
	mov	r10, rax
	push	r10
	mov	r11, qword [rbp-8]
	mov	r12, 2
	sub	r11, r12
	mov	rdi, r11
	sub	rsp, 8
	lea	r11, [rel fib]
	call	r11
	add	rsp, 8
	pop	r10
	mov	r11, rax
	add	r10, r11
	mov	rax, r10
	jmp	.L1
.L1:
	add	rsp, 16
	pop	rbp
	ret

	global fib2
fib2:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 32
	mov	qword [rbp-8], rdi
	lea	r10, [rbp-16]
	mov	r11, 0
	mov	qword [r10], r11
	lea	r10, [rbp-24]
	mov	r11, 1
	mov	qword [r10], r11
	lea	r10, [rbp-32]
	mov	r11, 0
	mov	qword [r10], r11
	jmp	.L5
.L4:
	lea	r10, [rbp-32]
	mov	r11, qword [rbp-16]
	mov	r12, qword [rbp-24]
	add	r11, r12
	mov	qword [r10], r11
	lea	r10, [rbp-16]
	mov	r11, qword [rbp-24]
	mov	qword [r10], r11
	lea	r10, [rbp-24]
	mov	r11, qword [rbp-32]
	mov	qword [r10], r11
	lea	r10, [rbp-8]
	mov	r11, qword [rbp-8]
	mov	r12, 1
	sub	r11, r12
	mov	qword [r10], r11
.L5:
	mov	r10, qword [rbp-8]
	mov	r11, 0
	cmp	r10, r11
	setg	r10b
	test	r10b, r10b
	jnz	.L4
	mov	r10, qword [rbp-16]
	mov	rax, r10
	jmp	.L3
.L3:
	add	rsp, 32
	pop	rbp
	ret

	global main
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	lea	r10, [rbp-8]
	mov	r11, 0
	mov	qword [r10], r11
	jmp	.L8
.L7:
	lea	r10, [rbp-16]
	mov	r11, 0
	mov	qword [r10], r11
	jmp	.L10
.L9:
	mov	r10, qword [rbp-8]
	mov	r11, qword [rbp-16]
	imul	r10, r11
	mov	rdi, r10
	call	printi
	lea	r10, [rbp-16]
	mov	r11, qword [rbp-16]
	mov	r12, 1
	add	r11, r12
	mov	qword [r10], r11
.L10:
	mov	r10, qword [rbp-16]
	mov	r11, 10
	cmp	r10, r11
	setl	r10b
	test	r10b, r10b
	jnz	.L9
	lea	r10, [rbp-8]
	mov	r11, qword [rbp-8]
	mov	r12, 1
	add	r11, r12
	mov	qword [r10], r11
.L8:
	mov	r10, qword [rbp-8]
	mov	r11, 10
	cmp	r10, r11
	setl	r10b
	test	r10b, r10b
	jnz	.L7
	mov	r10, 40
	mov	rdi, r10
	lea	r10, [rel fib]
	call	r10
	mov	r10, rax
	mov	rdi, r10
	call	printi
	mov	r10, 40
	mov	rdi, r10
	lea	r10, [rel fib2]
	call	r10
	mov	r10, rax
	mov	rdi, r10
	call	printi
	mov	r10, 20
	mov	r11, 30
	mov	r12, 40
	mov	r13, 50
	push	r10
	push	r11
	push	r12
	push	r13
	mov	r14, 1
	mov	r15, 2
	push	r10
	mov	r10, 3
	push	r11
	mov	r11, 4
	push	r12
	mov	r12, 5
	push	r13
	mov	r13, 6
	add	r12, r13
	pop	r13
	add	r11, r12
	pop	r12
	add	r10, r11
	pop	r11
	add	r15, r10
	pop	r10
	add	r14, r15
	mov	dil, r14b
	mov	r14, 2
	mov	si, r14w
	mov	r14, 3
	mov	edx, r14d
	mov	r14, 4
	mov	rcx, r14
	mov	r14, 5
	mov	r8d, r14d
	mov	r14, 6
	mov	r9w, r14w
	sub	rsp, 96
	mov	r14, 7
	mov	qword [rsp+0], r14
	mov	r14, 8
	mov	qword [rsp+8], r14
	mov	r14, 9
	mov	qword [rsp+16], r14
	mov	r14, 10
	mov	qword [rsp+24], r14
	mov	r14, 11
	mov	qword [rsp+32], r14
	mov	r14, 12
	mov	qword [rsp+40], r14
	mov	r14, 13
	mov	qword [rsp+48], r14
	mov	r14, 14
	mov	qword [rsp+56], r14
	mov	r14, 15
	mov	qword [rsp+64], r14
	mov	r14, 16
	mov	qword [rsp+72], r14
	mov	r14, 17
	mov	qword [rsp+80], r14
	mov	r14, 18
	mov	qword [rsp+88], r14
	lea	r14, [rel take_many]
	call	r14
	add	rsp, 96
	pop	r13
	pop	r12
	pop	r11
	pop	r10
	mov	r14d, eax
	movsx	r14, r14d
	add	r13, r14
	add	r12, r13
	add	r11, r12
	add	r10, r11
	mov	rdi, r10
	call	printi
	mov	r10, 0
	mov	eax, r10d
	jmp	.L6
.L6:
	add	rsp, 16
	pop	rbp
	ret

section .data
	printi_s: db "%ld", 10, 0

section .note.GNU-stack

