		org	100h

		mov	di, arg1
		mov	cx, numsize
		call	read_long

		mov	di, arg2
		mov	cx, numsize
		call	read_long

		mov	si, arg1
		mov	cx, numsize
		call	add_2long

		call	print_long

		mov	ax, 4c00h
		int	21h

; es:di - long number
; cx    - length in words
fill_zeros:
		push	cx
		push	di
		xor	ax, ax
		rep	stosw
		pop	di
		pop	cx
		ret

; ds:di - long number
; cx    - length in words
; bx    - summand
add_long:
		push	ax
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
		pop	ax
		ret

; ds:di - long number 1
; ds:si - long number 2
; cx    - length in words
; result is stored in ds:di
add_2long:
		push	cx
		push	di

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
		pop	di
		pop	cx
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
		push	es
		mov	ax, ds
		mov	es, ax
		call	fill_zeros
		pop	es

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
		push	dx
		mov	ah, 2
		int	21h
		pop	dx

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

crlf:		db	13,10,'$'

numsize = 1024
arg1:		rw	numsize
arg2:		rw	numsize

print_long_buf_size = 8192

print_long_buf: rb	print_long_buf_size

