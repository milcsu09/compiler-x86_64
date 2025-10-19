section .text
	extern	printf

printi:
	mov	rsi, rdi
	xor	eax, eax
	mov	edi, printi_s
	jmp	printf

	global takemany
takemany:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 48
	;
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
	;
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
	;
	add	rsp, 48
	pop	rbp
	xor	rax, rax
	ret

	global main
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	;
	mov	dword [rbp-4], edi
	mov	qword [rbp-16], rsi
	;
	mov	r10d, dword [rbp-4]
	movsx	r10, r10d
	mov	rdi, r10
	call	printi
	mov	r10, qword [rbp-16]
	mov	rdi, r10
	call	printi
	;
	add	rsp, 16
	pop	rbp
	xor	rax, rax
	ret

section .data
	printi_s: db "%ld", 10, 0

section .note.GNU-stack

