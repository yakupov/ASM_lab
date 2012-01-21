#include "mulvectors.h"

	.intel_syntax noprefix
	.text
	.globl mulVectors

mulVectors:
	push	ebp
	mov	ebp, esp
	sub	esp, 24
	mov	DWORD PTR [ebp-8], 0	/* res */
	mov	DWORD PTR [ebp-4], 0	/* still res, 'cause we can't mov QWORD */
	mov	DWORD PTR [ebp-12], 0	/* i */

//for i = 0..sz
	jmp	.L2
.L3:
	mov	eax, DWORD PTR [ebp-12]
	sal	eax, 3
	add	eax, DWORD PTR [ebp+8]	/* 1st arg */
	fld	QWORD PTR [eax]
	mov	eax, DWORD PTR [ebp-12]
	sal	eax, 3
	add	eax, DWORD PTR [ebp+12]	/* 2nd arg */
	fld	QWORD PTR [eax]
	fmulp	st(1), st
	fld	QWORD PTR [ebp-8]	/* res */
	faddp	st(1), st
	fstp	QWORD PTR [ebp-8]
	inc	DWORD PTR [ebp-12]
.L2:
	mov	eax, DWORD PTR [ebp-12]
	cmp	eax, DWORD PTR [ebp+16]	/* sz */
	jb	.L3
//end of for i = 0..sz

	fld	QWORD PTR [ebp-8]	/* return */
	leave
	ret
