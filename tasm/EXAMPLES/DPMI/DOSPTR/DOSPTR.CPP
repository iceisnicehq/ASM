/*
 *
 *      Copyright (c) 1994 by Borland International
 *      All Rights Reserved.
 *
 */

//
//  This example provides a template class DOSPtr (DOS Pointer) which
//  can be used to access any memory location below 1 Meg regardless
//  of the target platform (DOS Real mode, DOS 16bit DPMI, or
//  DOS 32bit DPMI)
//
//  The DOSPtr<T> template class creates an array of type T, starting
//  at the real mode memory segment passed to the constructor:
//
//     DOSPtr<long> vidmem(0xA000);
//
//  The appropriate assignment and cast operators are defined for
//  DOSPtr, so that the following operations can be performed on
//  the DOSPtr:
//
//  vidmem[0] = 0x12345678;
//  long l = vidmem[0];
//

//
// Important:  When built as a DPMI32 application, this method for accessing
//             the screen is not compatible with Win32.  Please see
//             ...\examples\dpmi32\screen32\screen32.c for a simpler, Win32
//             compatible screen access method.
//

#include <stdio.h>   // printf()
#include <stdlib.h>  // random()
#include <dos.h>     // geninterrupt()
#include <conio.h>   // getch(), gotoxy(), clrscr()

#if !defined( __DPMI32__) && !defined( __DPMI16__) && !defined( __LARGE__ )
#error DOSPtr requires large model, DPMI16, or DPMI32
#endif

// DOS_LValue is an intermediate class that is created and returned
// by the bracket operator of a DOSPtr object.
//
template <class T>
class DOS_LValue {
#if defined( __DPMI32__ )     // in 32 bit land, we need a selector/offset
  short selector;
  int   offset;
#else
  char *ptr;                  // in 16 bit, we can use a pointer
#endif
public:
  DOS_LValue( short aSelector, int aOffset ) {
#if defined( __DPMI32__ )
    selector = aSelector;
    offset = aOffset;
#else
    ptr = (char far *)MK_FP(aSelector,aOffset);
#endif
  }
  DOS_LValue<T>& operator =( T x );
  operator T();
};

//
// Assembly macros so bcc32 doesn't need to call tasm32 (for speed)
//
#define PUSH_ES      {__emit__ (0x06);}
#define PUSH_DS      {__emit__ (0x1E);}
#define POP_ES       {__emit__ (0x07);}
#define POP_DS       {__emit__ (0x1F);}
#define REP_MOVSB    {__emit__ (0xF3); __emit__ (0xA4); }

// In 16 bit (real or protected), the assignment operator
// simply casts the ptr member to the appropriate type,
// and assigns it the value passed.
//
// In 32 bit protected mode, use inline assembly to access
// the memory desired.
//
template <class T>
DOS_LValue<T>& DOS_LValue<T>::operator =( T x ) {
#if !defined( __DPMI32__ )
  ((T *)ptr)[0] = x;
  return *this;
#else
  PUSH_ES;
  _BX  = selector;
  _ES  = _BX;
  _EDI = offset;
  _ECX = 0;
  _CX  = sizeof(x);
  _ESI = (long)&x;
  REP_MOVSB;
  POP_ES;

  return *this;
#endif
}

// Cast operator works similar to the assignment.  In 16 bit
// real/protected mode, use a cast.  In 32 bit, use inline
// assembly to access the memory
//
template <class T>
DOS_LValue<T>::operator T() {
#if !defined( __DPMI32__)
  return ((T *)ptr)[0];
#else
  T retval;

  _ESI = offset;
  _EDI = (long)&retval;
  PUSH_DS
  PUSH_ES
  _ES = _DS;
  _BX  = selector;   // must use BX, since AX is used to access
                     // member variable when assigning to pseudo-register
  _DS = _BX;
  _ECX = 0;
  _CX = sizeof(T);
  REP_MOVSB;
  POP_ES;
  POP_DS;
  return retval;
#endif
}

// DOSPtr class
//
template <class T>
class DOSPtr {
  short    selector;
public:
  DOSPtr( short segment );
  DOS_LValue<T> operator[]( int offset );
};

// constructor allocates a selector when compiled in DPMI16 or
// DPMI32.  In large model, it simply uses the segment address
// as a 'selector'.
//
template <class T>
DOSPtr<T>::DOSPtr( short segment ) {

#if defined( __DPMI16__ ) || defined( __DPMI32__ )
  short temp;

  _AX = 0x02;            // call DPMI to get a selector
  _BX = segment;
  geninterrupt(0x31);
  temp = _AX;
  if (_FLAGS & 1)
    selector = 0;
  else
    selector = temp;
  return;
#else
  selector = segment;
#endif
}

// bracket operators returns a DOS_LValue<T> instance with
// a selector/offset pair pointing to the specified offset
// within the array
//
template <class T>
DOS_LValue<T> DOSPtr<T>::operator[]( int offset ) {
  return DOS_LValue<T>( selector, offset );
}

// Test program for DOSPtr.  It "fizzles" out the screen
// by decrementing each character until it is a space.
// when no more characters are left, it stops.
//

int main(void) {

  short x,y,done;
  DOSPtr<unsigned char> textmem(0xb800);

  done = 0;

  // fill the screen with characters

  clrscr();
  for (y=0;y<24;y++)
    for (x=0;x<80;x++)
      textmem[y*160+x*2]=(short)random(256);

  gotoxy(1,25);
  printf("Press any key to fizzle screen.");
  getch();

  while (!done) {
    done = 1;                                      // assume that we're done
    for (y=0;y<25;y++)
      for (x=0;x<80;x++)
        if (textmem[y*160+x*2]!=32) {              // if a character is found
          textmem[y*160+x*2]=(short)(textmem[y*160+x*2]-1); // decrement it
          done=0;                                  // and force another pass
        }
  }

  return 0;
}

