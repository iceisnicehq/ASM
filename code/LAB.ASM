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
mov ax,@data
mov ds,ax

mov bh,a
mov bl,b
mov cl,c

movsx di,bh
mov ax,di
sal di,4
sal ax,2
sub di,ax

movsx eax,bl
imul di
sal edx,16
movzx eax,ax
or eax,edx

movsx edx,cl
sub eax,edx
add eax,1
mov edi,eax
jz STOP



mov al,bh
imul al
sal ax,1
mov si,ax
cwde
sal eax,2
movsx esi,si
add esi,eax


movsx dx,cl
mov bp,dx
sal dx,3
sal bp,1
add dx,bp
movsx cx,cl
add dx,cx
movsx ebp,dx

xor eax,eax
movsx cx,bl
mov al,bl
imul cl
imul cx
sal edx,16
or eax,edx
imul ebp

xor ebp,ebp
sub esi,eax
sbb ebp,edx

mov edx,ebp
mov eax,esi
idiv edi
mov d,eax

STOP:
mov ah,4ch
int 21h
end start
