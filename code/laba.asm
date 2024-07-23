%TITLE "calculating an equation"
;— — — — — — — — — — — — — — — — — — — — — — — — — — 
;         (10*a^2 - 11*c*b^3)                       |
;   d = ---------------------                       |
;          (12*a*b - c +1)                          |
;— — — — — — — — — — — — — — — — — — — — — — — — — — 

.model small

.386
.stack 100h

.data
    excode   db 0
    a        dd -1
    b        dd 1
    c        dd -2
    d        dd ?

.code

Start:
    mov     ax,@data    ; initialize DS to address
    mov     ds, ax       ; of data segment
    xor     ax, ax
    ; 12*a*b - c + 1       
    mov     eax, a      ; set eax equal to a
    imul    b      ; eax = a * b
    fastimul edi, eax   , 12
    sub     edi, c
    add     edi, 1
    ; edx should be -9 or FFFFFFF7 (-9 in hex)

    ; now 11*c*b^3
    xor     eax, eax
    mov     eax, b      ; set eax equal to b
    imul    b           ; b^2
    imul    b           ; b^3
    imul    c         ; c*b^3
    fastimul ebx, eax, 11
    ; ebx should be FFFFFFEA (-22 in hex)

    ; calculating (10*a^2) in ecx
    xor     eax, eax      ; set ax to zero
    mov     eax, a      ; set eax equal to a
    imul    eax         ; a^2
    fastimul ecx, eax, 10
    ; ecx should be 0000000A (10 in hex)

    ;32 / -9 = -3 -> 0011  >  1100 > 1101 > FFFFFFFD (-3 in hex)
    sub ecx, ebx
    xchg ecx, eax
    idiv edi
    mov d, eax

Exit:
    mov     ah, 04Ch
    mov     al, [excode]
    int     21h
    End     Start

