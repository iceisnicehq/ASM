.model small

.386
.stack 100h

.data
    a db -1
    b db 1
    c db -2
    d dd ?

.code
start:
    mov ax,@data    ; initialize DS to address
    mov ds,ax       ; of data segment

    mov bh,a        ; bh <- 0Fh
    mov bl,b        ; bl <- 01h (bx <- 0F01)
    mov cl,c        ; cl <- 0E

    ; sign extend bh into di
    movsx di,bh     ; 
    mov ax,di
    sal di,4        ; shift di 4 bits to the left
    sal ax,2        ; shift ax 2 bits to the left
    sub di,ax       ; di = di - ax

    ; sign extend bl into eax
    movsx eax,bl
    imul di         ; multiply eax by di
    sal edx,16      ; shift edx 16 bits to the left
    movzx eax,ax    ; move ax into eax, zero-extending
    or eax,edx      ; eax = eax | edx

    ; sign extend cl into edx
    movsx edx,cl
    sub eax,edx     ; eax = eax - edx
    add eax,1       ; eax = eax + 1
    mov edi,eax     ; edi = eax
    jz STOP         ; jump to STOP if eax is zero

    ; calculate some value using bh
    mov al,bh
    imul al         ; multiply al by itself
    sal ax,1        ; shift ax 1 bit to the left
    mov si,ax
    cwde            ; convert word to double word
    sal eax,2       ; shift eax 2 bits to the left
    movsx esi,si    ; sign extend si into esi
    add esi,eax     ; esi = esi + eax

    ; calculate some value using cl
    movsx dx,cl
    mov bp,dx
    sal dx,3        ; shift dx 3 bits to the left
    sal bp,1        ; shift bp 1 bit to the left
    add dx,bp       ; dx = dx + bp
    movsx cx,cl
    add dx,cx       ; dx = dx + cx
    movsx ebp,dx    ; sign extend dx into ebp

    ; calculate some value using bl
    xor eax,eax
    movsx cx,bl
    mov al,bl
    imul cl         ; multiply al by cl
    imul cx         ; multiply eax by cx
    sal edx,16      ; shift edx 16 bits to the left
    or eax,edx      ; eax = eax | edx
    imul ebp        ; multiply eax by ebp

    ; calculate some value
    xor ebp,ebp
    sub esi,eax     ; esi = esi - eax
    sbb ebp,edx     ; ebp = ebp - edx with borrow

    ; divide some value
    mov edx,ebp
    mov eax,esi
    idiv edi        ; divide eax by edi
    mov d,eax       ; store result in d

    ; clear registers
    xor ax, ax
    xor bx, bx
    xor cx, cx
    xor dx, dx

    ; move values around
    mov eax, d
    xor eax, eax
    mov eax, ebx

    mov ebx, d
    mov edx, d
    mov ecx, d

STOP:
    mov ah,4ch
    int 21h
    end start
