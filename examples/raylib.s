section .text
	extern	printf

printi:
	mov	rsi, rdi
	xor	eax, eax
	mov	rdi, printi_s
	jmp	printf

	extern	InitWindow
	extern	CloseWindow
	extern	WindowShouldClose
	extern	BeginDrawing
	extern	EndDrawing
	extern	ClearBackground
	extern	DrawText
	extern	DrawFPS
	global	main
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 0
	mov	r10, 800
	mov	edi, r10d
	mov	r10, 600
	mov	esi, r10d
	mov	r10, LS0
	mov	rdx, r10
	lea	r10, [rel InitWindow]
	call	r10
	jmp	.L2
.L1:
	lea	r10, [rel BeginDrawing]
	call	r10
	mov	r10, 4279374354
	mov	rdi, r10
	lea	r10, [rel ClearBackground]
	call	r10
	mov	r10, LS1
	mov	rdi, r10
	mov	r10, 175
	mov	esi, r10d
	mov	r10, 200
	mov	edx, r10d
	mov	r10, 72
	mov	ecx, r10d
	mov	r10, 4294967295
	mov	r8, r10
	lea	r10, [rel DrawText]
	call	r10
	mov	r10, 10
	mov	edi, r10d
	mov	r10, 10
	mov	esi, r10d
	lea	r10, [rel DrawFPS]
	call	r10
	lea	r10, [rel EndDrawing]
	call	r10
.L2:
	lea	r10, [rel WindowShouldClose]
	call	r10
	mov	r10b, al
	movzx	r10, r10b
	mov	r11, 1
	cmp	r10, r11
	setne	r10b
	test	r10b, r10b
	jnz	.L1
.L3:
	lea	r10, [rel CloseWindow]
	call	r10
	mov	r10, 0
	mov	eax, r10d
	jmp	.L0
	xor	rax, rax
.L0:
	add	rsp, 0
	pop	rbp
	ret

section .rodata
	printi_s: db "%ld", 10, 0
	LS0: db 116, 105, 116, 108, 101, 0
	LS1: db 72, 101, 108, 108, 111, 44, 32, 119, 111, 114, 108, 100, 33, 0

section .note.GNU-stack

