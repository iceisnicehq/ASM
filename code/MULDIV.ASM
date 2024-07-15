%TITLE "MUL, DIV, IMUL, IDIV demo"

        IDEAL

        MODEL small
        STACK 256

        DATASEG

exCode          DB      0
opByte          DB      8
opWord          DW      100
sourceByte      DB      64
sourceWord      DW      4000

        CODESEG

Start:
        mov     ax, @data
        mov     ds, ax

        mov     al, [opByte]
        mul     [sourceByte]

        mov     ax, [opWord]
        mul     [sourceWord]

        mov     ax, [opWord]
        mul     ax

        mov     ax, [opWord]
        div     [sourceByte]

        mov     ax, [opWord]
        mov     dx, [opWord]
        div     [sourceWord]

Exit:
        mov     ax, 04Ch
        mov     al, [exCode]
        int     21h

        END     Start
