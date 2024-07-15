/*----------------------------------------------------------*/
/*                                                          */
/*   Copyright (c) 1991 by Borland International            */
/*                                                          */
/*----------------------------------------------------------*/
/*
 *      Turbo Vision - Version 2.0
 *
 *      Copyright (c) 1994 by Borland International
 *      All Rights Reserved.
 *
 */

#define Uses_TWindow
#define Uses_TApplication
#define Uses_TRect
#define Uses_TTerminal
#define Uses_MsgBox
#define Uses_otstream
#define Uses_TDeskTop
#define Uses_TScreen
#include <tvision\tv.h>

#include <string.h>
#include <stdlib.h>
#include <io.h>
#include <alloc.h>
#include <fcntl.h>
#include <strstrea.h>
#include <fstream.h>

class TTerminalWindow : public TWindow
{

public:

    TTerminalWindow( TRect bounds,
                     const char *winTitle,
                     ushort windowNo,
                     TTerminal *&interior,
                     ushort aBufSize
                   );
    TTerminal *makeInterior( TRect bounds, ushort aBufSize );
};

class TMyApp : public TApplication
{

public:

    TMyApp( int argc, char *argv[] );
    void showTerminalWindow( char *fileName );
};

void checkParamList( int argc, char *argv[] )
{
    if( argc != 2 )
        {
        TScreen::clearOnSuspend=False;
        cerr << "Syntax: TVTXTDEM <file to view>" << endl;
        exit(1);
        }

    if( access( argv[1], 4 ) != 0 ) // check for read permission
        {
        TScreen::clearOnSuspend=False;
        cerr << "Cannot open file (" << argv[1] << ")" << endl;
        exit(1);
        }
}

TTerminalWindow::TTerminalWindow( TRect bounds,
                                  const char *winTitle,
                                  ushort windowNo,
                                  TTerminal *&interior,
                                  ushort aBufSize
                                ) :
    TWindowInit( &TTerminalWindow::initFrame ),
    TWindow(bounds, winTitle, windowNo )
{
    interior = makeInterior( bounds, aBufSize );
    insert( interior );
}

TTerminal *TTerminalWindow::makeInterior( TRect bounds, ushort aBufSize )
{
    bounds = getExtent();
    bounds.grow( -1, -1 );
    return new TTerminal( bounds,
        0,			 // no horizontal scrollbar
        standardScrollBar( sbVertical | sbHandleKeyboard ),
        aBufSize );
};

TMyApp::TMyApp( int argc, char *argv[] ) :
    TProgInit( &TMyApp::initStatusLine,
               &TMyApp::initMenuBar,
               &TMyApp::initDeskTop
             )
{
    checkParamList( argc, argv );
    showTerminalWindow( argv[1] );
};

void TMyApp::showTerminalWindow( char *fileName )
{
    int handle = open( fileName, O_RDONLY );
    ushort buffSize = 8192;
    if( coreleft() - 1024 < buffSize )
        buffSize = ushort(coreleft() - 1024);

    if( filelength( handle ) > buffSize )
        {
        char buf[128];
        ostrstream os( buf, sizeof buf );
        os << "File is too big to fit in a TTerminal buffer. "
              "Only the first " << buffSize << " bytes of the file "
              "will be displayed." << ends;
        messageBox( buf, mfOKButton | mfWarning );
        }
    else
        buffSize = ushort(filelength( handle ));
    close( handle );

    // Initialize the terminal window object
    TTerminal *interior;
    TTerminalWindow *demo = new TTerminalWindow( TRect( 10, 1, 70, 18 ),
                                                 fileName,
                                                 wnNoNumber,
                                                 interior,
                                                 buffSize
                                               );
    deskTop->insert( demo );

    // Assign the TTerminal interior text device driver to a text "file"
    otstream os( interior );

    ifstream is( fileName );
    is.unsetf( ios::skipws );
    char st[128];

    while( is.getline( st, sizeof(st) ) && interior->canInsert((short)strlen(st)) )
        os << st << endl;

    interior->scrollTo(0, 0);
};

int main( int argc, char *argv[] )
{
    checkParamList( argc, argv );
    TMyApp *app = new TMyApp( argc, argv );
    app->run();
    TObject::destroy(app);
    return 0;
}
