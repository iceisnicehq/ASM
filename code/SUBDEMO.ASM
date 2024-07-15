%TITLE "Subroutine demo"

        IDEAL

        MODEL small
        STACK 256

        DATASEG

excode          DB      0

        CODESEG

Start:
        mov     ax, @data
        mov     ds, ax

        mov     al, 1
        mov     bl, 2
        mov     cl, 3
        mov     dl, 4
        call    AddRegisters
        call    AddRegisters
        call    AddRegisters

Exit:
        mov     ah, 04Ch
        mov     al, [exCode]
        int     21h

; AddRegisters --- sums al, bl, cl, dl
; Input        --- al, bl, cl, dl --- 4 8-bit values
; Output       --- ax = al+bl+cl+dl
; Registers    --- ah, bh, ch, dh --- changed

PROC    AddRegisters
        xor     ah, ah
        xor     bh, bh
        xor     ch, ch
        xor     dh, dh
        add     ax, bx
        adc     ax, cx
        adc     ax, dx
        ret
ENDP    AddRegisters

        END     Start
