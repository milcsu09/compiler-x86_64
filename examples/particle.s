section .text
	extern	printf

printi:
	mov	rsi, rdi
	xor	eax, eax
	mov	edi, printi_s
	jmp	printf

	extern	malloc
	extern	free
	extern	InitWindow
	extern	CloseWindow
	extern	WindowShouldClose
	extern	SetTargetFPS
	extern	BeginDrawing
	extern	EndDrawing
	extern	ClearBackground
	extern	DrawRectangle
	extern	DrawFPS
	extern	GetRandomValue
	global	particle_init
particle_init:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	mov	qword [rbp-8], rdi
	mov	r10, qword [rbp-8]
	add	r10, 0
	mov	r11, 400
	mov	r12, 256
	imul	r11, r12
	mov	dword [r10], r11d
	mov	r10, qword [rbp-8]
	add	r10, 4
	mov	r11, 300
	mov	r12, 256
	imul	r11, r12
	mov	dword [r10], r11d
	mov	r10, qword [rbp-8]
	add	r10, 8
	push	r10
	mov	r11, 0
	mov	edi, r11d
	mov	r11, 512
	mov	esi, r11d
	sub	rsp, 8
	lea	r11, [rel GetRandomValue]
	call	r11
	add	rsp, 8
	pop	r10
	mov	r11d, eax
	movsx	r11, r11d
	mov	r12, 256
	sub	r11, r12
	mov	r12, 4
	imul	r11, r12
	mov	dword [r10], r11d
	mov	r10, qword [rbp-8]
	add	r10, 12
	push	r10
	mov	r11, 0
	mov	edi, r11d
	mov	r11, 512
	mov	esi, r11d
	sub	rsp, 8
	lea	r11, [rel GetRandomValue]
	call	r11
	add	rsp, 8
	pop	r10
	mov	r11d, eax
	movsx	r11, r11d
	mov	r12, 256
	sub	r11, r12
	mov	r12, 4
	imul	r11, r12
	mov	dword [r10], r11d
	mov	r10, qword [rbp-8]
	add	r10, 16
	push	r10
	mov	r11, 200
	mov	edi, r11d
	mov	r11, 500
	mov	esi, r11d
	sub	rsp, 8
	lea	r11, [rel GetRandomValue]
	call	r11
	add	rsp, 8
	pop	r10
	mov	r11d, eax
	mov	dword [r10], r11d
.L0:
	add	rsp, 16
	pop	rbp
	ret

	global	main
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 48
	lea	r10, [rbp-1]
	mov	r11, 0
	add	r10, r11
	mov	r11, 0
	mov	byte [r10], r11b
	mov	r10, 800
	mov	edi, r10d
	mov	r10, 600
	mov	esi, r10d
	lea	r10, [rbp-1]
	mov	rdx, r10
	lea	r10, [rel InitWindow]
	call	r10
	mov	r10, 60
	mov	edi, r10d
	lea	r10, [rel SetTargetFPS]
	call	r10
	lea	r10, [rbp-16]
	push	r10
	mov	r11, 20
	mov	r12, 10000
	imul	r11, r12
	mov	rdi, r11
	sub	rsp, 8
	lea	r11, [rel malloc]
	call	r11
	add	rsp, 8
	pop	r10
	mov	r11, rax
	mov	qword [r10], r11
	lea	r10, [rbp-24]
	mov	r11, 0
	mov	qword [r10], r11
	jmp	.L3
.L2:
	mov	r10, qword [rbp-16]
	mov	r11, qword [rbp-24]
	imul	r11, 20
	add	r10, r11
	mov	rdi, r10
	lea	r10, [rel particle_init]
	call	r10
	lea	r10, [rbp-24]
	mov	r11, qword [rbp-24]
	mov	r12, 1
	add	r11, r12
	mov	qword [r10], r11
.L3:
	mov	r10, qword [rbp-24]
	mov	r11, 10000
	cmp	r10, r11
	setl	r10b
	test	r10b, r10b
	jnz	.L2
	jmp	.L5
.L4:
	lea	r10, [rbp-24]
	mov	r11, 0
	mov	qword [r10], r11
	jmp	.L7
.L6:
	mov	r10, qword [rbp-16]
	mov	r11, qword [rbp-24]
	imul	r11, 20
	add	r10, r11
	add	r10, 0
	mov	r11, qword [rbp-16]
	mov	r12, qword [rbp-24]
	imul	r12, 20
	add	r11, r12
	add	r11, 0
	mov	r11d, dword [r11]
	mov	r12, qword [rbp-16]
	push	r10
	mov	r10, qword [rbp-24]
	imul	r10, 20
	add	r12, r10
	pop	r10
	add	r12, 8
	mov	r12d, dword [r12]
	add	r11d, r12d
	mov	dword [r10], r11d
	mov	r10, qword [rbp-16]
	mov	r11, qword [rbp-24]
	imul	r11, 20
	add	r10, r11
	add	r10, 4
	mov	r11, qword [rbp-16]
	mov	r12, qword [rbp-24]
	imul	r12, 20
	add	r11, r12
	add	r11, 4
	mov	r11d, dword [r11]
	mov	r12, qword [rbp-16]
	push	r10
	mov	r10, qword [rbp-24]
	imul	r10, 20
	add	r12, r10
	pop	r10
	add	r12, 12
	mov	r12d, dword [r12]
	add	r11d, r12d
	mov	dword [r10], r11d
	mov	r10, qword [rbp-16]
	mov	r11, qword [rbp-24]
	imul	r11, 20
	add	r10, r11
	add	r10, 16
	mov	r11, qword [rbp-16]
	mov	r12, qword [rbp-24]
	imul	r12, 20
	add	r11, r12
	add	r11, 16
	mov	r11d, dword [r11]
	movsx	r11, r11d
	mov	r12, 5
	sub	r11, r12
	mov	dword [r10], r11d
	mov	r10, qword [rbp-16]
	mov	r11, qword [rbp-24]
	imul	r11, 20
	add	r10, r11
	add	r10, 16
	mov	r10d, dword [r10]
	movsx	r10, r10d
	mov	r11, 200
	cmp	r10, r11
	setle	r10b
	test	r10b, r10b
	jz	.L8
	mov	r10, qword [rbp-16]
	mov	r11, qword [rbp-24]
	imul	r11, 20
	add	r10, r11
	mov	rdi, r10
	lea	r10, [rel particle_init]
	call	r10
.L8:
	lea	r10, [rbp-24]
	mov	r11, qword [rbp-24]
	mov	r12, 1
	add	r11, r12
	mov	qword [r10], r11
.L7:
	mov	r10, qword [rbp-24]
	mov	r11, 10000
	cmp	r10, r11
	setl	r10b
	test	r10b, r10b
	jnz	.L6
	lea	r10, [rel BeginDrawing]
	call	r10
	mov	r10, 4278979596
	mov	rdi, r10
	lea	r10, [rel ClearBackground]
	call	r10
	mov	r10, 10
	mov	edi, r10d
	mov	r10, 10
	mov	esi, r10d
	lea	r10, [rel DrawFPS]
	call	r10
	lea	r10, [rbp-24]
	mov	r11, 0
	mov	qword [r10], r11
	jmp	.L10
.L9:
	lea	r10, [rbp-28]
	mov	r11, qword [rbp-16]
	mov	r12, qword [rbp-24]
	imul	r12, 20
	add	r11, r12
	add	r11, 0
	mov	r11d, dword [r11]
	movsx	r11, r11d
	mov	r12, 256
	mov	rax, r11
	cqo
	idiv	r12
	mov	r11, rax
	mov	dword [r10], r11d
	lea	r10, [rbp-32]
	mov	r11, qword [rbp-16]
	mov	r12, qword [rbp-24]
	imul	r12, 20
	add	r11, r12
	add	r11, 4
	mov	r11d, dword [r11]
	movsx	r11, r11d
	mov	r12, 256
	mov	rax, r11
	cqo
	idiv	r12
	mov	r11, rax
	mov	dword [r10], r11d
	lea	r10, [rbp-36]
	mov	r11, qword [rbp-16]
	mov	r12, qword [rbp-24]
	imul	r12, 20
	add	r11, r12
	add	r11, 16
	mov	r11d, dword [r11]
	movsx	r11, r11d
	mov	r12, 100
	mov	rax, r11
	cqo
	idiv	r12
	mov	r11, rax
	mov	dword [r10], r11d
	mov	r10d, dword [rbp-28]
	mov	edi, r10d
	mov	r10d, dword [rbp-32]
	mov	esi, r10d
	mov	r10d, dword [rbp-36]
	mov	edx, r10d
	mov	r10d, dword [rbp-36]
	mov	ecx, r10d
	mov	r10, 4278190335
	mov	r8, r10
	lea	r10, [rel DrawRectangle]
	call	r10
	lea	r10, [rbp-24]
	mov	r11, qword [rbp-24]
	mov	r12, 1
	add	r11, r12
	mov	qword [r10], r11
.L10:
	mov	r10, qword [rbp-24]
	mov	r11, 10000
	cmp	r10, r11
	setl	r10b
	test	r10b, r10b
	jnz	.L9
	lea	r10, [rel EndDrawing]
	call	r10
.L5:
	lea	r10, [rel WindowShouldClose]
	call	r10
	mov	r10b, al
	movzx	r10, r10b
	mov	r11, 1
	cmp	r10, r11
	setne	r10b
	test	r10b, r10b
	jnz	.L4
	lea	r10, [rel CloseWindow]
	call	r10
	mov	r10, qword [rbp-16]
	mov	rdi, r10
	lea	r10, [rel free]
	call	r10
	mov	r10, 0
	mov	eax, r10d
	jmp	.L1
.L1:
	add	rsp, 48
	pop	rbp
	ret

section .data
	printi_s: db "%ld", 10, 0

section .note.GNU-stack

