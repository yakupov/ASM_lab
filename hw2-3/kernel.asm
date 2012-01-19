jmp continue_loading

GDT:
	 dd  0,0
;           [ LIMIT    |      BASE    | PDLSTYPE   GD0ALIMT | BASE ]
	 db  0FFh, 0FFh, 00h, 00h, 01h, 10011010b, 11001111b, 00	; code	(selector = 8h == 000000000001 0 00b)
	 db  0FFh, 0FFh, 00h, 00h, 01h, 10010010b, 11001111b, 00	; data	(selector = 10h == 000000000010 0 00b)
	 db  0FFh, 0FFh, 00h, 80h, 0Bh, 10010010b, 01000000b, 00	; video	(selector = 18h == 000000000011 0 00b))
T0_DESC:	;selector == 000000000100 0 00 == 20h
	 db  067h, 00h	;limit
	 dw  T0_TSS	;base
	 db  01h	;base
	 db  10001001b	;PDLSTYPE
	 db  01000000b	;GD0ALIMT
	 db  00h	;limit
T1_DESC:	;selector == 000000000101 0 00 == 28h
	 db  067h, 00h	;limit
	 dw  T1_TSS	;base
	 db  01h	;base
	 db  10001001b	;PDLSTYPE
	 db  01000000b	;GD0ALIMT
	 db  00h	;limit
GDT_size	equ $-GDT
GDTR:
	 dw GDT_size-1
	 dd GDT



T0_TSS	db	68h dup(0)

T1_TSS:
	dd	0, 0, 0, 0, 0, 0, 0, 0
	dd	task1
	dd	0, 0, 0, 0, 0, 0xA000, 0, 0, 0
	dd	18h, 8h, 10h, 10h, 0, 0, 0, 0



IDT:
	 dd 0,0 ; 0
	 dw syscall_handler, 08h, 1000111000000000b, 0	 ; 1
	 dd 0,0 ; 2
	 dd 0,0 ; 3
	 dd 0,0 ; 4
	 dd 0,0 ; 5
	 dd 0,0 ; 6
	 dd 0,0 ; 7
	 dw int8_handler, 08h, 1000111000000000b, 0   ; 8  (IRQ 0 - timer)
	 dw int9_handler, 08h, 1000111000000000b, 0   ; 9  (IRQ 1 - keyboard)
	 dw int_EOI, 08h, 1000111000000000b, 0	 ; 10  (IRQ 2 - slave interrupt controller)
	 dw int_EOI, 08h, 1000111000000000b, 0	 ; 11  (IRQ 3 - COM2)
	 dw int_EOI, 08h, 1000111000000000b, 0	 ; 12  (IRQ 4 - COM1)
	 dw exGP_handler, 08h, 1000111000000000b, 0   ; 13  #GP - panic
	 dw int_EOI, 08h, 1000111000000000b, 0	 ; 14  (IRQ 6 - FDD)
	 dw int_EOI, 08h, 1000111000000000b, 0	 ; 15  (IRQ 7 - LPT1)
	 dw int_EOI, 08h, 1000111000000000b, 0	 ; 16 (8)
	 dw int_EOI, 08h, 1000111000000000b, 0	 ; 17 (9)
	 dw int_EOI, 08h, 1000111000000000b, 0	 ; 18 (10)
	 dw int_EOI, 08h, 1000111000000000b, 0	 ; 19 (11)
	 dw int_EOI, 08h, 1000111000000000b, 0	 ; 20 (12)
	 dw int_EOI, 08h, 1000111000000000b, 0	 ; 21 (13)
	 dw int_EOI, 08h, 1000111000000000b, 0	 ; 22 (14)
	 dw int_EOI, 08h, 1000111000000000b, 0	 ; 23 (15)
IDT_size	equ $-IDT
IDTR:
	dw IDT_size-1
	dd IDT

continue_loading:
	; enable A20
	in	al, 92h
	or	al, 2
	out	92h, al

	; disable interrupts
	cli
	in	al, 70h
	or	al, 80h
	out	70h, al  ; disable NMI

	; calc linear GDT address
	xor	eax, eax
	mov	ax, cs
	shl	eax, 4
	add	eax, GDT
	mov	[GDTR+2], eax
	; load GDTR
	lgdt fword [GDTR]


	; calc linear IDT address
	xor	eax, eax
	mov	ax, cs
	shl	eax, 4
	add	eax, IDT
	mov	[IDTR+2], eax
	; load IDTR
	lidt fword [IDTR]


	; switch to PM
	mov	eax, cr0
	or	al, 1
	mov	cr0, eax

	; finally LJ into PM
	jmp  00001000b:PROTECTED_ENTRY


; =============================
;    PM code
; =============================
use32
PROTECTED_ENTRY:
    mov  ax, 00010000b	; DATA
    mov  ss, ax
    mov  ds, ax
    mov  esp, 0xFFFF

    mov  ax, 00011000b	; VIDEO
    mov  es, ax
    mov  edi, 0

    ;load task0
    mov	ax, 20h
    ltr ax

    ; enable interrupts and NMI
    in	 al, 70h
    and  al, 7Fh
    out  70h, al
    sti

    mov  dword [cursor], 80
    mov  esi, string ;test it
    int 1
    mov  dword [cursor], 160


task0:
    cmp  [timer], 60
    jae  @f
    jmp  task0
@@:
    ;switch to task1
    mov  [timer], 0
    db	0x0EA
    dd	0
    dw	28h

    ;print something
    mov  dword [cursor], 80
    mov  esi, string
    int 1
    mov  dword [cursor], 160

    jmp task0


; =============================================
; Interrupt handlers
; =============================================

; IRQ 0 - timer
int8_handler:
    push eax
    mov eax, [timer]
    call print_eax
    pop eax

;    inc  byte [es:0]
    inc  dword [timer]
    jmp  int_EOI


; IRQ 1 - kb
int9_handler:
    push ax
    push edi
    xor  ax, ax

    ; request pos. code
    in	 al, 060h
    dec  al

    ; do not process releases
    mov  ah, al
    and  ah, 80h
    jnz clear_request

    ; convert pos. code to ASCII
    and  al, 7Fh
    push edi
    mov  edi, ascii
    add  di, ax
    mov  al, [edi]
    pop  edi

    ; print symbols
    mov  edi, dword [cursor]
    shl  edi, 1
    mov  byte [es:edi], al
    inc  dword [cursor]

    ; send confirmation to KB port
    in	 al, 061h
    or	 al, 80
    out  061h, al
    xor  al, 80
    out  061h, al

clear_request:
    pop  edi
    pop  ax
    jmp  int_EOI


; Empty handler. Sends End-Of-Interrupt (EOI) to controller
int_EOI:
    push ax
    mov  al, 20h
    out  020h, al	; ... to Master controller
    out  0a0h, al	; ... to Slave controller
    pop  ax
    iretd


; panic
exGP_handler:
    pop  eax	;err code
    mov  esi, gp
    int  1
    iretd


; Syscall INT 1 - print string
; String in DS:ESI
syscall_handler:
    pushad
  _puts:
    lodsb
    mov  edi, dword [cursor]
    mov  [es:edi*2], al
    inc  dword [cursor]
    test al, al
    jnz  _puts
    popad
    iretd



; ===========================
; Tasks
; ===========================

task1:
	sti

	cmp  [timer], 60
	jae  @f
	jmp  task1
@@:
	;switch to task0
	mov  [timer], 0
	db	0x0EA
	dd	0
	dw	20h

	;print something
	mov  dword [cursor], 80
	mov  esi, string1
	int 1
	mov  dword [cursor], 160

	jmp task1


; ===========================
; Useful functions
; ===========================

print_eax:
	push eax
	push ebx
	push ecx
	push edx

	mov ebx, 10
	mov ecx, 1
	xor edx, edx
@@:
	div ebx
	mov dh, 07h
	add dl, 30h
	mov [buffer+2*ecx], dx
	inc ecx
	xor edx, edx
	or eax, eax
	jnz @b	
	
	xor ebx, ebx
	dec ecx

@@:
	mov ax, [buffer+2*ecx]
	mov [es:2*ebx], ax
	dec ecx
	inc ebx
	or ecx, ecx
	jnz @b

@@:
	pop edx
	pop ecx
	pop ebx
	pop eax
	ret


; ===========================
; Data
; ===========================

; Scan-code to ASCII conversion table
ascii	db 0,'1234567890-+',0,0,'QWERTYUIOP[]',0,0,'ASDFGHJKL;',"'`",0,0,'ZXCVBNM,./',0,'*',0,' ',0, 0,0,0,0,0,0,0,0,0,0, 0,0, '789-456+1230.', 0,0

; just data
string	db '  task0 working.........................', 0
string1	db '  task1 working..................', 0
gp db '** GENERAL PROTECTION FAULT **',0

; cursor pos
cursor	dd 0

; time
timer	dd 0

; buffer
buffer	rw 100
