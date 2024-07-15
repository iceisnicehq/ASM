%TITLE "AND, OR, XOR demo"

        IDEAL

        MODEL small
        STACK 256

        DATASEG

exCode          DB      0
sourceWord      DW      0ABh
wordMask        DW      0CFh

        CODESEG

Start:
        mov     ax, @data
        mov     ds, ax

        mov     ax, [sourceWord]
        mov     bx, ax
        mov     cx, ax
        mov     dx, ax

        and     ax, [wordMask]

        or      bx, [wordMask]

        xor     cx, [wordMask]

        xor     dx, dx

        mov     ax, [sourceWord]
        test    ah, 02h
Exit:
        mov     ah, 04Ch
        mov     al, [exCode]
        int     21h

        END     Start
