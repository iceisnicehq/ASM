%TITLE "Hello, World! in TASM"

.model small
.stack 256

.data
        strng     db      "Hello, World!", "$"

.code
Start:
        mov ax, @data
        mov ds, ax

        mov dx, OFFSET strng
        mov ah, 09h
        int 21h

Exit:
        mov ah, 04Ch
        mov al, 0
        int 21h

        END Start
