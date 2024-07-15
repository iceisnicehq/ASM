%TITLE "EXERCISE 4.6"

        IDEAL

        MODEL small
        STACK 256

        DATASEG
exCode          DB      0
initval         DB      05Fh

        CODESEG
Start:
        mov     ax, @data
        mov     ds, ax

        xor     ax, ax
        mov     ah, [initval]
        mov     cl, 4
        ror     ax, cl
        mov     dh, ah
        mov     cl, 4
        ror     al, cl
        mov     dl, al

Exit:
        mov     ah, 04Ch
        mov     al, [exCode]
        int     21h

        END     Start
