%TITLE "MOV demonstration"
        IDEAL
        MODEL small
        STACK 256

        DATASEG
exCode  DB      0
speed   DB      99

        CODESEG
Start:
        mov ax, @data
        mov ds, ax
        mov ax, 1
        mov bx, 2
        mov cx, 3
        mov dx, 4

        mov ah, [speed]
        mov si, offset speed
Exit:
        mov ah, 04Ch
        mov al, [exCode]
        int 21h
        END Start
