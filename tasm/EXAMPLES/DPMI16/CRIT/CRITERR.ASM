;
;       Copyright (c) 1993,1994 by Borland International Inc.
;       All Rights Reserved.
;
;
;       Demo program to detect Critical errors under DPMI.
;
;
;       Unfortunately DPMI is not really clear about when and how these
;       interrupts are reflected for protected vs. real mode, so to be on
;       the safe side, we hook interrupt 24h in both modes.
;
;       Per the DPMI spec the client can not terminate from the
;       Critical Error handler.  Experience also shows that also bad
;       things can happen if clients attempt to terminate from real mode.
;       In addition, the context of the client is unknown from the handlers.
;
;
;       Based on all of the above the only thing one can do is to 
;       install a handler in both modes and set a global flag if the event
;       hits, and then check it frequently from the main program.
;
;
;       The functions HookCritHandler() and UnHookCritHandler()
;       will install the handlers in the appropriate modes, and
;       the public critErr variable should be checked.
;       It is safe to call HookCritHandler() and/or UnHookCritHandler
;       multiple times.
;
;       The segments _REAL_DATA and _REAL_TEXT should be marked as
;       PRELOAD FIXED REAL in the .def file.
;
;

_REAL_DATA      SEGMENT PARA PUBLIC 'REALDATA'

		PUBLIC  _critErr
_critErr        DW      0


oldRCritErrHandler      DD      0
oldCritErrHandler       DD      0

codeSegPara     DW      _REAL_TEXT      ;; para address of real CS

_REAL_DATA      ENDS


;;
;;      The real mode codeseg with the simple handlers
;;

_REAL_TEXT      SEGMENT BYTE PUBLIC 'REALCODE'

		ASSUME  DS:_REAL_DATA
		ASSUME  CS:_REAL_TEXT


RCritErrHandler PROC
		push    ax
		push    ds
		mov     ax, _REAL_DATA          ;; paragraph fixup
		mov     ds, ax
		mov     _critErr, 1
		pop     ds
		pop     ax
		iret
RCritErrHandler ENDP


_REAL_TEXT      ENDS


;;
;;      The prot mode code
;;


_TEXT           SEGMENT BYTE PUBLIC 'CODE'

		ASSUME  DS:_REAL_DATA
		ASSUME  CS:_TEXT

CritErrHandler  PROC
		push    ax
		push    ds
		mov     ax, _REAL_DATA          ;; selector fixup
		mov     ds, ax
		mov     _critErr, 1
		pop     ds
		pop     ax
		iret
CritErrHandler  ENDP



		PUBLIC  _HookCritHandlers
_HookCritHandlers PROC  FAR
		push    ds
		mov     ax, _REAL_DATA          ;; selector fixup
		mov     ds, ax

		cmp     word ptr oldCritErrHandler+2, 0
		jne     @@installed
		
		;; get old vectors

		mov     bx, 024h
		mov     ax, 0204h       ;; get PM vector
		int     31h
		mov     word ptr oldCritErrHandler+2, cx
		mov     word ptr oldCritErrHandler+0, dx

		mov     bx, 024h
		mov     ax, 0200h       ;; get RM vector
		int     31h
		mov     word ptr oldRCritErrHandler+2, cx
		mov     word ptr oldRCritErrHandler+0, dx

		;; set new vectors

		mov     bx, 024h
		mov     ax, 0205h       ;; set PM vector
		mov     cx, cs
		mov     dx, OFFSET CritErrHandler
		int     31h

		mov     bx, 024h
		mov     ax, 0201h       ;; get RM vector
		mov     cx, codeSegPara
		mov     dx, OFFSET _REAL_TEXT:RCritErrHandler
		int     31h
@@installed:
		pop     ds
		ret

_HookCritHandlers ENDP



		PUBLIC  _UnhookCritHandlers
_UnhookCritHandlers PROC FAR
		push    ds
		mov     ax, _REAL_DATA          ;; selector fixup
		mov     ds, ax

		cmp     word ptr oldCritErrHandler+2, 0
		je      @@notinstalled
		
		;; restore old vectors

		mov     bx, 024h
		mov     ax, 0205h       ;; set PM vector
		mov     cx, word ptr oldCritErrHandler+2
		mov     dx, word ptr oldCritErrHandler+0
		int     31h

		mov     bx, 024h
		mov     ax, 0201h       ;; set RM vector
		mov     cx, word ptr oldRCritErrHandler+2
		mov     dx, word ptr oldRCritErrHandler+0
		int     31h

@@notinstalled:
		pop     ds
		ret

_UnhookCritHandlers ENDP




_TEXT           ENDS

		END
