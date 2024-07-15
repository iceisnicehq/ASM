%TITLE "ADD, SUB, INC, DEC demo"

        IDEAL

        MODEL small
        STACK 256

        DATASEG

exCode          DB      0
count           DW      1

        CODESEG

Start:
        mov     ax, @data
        mov     ds, ax

        mov     ax, 4
        mov     bx, 2
        add     ax, bx

        mov     cx, 8
        add     cx, [count]

        add     [count], cx

        inc     [count]
        dec     [count]
        inc     ax
        dec     cx

Exit:
        mov     ah, 04Ch
        mov     al, [exCode]
        int     21h

        END     Start

        
