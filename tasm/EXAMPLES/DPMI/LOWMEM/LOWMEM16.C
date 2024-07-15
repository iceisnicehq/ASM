/*
 *
 *      Copyright (c) 1994 by Borland International
 *      All Rights Reserved.
 *
 */

#include "lowmem.h"

static unsigned int dosSel = 0;
static unsigned int dosSeg = 0;


/*
	Function getSelFor()

	Converts a selector to a segment address. The selector is valid until the
	next call to getSelFor().
*/


unsigned short getSelFor(unsigned short dos_seg)
	{
	switch (dos_seg)
		{
		case 0x0000:
			return __Seg0000;
		case 0x0040:
			return __Seg0040;
		case 0xA000:
			return __SegA000;
		case 0xB000:
			return __SegB000;
		case 0xB800:
			return __SegB800;
		case 0xC000:
			return __SegC000;
		case 0xD000:
			return __SegD000;
		case 0xE000:
			return __SegE000;
		case 0xF000:
			return __SegF000;
		default:

			if (dosSeg != dos_seg)
				{
				asm {
					mov  dx, dos_seg
					xor  cx, dx
					shl  dx, 4
					shr  cx, 12
					mov  bx, dosSel
					mov  ax, 0007h          // DPMI Set Segment Base Address
					int  31h
					}
				dosSeg = dos_seg;
				}
			return dosSel;
		}
	}


/*
	Function getSel()
	Function freeSel()

	The startup and cleanup functions will get a selector from DPMI to be
	used for low memory access requests with segments that are not mapped
	to selectors by the operating system (RTM/Windows). The freeSel function
	will free the selector at termination time.
*/

static void getSel(void)
	{
	asm
		{
		mov  cx, 1
		mov  ax, 0000h          // DPMI Allocate Selector
		int  31h
		mov  dosSel, ax
		mov  bx, ax
		mov  dx, 0FFFFh         // 64K limit
		xor  cx, cx
		mov  ax, 0008h          // DPMI Set Segment Limit
		int  31h
		}
	}

static void freeSel(void)
	{
	asm
		{
		mov     bx, dosSel
		mov     ax, 0001h                       // DPMI Free Selector
		int     31h
		}
	}
#pragma startup getSel
#pragma exit freeSel
