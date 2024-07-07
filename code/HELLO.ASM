.MODEL SMALL
.STACK

.DATA
        Message DB "HELLO!", 13, 10, "$"
.CODE

START:
        mov ax, @Data
        mov ds, ax
        mov ah, 09h               
        mov dx, OFFSET Message
        int 21h
        mov ah, 4ch
        int 21h
END START
