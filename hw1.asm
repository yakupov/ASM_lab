		org	100h

		mov	di, arg1
		mov	cx, numsize
		call	read_long

		mov	di, arg2
		mov	cx, numsize
		call	read_long

		mov	si, arg1
		mov	cx, numsize
		call	div_2long

		mov	di, res4
		call	print_long

		mov	ax, 4c00h
		int	21h

; es:di - long number
; cx    - length in words
fill_zeros:
		push	ax
		push	cx
		push	di
		xor	ax, ax
		rep	stosw
		pop	di
		pop	cx
		pop	ax
		ret

; ds:di - long number
; cx    - length in words
; bx    - summand
add_long:
		push	ax
		push	bx
		push	cx
		push	dx
		push	di

@@:
		or	cx, cx
		jz	@f

		xor	dx, dx
		add	[di], bx
		adc	dx, 0
		mov	bx, dx
		add	di, 2
		dec	cx
		jmp	@b

@@:
		pop	di
		pop	dx
		pop	cx
		pop	bx
		pop	ax
		ret

; ds:di - long number 1
; ds:si - long number 2
; cx    - length in words
; result is stored in ds:di
add_2long:
		push	ax
		push	bx
		push	cx
		push	di
		push	si

		xor	ax, ax
@@:
		or	cx, cx
		jz	@f

		xor	bx, bx
		add	ax, [di]
		adc	bx, 0
		add	ax, [si]
		adc	bx, 0
		mov	[di], ax
		mov	ax, bx

		add	di, 2
		add	si, 2
		dec	cx
		jmp	@b

@@:
		pop	si
		pop	di
		pop	cx
		pop	bx
		pop	ax
		ret

; ds:di - long number
; cx    - length in words
; bx    - multiplier
mul_long:
		push	ax
		push	bx
		push	cx
		push	dx
		push	di
		push	si

		xor	si, si

@@:
		or	cx, cx
		jz	@f

		mov	ax, [di]
		mul	bx
		add	ax, si
		adc	dx, 0
		mov	[di], ax
		add	di, 2
		mov	si, dx

		dec	cx
		jmp	@b

@@:
		pop	si
		pop	di
		pop	dx
		pop	cx
		pop	bx
		pop	ax
		ret

; ds:di - long number
; cx    - length in words
; bx    - divisor
; returns remainder in dx
div_long:
		push	ax
		push	cx
		push	di

		add	di, cx
		add	di, cx
		xor	dx, dx

@@:
		or	cx, cx
		jz	@f

		sub	di, 2
		mov	ax, [di]
		div	bx
		mov	[di], ax

		dec	cx
		jmp	@b
@@:
		pop	di
		pop	cx
		pop	ax
		ret

; ds:di - long number
; cx    - length in words
; returns result in cf
is_zero:
		push	cx
		push	di
@@:
		or	cx, cx
		jz	@f
		mov	ax, [di]
		add	di, 2
		dec	cx
		or	ax, ax
		jz	@b
		clc
		jmp	is_zero_quit
@@:
		stc
is_zero_quit:
		pop	di
		pop	cx
		ret

; ds:di - long number
; cx    - length in words
print_long:
		mov	si, print_long_buf + print_long_buf_size
		dec	si
		mov	byte [si], '$'
		dec	si
		mov	byte [si], 10
		dec	si
		mov	byte [si], 13
		mov	bx, 10

@@:
		call	div_long
		add	dl, '0'
		dec	si
		mov	[si], dl

		call	is_zero
		jnz	@b

		mov	ah, 9
		mov	dx, si
		int	21h

		ret

; ds:di - long number
; cx    - length in words
read_long:
;		push	es
;		mov	ax, ds
;		mov	es, ax
		call	fill_zeros
;		pop	es

read_long_read_again:
		mov	ah, 8
		int	21h

		cmp	al, 13
		jz	read_long_quit

		cmp	al, '0'
		jb	read_long_read_again

		cmp	al, '9'
		ja	read_long_read_again

		mov	dl, al
;		push	dx
		mov	ah, 2
		int	21h
;		pop	dx

		mov	bx, 10
		call	mul_long

		sub	dl, '0'

		xor	bx, bx
		mov	bl, dl
		call	add_long

		jmp	read_long_read_again

read_long_quit:
		mov	ah, 9
		mov	dx, crlf
		int	21h

		ret

; ds:di - long number 2
; ds:si - long number 1
; cx    - length in words
; result is stored in ds:si
subtr_2long:
		push	ax
		push	bx
		push	cx
		push	dx
		push	di
		push	si
		xor	dx, dx
s2l1:
		or	cx, cx
		jz	s2l2

		xor	ax, ax
		xor	bx, bx
		add	ax, [si]
		sbb	ax, [di]
		adc	bx, 0
		sbb	ax, dx
		adc	bx, 0
		mov 	dx, bx
		mov	[si], ax

		add	di, 2
		add	si, 2
		dec	cx
		jmp	s2l1

s2l2:
		pop	si
		pop	di
		pop	dx
		pop	cx
		pop	bx
		pop	ax
		ret

; ds:di - long number 2
; ds:si - long number 1
; cx    - length in words
; result is stored in res2
mul_2long:
		push	ax
		push	bx
		push	cx
		push	dx
		push	di
		push	si

		xor	dx, dx
		push	di
		mov	di, res2
		call	fill_zeros
		pop 	di


m2l1:
		or	cx, cx
		jz	m2l3

		push	cx
		push	si
		push	di

		mov	ax, [di] ;current multiplier (si - arg1, di - arg2)
		or	ax, ax
		jz	m2l2

		mov	di, res1
		mov	cx, numsize
		call	fill_zeros
		call	add_2long
		
		mov	bx, 100h
		mov	cx, dx
@@:
		or	cx, cx
		jz	@f

		push	cx
		mov	cx, numsize
		call	mul_long
		call	mul_long	;we need to multiply it by 10^4h, it doesn't fit 2 bytes :(
		pop	cx

		dec	cx
		jmp	@b
@@:

		mov	bx, ax
		mov	cx, numsize
		call	mul_long

		mov	di, res2
		mov	si, res1
		call	add_2long
m2l2:
		inc	dx
		pop 	di
		pop 	si
		pop	cx

		add	di, 2
		dec	cx
		jmp	m2l1

m2l3:
		pop	si
		pop	di
		pop	dx
		pop	cx
		pop	bx
		pop	ax
		ret






; ds:si - long number 1
; ds:di - long number 2
; cx    - length in words
; result is stored in res2
div_2long:
		push	ax
		push	bx
		push	cx
		push	dx
		push	di
		push	si

		mov	dx, si		;divided
		mov	bx, di		;divisor

		mov	di, res3	;curr min value
		call	fill_zeros
		mov	di, res5	;curr max value
		call	fill_zeros
		call	add_2long

d2l1:
		mov	di, res4

		push	bx
		push	dx
		call	fill_zeros
		mov 	si, res3
		call	add_2long
		mov 	si, res5
		call	add_2long
		mov	bx, 2
		call	div_long
		pop	dx
		pop	bx

		mov	si, res3
		call	compare_2long
		cmp	ax, 1
		jz	d2l1.5

		mov	si, res5
		call	compare_2long
		cmp	ax, 1
		jz	d2l1.5

		mov	si, bx
		call	mul_2long
		
		mov	si, res2
		mov 	di, dx
		call	compare_2long

		cmp	ax, 1
		jz	d2l2
		ja	@f

		mov	di, res3
		call	fill_zeros
		mov	si, res4
		call	add_2long
		jmp	d2l1
@@:
		mov	di, res5
		call	fill_zeros
		mov	si, res4
		call	add_2long
		jmp	d2l1
d2l1.5:
		mov	si, res5
		mov	di, bx
		call	mul_2long
		mov	si, res2
		mov	di, dx
		call	compare_2long
		cmp	ax, 2
		jz	d2l2
		mov	di, res4
		call	fill_zeros
		call	add_2long
d2l2:
		pop	si
		pop	di
		pop	dx
		pop	cx
		pop	bx
		pop	ax
		ret


; result is stored in ax
; 2 if[si...] > [di...], 1 if equal, 0 if <
compare_2long:
		push	di
		push	si
		push	cx
		mov 	ax, 1
c2l1:
		or	cx, cx
		jz	c2l2

		cmpsw
		jz	@f
		mov	ax, 2
		jae	@f
		mov 	ax, 0
@@:
		dec	cx
		jmp	c2l1
c2l2:
		pop	cx
		pop	si
		pop	di
		ret




crlf:		db	13,10,'$'

numsize = 1024
arg1:		rw	numsize
arg2:		rw	numsize
res1:		rw	numsize
res2:		rw	numsize
res3:		rw	numsize
res4:		rw	numsize
res5:		rw	numsize

print_long_buf_size = 8192

print_long_buf: rb	print_long_buf_size

