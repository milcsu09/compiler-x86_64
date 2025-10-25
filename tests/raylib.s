section .text
	extern	printf

printi:
	mov	rsi, rdi
	xor	eax, eax
	mov	edi, printi_s
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
	sub	rsp, 16
	lea	r10, [rbp-1]
	mov	r11, 0
	imul	r11, 1
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
	lea	r10, [rbp-15]
	mov	r11, 0
	imul	r11, 1
	add	r10, r11
	mov	r11, 72
	mov	byte [r10], r11b
	lea	r10, [rbp-15]
	mov	r11, 1
	imul	r11, 1
	add	r10, r11
	mov	r11, 101
	mov	byte [r10], r11b
	lea	r10, [rbp-15]
	mov	r11, 2
	imul	r11, 1
	add	r10, r11
	mov	r11, 108
	mov	byte [r10], r11b
	lea	r10, [rbp-15]
	mov	r11, 3
	imul	r11, 1
	add	r10, r11
	mov	r11, 108
	mov	byte [r10], r11b
	lea	r10, [rbp-15]
	mov	r11, 4
	imul	r11, 1
	add	r10, r11
	mov	r11, 111
	mov	byte [r10], r11b
	lea	r10, [rbp-15]
	mov	r11, 5
	imul	r11, 1
	add	r10, r11
	mov	r11, 44
	mov	byte [r10], r11b
	lea	r10, [rbp-15]
	mov	r11, 6
	imul	r11, 1
	add	r10, r11
	mov	r11, 32
	mov	byte [r10], r11b
	lea	r10, [rbp-15]
	mov	r11, 7
	imul	r11, 1
	add	r10, r11
	mov	r11, 119
	mov	byte [r10], r11b
	lea	r10, [rbp-15]
	mov	r11, 8
	imul	r11, 1
	add	r10, r11
	mov	r11, 111
	mov	byte [r10], r11b
	lea	r10, [rbp-15]
	mov	r11, 9
	imul	r11, 1
	add	r10, r11
	mov	r11, 114
	mov	byte [r10], r11b
	lea	r10, [rbp-15]
	mov	r11, 10
	imul	r11, 1
	add	r10, r11
	mov	r11, 108
	mov	byte [r10], r11b
	lea	r10, [rbp-15]
	mov	r11, 11
	imul	r11, 1
	add	r10, r11
	mov	r11, 100
	mov	byte [r10], r11b
	lea	r10, [rbp-15]
	mov	r11, 12
	imul	r11, 1
	add	r10, r11
	mov	r11, 33
	mov	byte [r10], r11b
	lea	r10, [rbp-15]
	mov	r11, 13
	imul	r11, 1
	add	r10, r11
	mov	r11, 0
	mov	byte [r10], r11b
	jmp	.L2
.L1:
	lea	r10, [rel BeginDrawing]
	call	r10
	mov	r10, 4278190080
	mov	rdi, r10
	lea	r10, [rel ClearBackground]
	call	r10
	lea	r10, [rbp-15]
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
	lea	r10, [rel CloseWindow]
	call	r10
	mov	r10, 0
	mov	eax, r10d
	jmp	.L0
.L0:
	add	rsp, 16
	pop	rbp
	ret

section .data
	printi_s: db "%ld", 10, 0

section .note.GNU-stack

