%TITLE "test4_0"

.model small

.386
.stack 256

.data
        ExCode  db      0

.code

Start:
        mov     ax, @data
        mov     ds, ax
        xor     ax, ax
; intial definiton
        mov     ax, 08630h
        mov     bx, 00234h
        add     bx, 01000h
        mov     cx, 0005Ch
        mov     dx, 0FFD5h
        mov     di, 052h
        stc
        ; toggle of 
;;;;;;;;;;;;;;;;;
; testing part with nops to input commands
        mov cx, 10
        nop
        nop
        nop

Exit:
        mov     ah, 04Ch
        mov     al, [ExCode]
        int     21h
        End     Start


