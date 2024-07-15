/*
 *
 *      Copyright (c) 1994 by Borland International
 *      All Rights Reserved.
 *
 */

#include <dos.h>

#if     defined(__DPMI32__)

extern void copyToLow(unsigned short dst_seg, unsigned short dst_offset, void * src, unsigned int size);
extern void copyFromLow(void * dst, unsigned short src_seg, unsigned short src_offset, unsigned int size);

extern unsigned long  getLowMemDword(unsigned short seg, unsigned short offset);
extern unsigned short getLowMemWord(unsigned short seg, unsigned short offset);
extern unsigned char  getLowMemByte(unsigned short seg, unsigned short offset);

extern void setLowMemDword(unsigned short seg, unsigned short offset, unsigned long dw);
extern void setLowMemWord (unsigned short seg, unsigned short offset, unsigned short w);
extern void setLowMemByte (unsigned short seg, unsigned short offset, unsigned char  c);

#else

#if defined(__DPMI16__)

extern unsigned short getSelFor(unsigned short seg);

#else

#define getSelFor(seg) (seg)

#endif

#define setLowMemByte(seg, offset, b) (*((unsigned char far * ) MK_FP(getSelFor(seg),offset)) = b)
#define setLowMemWord(seg, offset, w) (*((unsigned short far *) MK_FP(getSelFor(seg),offset)) = w)
#define setLowMemDword(seg, offset, d)(*((unsigned long far * ) MK_FP(getSelFor(seg),offset)) = d)

#define getLowMemByte(seg, offset) (*((unsigned char far * ) MK_FP(getSelFor(seg),offset)))
#define getLowMemWord(seg, offset) (*((unsigned short far *) MK_FP(getSelFor(seg),offset)))
#define getLowMemDword(seg, offset)(*((unsigned long far * ) MK_FP(getSelFor(seg),offset)))

#define copyToLow(dst_seg,dst_offset,src,size)   _fmemcpy(MK_FP(getSelFor(dst_seg),dst_offset), src, size)
#define copyFromLow(dst,src_seg,src_offset,size) _fmemcpy(dst, MK_FP(getSelFor(src_seg),src_offset), size)

#endif

