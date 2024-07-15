;
;       Copyright (c) 1993,1994 by Borland International Inc.
;       All Rights Reserved.
;
;
;       Demo program to detect Ctrl-Break/Ctrl-C under DPMI.
;
;
;       Unfortunately DPMI is not really clear about when and how these
;       interrupts are reflected for protected vs. real mode, so to be on
;       the safe side, we hook interrupt 23h in both modes. Interrupt 
;       1Bh (the Ctrl-Break interrupt) is not reflected for protected mode 
;       at all.
;
;       Also, as per the DPMI spec the client can not terminate from the
;       Ctrl-Break handler.  Experience shows that also bad things can
;       happen if the clients attempts to terminate from real mode.
;       In addition, the context of the client is unknown from the handlers.
;
;
;       Based on all of the above the only thing one can do is to 
;       install a handler in both modes and set a global flag if the event
;       hits, and then check it frequently from the main program.
;
;
;       The functions HookCtrlHandler() and UnHookCtrlHandlers()
;       will install the handlers in the appropriate modes, and
;       the public ctrlBreak and ctrlC variables should be checked.
;       It is safe to call HookCtrlHandler() and/or UnHookCtrlHandler
;       multiple times.
;
;       The segments _REAL_DATA and _REAL_TEXT should be marked as
;       PRELOAD FIXED REAL in the def file.
;
;

_REAL_DATA      SEGMENT PARA PUBLIC 'REALDATA'

		PUBLIC  _ctrlBreak
_ctrlBreak      DW      0

		PUBLIC  _ctrlC
_ctrlC          DW      0

oldRCtrlCHandler        DD      0
oldRCtrlBrkHandler      DD      0
oldCtrlCHandler DD      0

codeSegPara     DW      _REAL_TEXT      ;; para address of real CS

RMSTRUC		STRUC

rm_di		LABEL WORD
rm_edi		dd	?

rm_si		LABEL WORD
rm_esi		dd	?

rm_bp		LABEL WORD
rm_ebp		dd	?

rm_res1		dd	?

rm_bx		LABEL WORD
rm_ebx		LABEL DWORD
rm_bl		db	?
rm_bh		db	?
rm_bhigh	dw	?

rm_dx		LABEL WORD
rm_edx		LABEL DWORD
rm_dl		db	?
rm_dh		db	?
rm_dhigh	dw	?

rm_cx		LABEL WORD
rm_ecx		LABEL DWORD
rm_cl		db	?
rm_ch		db	?
rm_chigh	dw	?

rm_ax		LABEL WORD
rm_eax		LABEL DWORD
rm_al		db	?
rm_ah		db	?
rm_ahigh	dw	?

rm_flags	dw	?
rm_es		dw	?
rm_ds		dw	?
rm_fs		dw	?
rm_gs		dw	?
rm_ip		dw	?
rm_cs		dw	?
rm_sp		dw	?
rm_ss		dw	?

RMSTRUC		ENDS

_REAL_DATA      ENDS


;;
;;      The real mode codeseg with the simple handlers
;;

_REAL_TEXT      SEGMENT BYTE PUBLIC 'REALCODE'

		ASSUME  DS:_REAL_DATA
		ASSUME  CS:_REAL_TEXT

RCtrlCHandler   PROC
		push    ax
		push    ds
		mov     ax, _REAL_DATA          ;; paragraph fixup
		mov     ds, ax
		mov     _ctrlC, 1
		pop     ds
		pop     ax
		iret
RCtrlCHandler   ENDP

RCtrlBrkHandler PROC
		push    ax
		push    ds
		mov     ax, _REAL_DATA          ;; paragraph fixup
		mov     ds, ax
		mov     _ctrlBreak, 1
		pop     ds
		pop     ax
		iret
RCtrlBrkHandler ENDP


_REAL_TEXT      ENDS


;;
;;      The prot mode code
;;


_TEXT           SEGMENT BYTE PUBLIC 'CODE'

		ASSUME  DS:_REAL_DATA
		ASSUME  CS:_TEXT

;;
;;	get/set real mode int vector using DPMI simulate real mode int. This
;;	is safer than calling the DPMI server. Some servers try to be smart
;;	about the 'Set Real Mode Interrupt Vector API'.
;;


getRMvect	PROC	C NEAR
		USES	di
		LOCAL	rmData:RMSTRUC

		mov	ah, 35h
		mov	al, bl
		mov	rmData.rm_ax, ax
		xor	ax, ax
		mov	rmData.rm_ss, ax
		mov	rmData.rm_sp, ax
		push	ss
		pop	es
		lea	di, rmData

		mov	ax, 0300h		;; simulate real mode int
		mov	bx, 021h
		xor	cx, cx
		int	31h

		mov	dx, rmData.rm_bx
		mov	cx, rmData.rm_es
		ret
getRMvect	ENDP

setRMvect	PROC	C NEAR
		USES	di
		LOCAL	rmData:RMSTRUC

		mov	ah, 25h
		mov	al, bl
		mov	rmData.rm_ax, ax
		mov	rmData.rm_dx, dx
		mov	rmData.rm_ds, cx
		xor	ax, ax
		mov	rmData.rm_ss, ax
		mov	rmData.rm_sp, ax
		push	ss
		pop	es
		lea	di, rmData

		mov	ax, 0300h		;; simulate real mode int
		mov	bx, 021h
		xor	cx, cx
		int	31h
		ret
setRMvect	ENDP


CtrlCHandler    PROC
		push    ax
		push    ds
		mov     ax, _REAL_DATA          ;; selector fixup
		mov     ds, ax
		mov     _ctrlC, 1
		pop     ds
		pop     ax
		iret
CtrlCHandler    ENDP



		PUBLIC  _HookCtrlHandlers
_HookCtrlHandlers PROC  FAR
		push    ds
		mov     ax, _REAL_DATA          ;; selector fixup
		mov     ds, ax

		cmp     word ptr oldCtrlCHandler+2, 0
		jne     @@installed
		
		;; get old vectors

		mov     bx, 023h
		mov     ax, 0204h       ;; get PM vector
		int     31h
		mov     word ptr oldCtrlCHandler+2, cx
		mov     word ptr oldCtrlCHandler+0, dx

		mov     bx, 023h
		call	getRMvect
		mov     word ptr oldRCtrlCHandler+2, cx
		mov     word ptr oldRCtrlCHandler+0, dx

		mov     bx, 01Bh
		call	getRMvect
		mov     word ptr oldRCtrlBrkHandler+2, cx
		mov     word ptr oldRCtrlBrkHandler+0, dx

		;; set new vectors

		mov     bx, 023h
		mov     cx, cs
		mov     dx, OFFSET CtrlCHandler
		mov     ax, 0205h       ;; set PM vector
		int     31h

		mov     bx, 023h
		mov     cx, codeSegPara
		mov     dx, OFFSET _REAL_TEXT:RCtrlCHandler
		call	setRMvect

		mov     bx, 01Bh
		mov     cx, codeSegPara
		mov     dx, OFFSET _REAL_TEXT:RCtrlBrkHandler
		call	setRMvect
@@installed:
		pop     ds
		ret

_HookCtrlHandlers ENDP



		PUBLIC  _UnhookCtrlHandlers
_UnhookCtrlHandlers PROC FAR
		push    ds
		mov     ax, _REAL_DATA          ;; selector fixup
		mov     ds, ax

		cmp     word ptr oldCtrlCHandler+2, 0
		je      @@notinstalled
		
		;; get old vectors

		mov     bx, 023h
		mov     cx, word ptr oldCtrlCHandler+2
		mov     dx, word ptr oldCtrlCHandler+0
		mov     ax, 0205h       ;; set PM vector
		int     31h

		mov     bx, 023h
		mov     cx, word ptr oldRCtrlCHandler+2
		mov     dx, word ptr oldRCtrlCHandler+0
		call	setRMvect

		mov     bx, 01Bh
		mov     cx, word ptr oldRCtrlBrkHandler+2
		mov     dx, word ptr oldRCtrlBrkHandler+0
		call	setRMvect

@@notinstalled:
		pop     ds
		ret

_UnhookCtrlHandlers ENDP




_TEXT           ENDS

		END
