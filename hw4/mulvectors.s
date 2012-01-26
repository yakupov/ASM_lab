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
        
	push	ebx
	push	ecx
        push	edx

        mov	eax, 1
        cpuid

        test    edx, 0x4000000		/* sse2 bit */
        jz	mulVectors_FPU

        test    edx, 0x80000		/* sse4.1 bit */
        jz	mulVectors_SSE2

	jmp	mulVectors_SSE41

mulVectors_FPU:
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
	jmp	return


mulVectors_SSE2:
//for i = 0..sz
	subps	xmm0, xmm0
	jmp	.L5
.L4:
	mov	eax, DWORD PTR [ebp-12]
	sal	eax, 3
	add	eax, DWORD PTR [ebp+8]	/* 1st arg */
	movlpd	xmm1, [eax]
	mov	eax, DWORD PTR [ebp-12]
	sal	eax, 3
	add	eax, DWORD PTR [ebp+12]	/* 2nd arg */
	movlpd	xmm2, [eax]
	mulsd	xmm2, xmm1
	addsd	xmm0, xmm2
	inc	DWORD PTR [ebp-12]
.L5:
	mov	eax, DWORD PTR [ebp-12]
	cmp	eax, DWORD PTR [ebp+16]	/* sz */
	jb	.L4
//end of for i = 0..sz
	movsd	QWORD PTR [ebp-8], xmm0
	jmp	return


mulVectors_SSE41:
	subps	xmm0, xmm0		/* result */
	mov	eax, DWORD PTR [ebp+16]	/* our sz */
	mov	ebx, eax
	and	ebx, 0xFFFFFFFE
	mov	DWORD PTR [ebp-8], ebx	/* evenized sz */
	xor	ebx, eax		/* "one more element left" flag */
	jmp	.L7

//for i = 0..sz	
.L6:
	mov	eax, DWORD PTR [ebp-12]
	sal	eax, 3
	add	eax, DWORD PTR [ebp+8]	/* 1st arg */
	movupd	xmm1, [eax]
	mov	eax, DWORD PTR [ebp-12]
	sal	eax, 3
	add	eax, DWORD PTR [ebp+12]	/* 2nd arg */
	movupd	xmm2, [eax]
	dppd	xmm1, xmm2, 243
	addsd	xmm0, xmm1
	inc	DWORD PTR [ebp-12]
	inc	DWORD PTR [ebp-12]

.L7:
	mov	eax, DWORD PTR [ebp-12]
	cmp	eax, DWORD PTR [ebp-8]	/* sz */
	jb	.L6
//end of for i = 0..sz

	or 	ebx, ebx
	jz .L8
//process last element
	mov	eax, DWORD PTR [ebp-12]
	sal	eax, 3
	add	eax, DWORD PTR [ebp+8]	/* 1st arg */
	movlpd	xmm1, [eax]
	mov	eax, DWORD PTR [ebp-12]
	sal	eax, 3
	add	eax, DWORD PTR [ebp+12]	/* 2nd arg */
	movlpd	xmm2, [eax]
	mulsd	xmm2, xmm1
	addsd	xmm0, xmm2
.L8:
	movsd	QWORD PTR [ebp-8], xmm0
	jmp	return


return:
	pop	edx
	pop	ecx
	pop	ebx
	fld	QWORD PTR [ebp-8]	/* return */
	leave
	ret
