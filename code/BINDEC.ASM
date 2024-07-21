%TITLE "perevody"

IDEAL

MODEL small
STACK 256

DATASEG

numb    dw  01110110b   

CODESEG
Start:
        mov ax, @data
        mov ds, ax
        xor ax, ax
        mov ax, [numb]
        neg ah
Exit:
        mov ah, 04Ch
        mov al, 0
        int 21h
END     Start
