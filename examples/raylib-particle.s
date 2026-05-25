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
	mov	r8, 102400
	mov	r9, qword [rbp-8]
	add	r9, 0
	mov	dword [r9], r8d
	mov	r8, 76800
	mov	r9, qword [rbp-8]
	add	r9, 4
	mov	dword [r9], r8d
	mov	r10, 0
	mov	edi, r10d
	mov	r10, 512
	mov	esi, r10d
	lea	r10, [rel GetRandomValue]
	call	r10
	mov	r8d, eax
	movsx	r8, r8d
	mov	r9, 256
	sub	r8, r9
	mov	r9, 2
	imul	r8, r9
	mov	r9, qword [rbp-8]
	add	r9, 8
	mov	dword [r9], r8d
	mov	r10, 0
	mov	edi, r10d
	mov	r10, 512
	mov	esi, r10d
	lea	r10, [rel GetRandomValue]
	call	r10
	mov	r8d, eax
	movsx	r8, r8d
	mov	r9, 256
	sub	r8, r9
	mov	r9, 2
	imul	r8, r9
	mov	r9, qword [rbp-8]
	add	r9, 12
	mov	dword [r9], r8d
	mov	r10, 200
	mov	edi, r10d
	mov	r10, 1000
	mov	esi, r10d
	lea	r10, [rel GetRandomValue]
	call	r10
	mov	r8d, eax
	mov	r9, qword [rbp-8]
	add	r9, 16
	mov	dword [r9], r8d
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
	mov	r10, 200000
	mov	rdi, r10
	lea	r10, [rel malloc]
	call	r10
	mov	r8, rax
	lea	r9, [rbp-8]
	mov	qword [r9], r8
	mov	r8, 0
	lea	r9, [rbp-16]
	mov	qword [r9], r8
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
	lea	r8, [rbp-16]
	mov	r9, qword [r8]
	mov	r10, 1
	add	r9, r10
	mov	qword [r8], r9
.L3:
	mov	r8, qword [rbp-16]
	mov	r9, 10000
	cmp	r8, r9
	setl	r8b
	test	r8b, r8b
	jnz	.L2
.L4:
	jmp	.L7
.L6:
	mov	r8, 0
	lea	r9, [rbp-16]
	mov	qword [r9], r8
	jmp	.L10
.L9:
	mov	r8, qword [rbp-8]
	mov	r9, qword [rbp-16]
	imul	r9, 20
	add	r8, r9
	add	r8, 0
	mov	r9d, dword [r8]
	mov	r10, qword [rbp-8]
	mov	r11, qword [rbp-16]
	imul	r11, 20
	add	r10, r11
	add	r10, 8
	mov	r10d, dword [r10]
	add	r9d, r10d
	mov	dword [r8], r9d
	mov	r8, qword [rbp-8]
	mov	r9, qword [rbp-16]
	imul	r9, 20
	add	r8, r9
	add	r8, 4
	mov	r9d, dword [r8]
	mov	r10, qword [rbp-8]
	mov	r11, qword [rbp-16]
	imul	r11, 20
	add	r10, r11
	add	r10, 12
	mov	r10d, dword [r10]
	add	r9d, r10d
	mov	dword [r8], r9d
	mov	r8, qword [rbp-8]
	mov	r9, qword [rbp-16]
	imul	r9, 20
	add	r8, r9
	add	r8, 16
	mov	r9d, dword [r8]
	movsx	r9, r9d
	mov	r10, 5
	sub	r9, r10
	mov	dword [r8], r9d
	mov	r8, qword [rbp-8]
	mov	r9, qword [rbp-16]
	imul	r9, 20
	add	r8, r9
	add	r8, 16
	mov	r8d, dword [r8]
	movsx	r8, r8d
	mov	r9, 200
	cmp	r8, r9
	setle	r8b
	test	r8b, r8b
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
	lea	r8, [rbp-16]
	mov	r9, qword [r8]
	mov	r10, 1
	add	r9, r10
	mov	qword [r8], r9
.L10:
	mov	r8, qword [rbp-16]
	mov	r9, 10000
	cmp	r8, r9
	setl	r8b
	test	r8b, r8b
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
	mov	r8, 0
	lea	r9, [rbp-16]
	mov	qword [r9], r8
	jmp	.L15
.L14:
	mov	r8, qword [rbp-8]
	mov	r9, qword [rbp-16]
	imul	r9, 20
	add	r8, r9
	add	r8, 0
	mov	r8d, dword [r8]
	movsx	r8, r8d
	mov	r9, 256
	mov	rax, r8
	cqo
	idiv	r9
	mov	r8, rax
	lea	r9, [rbp-20]
	mov	dword [r9], r8d
	mov	r8, qword [rbp-8]
	mov	r9, qword [rbp-16]
	imul	r9, 20
	add	r8, r9
	add	r8, 4
	mov	r8d, dword [r8]
	movsx	r8, r8d
	mov	r9, 256
	mov	rax, r8
	cqo
	idiv	r9
	mov	r8, rax
	lea	r9, [rbp-24]
	mov	dword [r9], r8d
	mov	r8, qword [rbp-8]
	mov	r9, qword [rbp-16]
	imul	r9, 20
	add	r8, r9
	add	r8, 16
	mov	r8d, dword [r8]
	movsx	r8, r8d
	mov	r9, 100
	mov	rax, r8
	cqo
	idiv	r9
	mov	r8, rax
	lea	r9, [rbp-28]
	mov	dword [r9], r8d
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
	lea	r8, [rbp-16]
	mov	r9, qword [r8]
	mov	r10, 1
	add	r9, r10
	mov	qword [r8], r9
.L15:
	mov	r8, qword [rbp-16]
	mov	r9, 10000
	cmp	r8, r9
	setl	r8b
	test	r8b, r8b
	jnz	.L14
.L16:
	lea	r10, [rel EndDrawing]
	call	r10
.L7:
	lea	r10, [rel WindowShouldClose]
	call	r10
	mov	r8b, al
	test	r8b, r8b
	sete	r8b
	test	r8b, r8b
	jnz	.L6
.L8:
	lea	r10, [rel CloseWindow]
	call	r10
	mov	r10, qword [rbp-8]
	mov	rdi, r10
	lea	r10, [rel free]
	call	r10
.L1:
	xor	rax, rax
	add	rsp, 32
	pop	rbp
	ret

section .rodata
	printi_s: db "%ld", 10, 0
	LS0: db 80, 97, 114, 116, 105, 99, 108, 101, 115, 33, 0

section .bss

section .note.GNU-stack

