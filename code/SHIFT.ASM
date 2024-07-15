%TITLE "Shift instruction demo"

        IDEAL

        MODEL small
        STACK 256

        DATASEG

exCode          DB      0
operand         DB      0AAh

        CODESEG

Start:
        mov     ax, @data
        mov     ds, ax
        shl     [operand], 1
        shr     [operand], 1
        rol     [operand], 1
        ror     [operand], 1
        stc
        rcl     [operand], 1
        rcr     [operand], 1
        sal     [operand], 1
        sar     [operand], 1

Exit:
        mov ah, 04Ch
        mov al, [exCode]
        int 21h

        END Start
