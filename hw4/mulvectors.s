#include "mulvectors.h"

	.intel_syntax noprefix
	.text
	.globl mulVectors

mulVectors:
	push	ebp
	mov	ebp, esp
	sub	esp, 24
	mov	eax, 0
	mov	edx, 0
	mov	DWORD PTR [ebp-8], eax
	mov	DWORD PTR [ebp-4], edx
	mov	DWORD PTR [ebp-12], 0
	jmp	.L2
.L3:
	mov	eax, DWORD PTR [ebp-12]
	sal	eax, 3
	add	eax, DWORD PTR [ebp+8]
	fld	QWORD PTR [eax]
	mov	eax, DWORD PTR [ebp-12]
	sal	eax, 3
	add	eax, DWORD PTR [ebp+12]
	fld	QWORD PTR [eax]
	fmulp	st(1), st
	fld	QWORD PTR [ebp-8]
	faddp	st(1), st
	fstp	QWORD PTR [ebp-8]
	inc	DWORD PTR [ebp-12]
.L2:
	mov	eax, DWORD PTR [ebp-12]
	cmp	eax, DWORD PTR [ebp+16]
	setb	al
	test	al, al
	jne	.L3
	mov	eax, DWORD PTR [ebp-8]
	mov	edx, DWORD PTR [ebp-4]
	mov	DWORD PTR [ebp-24], eax
	mov	DWORD PTR [ebp-20], edx
	fld	QWORD PTR [ebp-24]
	leave
	ret

