/* ------------------------------------------------------------------------*/
/*                                                                         */
/*   HARDWARE.H                                                            */
/*                                                                         */
/*   defines the class THardwareInfo                                       */
/*                                                                         */
/* ------------------------------------------------------------------------*/
/*
 *      Turbo Vision - Version 2.0
 *
 *      Copyright (c) 1994 by Borland International
 *      All Rights Reserved.
 *
 */

#pragma option -Vo-
#if defined( __BCOPT__ ) && !defined (__FLAT__)
#pragma option -po-
#endif

#if defined( Uses_THardwareInfo ) && !defined( __THardwareInfo )
#define __THardwareInfo

#if defined( __FLAT__ )

#if !defined( __WINDOWS_H )
#include <windows.h>
#endif

#else

#if !defined( MAKELONG )
#define MAKELONG(h,l) \
    ((long)(((unsigned)(l)) | (((long)((unsigned)(h))) << 16)))
#endif

#endif

class TEvent;
class MouseEventType;

class THardwareInfo
{

public:

    THardwareInfo();

    static ulong getTickCount();

#if defined( __FLAT__ )

    enum ConsoleType { cnInput = 0, cnOutput = 1 };
    enum PlatformType { plDPMI32 = 1, plWinNT = 2, plOS2 = 4 };

    static PlatformType getPlatform();

// Caret functions.

    static void setCaretSize( ushort size );
    static ushort getCaretSize();
    static void setCaretPosition( ushort x, ushort y );
    static BOOL isCaretVisible();

// Screen functions.

    static ushort getScreenRows();
    static ushort getScreenCols();
    static ushort getScreenMode();
    static void setScreenMode( ushort mode );
    static void clearScreen( ushort w, ushort h );
    static void screenWrite( ushort x, ushort y, ushort *buf, DWORD len );
    static ushort *allocateScreenBuffer();
    static void freeScreenBuffer( ushort *buffer );

// Mouse functions.

    static DWORD getButtonCount();
    static void cursorOn();
    static void cursorOff();

// Event functions.

    static BOOL getMouseEvent( MouseEventType& event );
    static BOOL getKeyEvent( TEvent& event );
    static void clearPendingEvent();

// System functions.

    static BOOL setCtrlBrkHandler( BOOL install );
    static BOOL setCritErrorHandler( BOOL install );

private:

    static BOOL __stdcall ctrlBreakHandler( DWORD dwCtrlType );

    static BOOL insertState;
    static PlatformType platform;
    static HANDLE consoleHandle[2];
    static DWORD consoleMode;
    static DWORD pendingEvent;
    static INPUT_RECORD irBuffer;
    static CONSOLE_CURSOR_INFO crInfo;
    static CONSOLE_SCREEN_BUFFER_INFO sbInfo;

#else

    static ushort *getColorAddr( ushort offset = 0 );
    static ushort *getMonoAddr( ushort offset = 0 );
    static uchar getShiftState();
    static uchar getBiosScreenRows();
    static uchar getBiosVideoInfo();
    static void setBiosVideoInfo( uchar info );
    static ushort getBiosEquipmentFlag();
    static ushort huge getBiosEquipmentFlag(int);   // Non-inline version.
    static void setBiosEquipmentFlag( ushort flag );
    static Boolean getDPMIFlag();

private:

    static ushort huge getBiosSelector();   // For SYSINT.ASM.

    static Boolean dpmiFlag;
    static ushort colorSel;
    static ushort monoSel;
    static ushort biosSel;

#endif

};

#if defined( __FLAT__ )

inline THardwareInfo::PlatformType THardwareInfo::getPlatform()
{
    return platform;
}

// Caret functions.

inline ushort THardwareInfo::getCaretSize()
{
    return crInfo.dwSize;
}


inline BOOL THardwareInfo::isCaretVisible()
{
    return crInfo.bVisible;
}


// Screen functions.

inline ushort THardwareInfo::getScreenRows()
{
    return sbInfo.dwSize.Y;
}

inline ushort THardwareInfo::getScreenCols()
{
    return sbInfo.dwSize.X;
}

#pragma option -w-inl
inline void THardwareInfo::clearScreen( ushort w, ushort h )
{
    COORD coord = { 0, 0 };
    DWORD read;

    FillConsoleOutputAttribute( consoleHandle[cnOutput], 0x07, w*h, coord, &read );
    FillConsoleOutputCharacterA( consoleHandle[cnOutput], ' ', w*h, coord, &read );
}
#pragma option -w+inl

inline ushort *THardwareInfo::allocateScreenBuffer()
{
    short x = sbInfo.dwSize.X, y = sbInfo.dwSize.Y;

    if( x < 80 )        // Make sure we allocate at least enough for
        x = 80;         //   a 80x50 screen.
    if( y < 50 )
        y = 50;

    return (ushort *) VirtualAlloc( 0, x * y * 4, MEM_COMMIT, PAGE_READWRITE );
}

inline void THardwareInfo::freeScreenBuffer( ushort *buffer )
{
    VirtualFree( buffer, 0, MEM_RELEASE );
}


// Mouse functions.

inline DWORD THardwareInfo::getButtonCount()
{
    DWORD num;
    GetNumberOfConsoleMouseButtons(&num);
    return num;
}

inline void THardwareInfo::cursorOn()
{
    SetConsoleMode( consoleHandle[cnInput], consoleMode | ENABLE_MOUSE_INPUT );
}

inline void THardwareInfo::cursorOff()
{
    SetConsoleMode( consoleHandle[cnInput], consoleMode & ~ENABLE_MOUSE_INPUT );
}


// Event functions.

inline void THardwareInfo::clearPendingEvent()
{
    pendingEvent = 0;
}


// System functions.

inline BOOL THardwareInfo::setCtrlBrkHandler( BOOL install )
{
    return SetConsoleCtrlHandler( &THardwareInfo::ctrlBreakHandler, install );
}

inline BOOL THardwareInfo::setCritErrorHandler( BOOL install )
{
    return TRUE;        // Handled by NT or DPMI32..
}


#else

inline ushort *THardwareInfo::getColorAddr( ushort offset )
    { return (ushort *) MAKELONG( colorSel, offset ); }

inline ushort *THardwareInfo::getMonoAddr( ushort offset )
    { return (ushort *) MAKELONG( monoSel, offset ); }

inline ulong THardwareInfo::getTickCount()
    { return *(ulong *) MAKELONG( biosSel, 0x6C ); }

inline uchar THardwareInfo::getShiftState()
    { return *(uchar *) MAKELONG( biosSel, 0x17 ); }

inline uchar THardwareInfo::getBiosScreenRows()
    { return *(uchar *) MAKELONG( biosSel, 0x84 ); }

inline uchar THardwareInfo::getBiosVideoInfo()
    { return *(uchar *) MAKELONG( biosSel, 0x87 ); }

inline void THardwareInfo::setBiosVideoInfo( uchar info )
    { *(uchar *) MAKELONG( biosSel, 0x87 ) = info; }

inline ushort THardwareInfo::getBiosEquipmentFlag()
    { return *(ushort *) MAKELONG( biosSel, 0x10 ); }

inline void THardwareInfo::setBiosEquipmentFlag( ushort flag )
    { *(ushort *) MAKELONG( biosSel, 0x10 ) = flag; }

inline Boolean THardwareInfo::getDPMIFlag()
    { return dpmiFlag; }

#endif

#endif  // __THardwareInfo

#pragma option -Vo.
#if defined( __BCOPT__ ) && !defined (__FLAT__)
#pragma option -po.
#endif
