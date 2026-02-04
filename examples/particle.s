section .text
	extern	printf

printi:
	mov	rsi, rdi
	xor	eax, eax
	mov	rdi, printi_s
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
	extern	DrawText
	extern	DrawFPS
	extern	GetRandomValue
	global	particle_init
particle_init:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	mov	qword [rbp-8], rdi
	mov	r10, 400
	mov	r11, 256
	imul	r10, r11
	mov	r11, qword [rbp-8]
	add	r11, 0
	mov	dword [r11], r10d
	mov	r10, 300
	mov	r11, 256
	imul	r10, r11
	mov	r11, qword [rbp-8]
	add	r11, 4
	mov	dword [r11], r10d
	mov	r10, 0
	mov	edi, r10d
	mov	r10, 512
	mov	esi, r10d
	lea	r10, [rel GetRandomValue]
	call	r10
	mov	r10d, eax
	movsx	r10, r10d
	mov	r11, 256
	sub	r10, r11
	mov	r11, 2
	imul	r10, r11
	mov	r11, qword [rbp-8]
	add	r11, 8
	mov	dword [r11], r10d
	mov	r10, 0
	mov	edi, r10d
	mov	r10, 512
	mov	esi, r10d
	lea	r10, [rel GetRandomValue]
	call	r10
	mov	r10d, eax
	movsx	r10, r10d
	mov	r11, 256
	sub	r10, r11
	mov	r11, 2
	imul	r10, r11
	mov	r11, qword [rbp-8]
	add	r11, 12
	mov	dword [r11], r10d
	mov	r10, 200
	mov	edi, r10d
	mov	r10, 1000
	mov	esi, r10d
	lea	r10, [rel GetRandomValue]
	call	r10
	mov	r10d, eax
	mov	r11, qword [rbp-8]
	add	r11, 16
	mov	dword [r11], r10d
.L0:
	xor	rax, rax
	add	rsp, 16
	pop	rbp
	ret

	global	main
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 32
	mov	r10, 800
	mov	edi, r10d
	mov	r10, 600
	mov	esi, r10d
	mov	r10, LS0
	mov	rdx, r10
	lea	r10, [rel InitWindow]
	call	r10
	mov	r10, 60
	mov	edi, r10d
	lea	r10, [rel SetTargetFPS]
	call	r10
	mov	r10, 20
	mov	r11, 10000
	imul	r10, r11
	mov	rdi, r10
	lea	r10, [rel malloc]
	call	r10
	mov	r10, rax
	lea	r11, [rbp-8]
	mov	qword [r11], r10
	mov	r10, 0
	lea	r11, [rbp-16]
	mov	qword [r11], r10
	jmp	.L3
.L2:
	mov	r10, qword [rbp-8]
	mov	r11, qword [rbp-16]
	imul	r11, 20
	add	r10, r11
	mov	rdi, r10
	lea	r10, [rel particle_init]
	call	r10
.L5:
	mov	r10, qword [rbp-16]
	mov	r11, 1
	add	r10, r11
	lea	r11, [rbp-16]
	mov	qword [r11], r10
.L3:
	mov	r10, qword [rbp-16]
	mov	r11, 10000
	cmp	r10, r11
	setl	r10b
	test	r10b, r10b
	jnz	.L2
.L4:
	jmp	.L7
.L6:
	mov	r10, 0
	lea	r11, [rbp-16]
	mov	qword [r11], r10
	jmp	.L10
.L9:
	mov	r10, qword [rbp-8]
	mov	r11, qword [rbp-16]
	imul	r11, 20
	add	r10, r11
	add	r10, 0
	mov	r10d, dword [r10]
	mov	r11, qword [rbp-8]
	mov	r12, qword [rbp-16]
	imul	r12, 20
	add	r11, r12
	add	r11, 8
	mov	r11d, dword [r11]
	add	r10d, r11d
	mov	r11, qword [rbp-8]
	mov	r12, qword [rbp-16]
	imul	r12, 20
	add	r11, r12
	add	r11, 0
	mov	dword [r11], r10d
	mov	r10, qword [rbp-8]
	mov	r11, qword [rbp-16]
	imul	r11, 20
	add	r10, r11
	add	r10, 4
	mov	r10d, dword [r10]
	mov	r11, qword [rbp-8]
	mov	r12, qword [rbp-16]
	imul	r12, 20
	add	r11, r12
	add	r11, 12
	mov	r11d, dword [r11]
	add	r10d, r11d
	mov	r11, qword [rbp-8]
	mov	r12, qword [rbp-16]
	imul	r12, 20
	add	r11, r12
	add	r11, 4
	mov	dword [r11], r10d
	mov	r10, qword [rbp-8]
	mov	r11, qword [rbp-16]
	imul	r11, 20
	add	r10, r11
	add	r10, 16
	mov	r10d, dword [r10]
	movsx	r10, r10d
	mov	r11, 5
	sub	r10, r11
	mov	r11, qword [rbp-8]
	mov	r12, qword [rbp-16]
	imul	r12, 20
	add	r11, r12
	add	r11, 16
	mov	dword [r11], r10d
	mov	r10, qword [rbp-8]
	mov	r11, qword [rbp-16]
	imul	r11, 20
	add	r10, r11
	add	r10, 16
	mov	r10d, dword [r10]
	movsx	r10, r10d
	mov	r11, 200
	cmp	r10, r11
	setle	r10b
	test	r10b, r10b
	jz	.L13
	mov	r10, qword [rbp-8]
	mov	r11, qword [rbp-16]
	imul	r11, 20
	add	r10, r11
	mov	rdi, r10
	lea	r10, [rel particle_init]
	call	r10
.L13:
.L12:
	mov	r10, qword [rbp-16]
	mov	r11, 1
	add	r10, r11
	lea	r11, [rbp-16]
	mov	qword [r11], r10
.L10:
	mov	r10, qword [rbp-16]
	mov	r11, 10000
	cmp	r10, r11
	setl	r10b
	test	r10b, r10b
	jnz	.L9
.L11:
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
	mov	r10, 0
	lea	r11, [rbp-16]
	mov	qword [r11], r10
	jmp	.L15
.L14:
	mov	r10, qword [rbp-8]
	mov	r11, qword [rbp-16]
	imul	r11, 20
	add	r10, r11
	add	r10, 0
	mov	r10d, dword [r10]
	movsx	r10, r10d
	mov	r11, 256
	mov	rax, r10
	cqo
	idiv	r11
	mov	r10, rax
	lea	r11, [rbp-20]
	mov	dword [r11], r10d
	mov	r10, qword [rbp-8]
	mov	r11, qword [rbp-16]
	imul	r11, 20
	add	r10, r11
	add	r10, 4
	mov	r10d, dword [r10]
	movsx	r10, r10d
	mov	r11, 256
	mov	rax, r10
	cqo
	idiv	r11
	mov	r10, rax
	lea	r11, [rbp-24]
	mov	dword [r11], r10d
	mov	r10, qword [rbp-8]
	mov	r11, qword [rbp-16]
	imul	r11, 20
	add	r10, r11
	add	r10, 16
	mov	r10d, dword [r10]
	movsx	r10, r10d
	mov	r11, 100
	mov	rax, r10
	cqo
	idiv	r11
	mov	r10, rax
	lea	r11, [rbp-28]
	mov	dword [r11], r10d
	mov	r10d, dword [rbp-20]
	movsx	r10, r10d
	mov	r11, 1
	sub	r10, r11
	mov	edi, r10d
	mov	r10d, dword [rbp-24]
	movsx	r10, r10d
	mov	r11, 1
	sub	r10, r11
	mov	esi, r10d
	mov	r10d, dword [rbp-28]
	movsx	r10, r10d
	mov	r11, 2
	add	r10, r11
	mov	edx, r10d
	mov	r10d, dword [rbp-28]
	movsx	r10, r10d
	mov	r11, 2
	add	r10, r11
	mov	ecx, r10d
	mov	r10, 4278190080
	mov	r8, r10
	lea	r10, [rel DrawRectangle]
	call	r10
	mov	r10d, dword [rbp-20]
	mov	edi, r10d
	mov	r10d, dword [rbp-24]
	mov	esi, r10d
	mov	r10d, dword [rbp-28]
	mov	edx, r10d
	mov	r10d, dword [rbp-28]
	mov	ecx, r10d
	mov	r10, 4278190335
	mov	r8, r10
	lea	r10, [rel DrawRectangle]
	call	r10
.L17:
	mov	r10, qword [rbp-16]
	mov	r11, 1
	add	r10, r11
	lea	r11, [rbp-16]
	mov	qword [r11], r10
.L15:
	mov	r10, qword [rbp-16]
	mov	r11, 10000
	cmp	r10, r11
	setl	r10b
	test	r10b, r10b
	jnz	.L14
.L16:
	lea	r10, [rel EndDrawing]
	call	r10
.L7:
	lea	r10, [rel WindowShouldClose]
	call	r10
	mov	r10b, al
	test	r10b, r10b
	sete	r10b
	test	r10b, r10b
	jnz	.L6
.L8:
	lea	r10, [rel CloseWindow]
	call	r10
	mov	r10, qword [rbp-8]
	mov	rdi, r10
	lea	r10, [rel free]
	call	r10
	mov	r10, 0
	mov	eax, r10d
	jmp	.L1
	xor	rax, rax
.L1:
	add	rsp, 32
	pop	rbp
	ret

section .rodata
	printi_s: db "%ld", 10, 0
	LS0: db 80, 97, 114, 116, 105, 99, 108, 101, 115, 33, 0

section .note.GNU-stack

