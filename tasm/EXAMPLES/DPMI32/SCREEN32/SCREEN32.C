//
//  Copyright (c) 1993,1994 by Borland International Inc.
//  All Rights Reserved.
//
//
//   This program demonstrates how to access the screen, handle keyboard and
//  mouse events under DPMI32.
//
//   This is by far the best way to write to the text screen and to handle
//  key and mouse events -- even for programs intended only for DOS -- for
//  the following reasons:
//
//    - using this method will work under Win32 as well as DOS.
//    - 32rtm will take care of finding the screen address (including
//      screen pages, and mono screen)
//    - 32rtm will take care of and optimize the mouse hide/show under the
//      modified area
//    - It is not necessary to write hundreds of lines of assembly code
//      to have a mouse or int 9 handler. Note that under DPMI the mouse
//      and the keyboard handler and any data it touches would also need
//      to be locked.
//
//  For a full reference of the Console APIs see the Windows API
//  help file and the PowerPack documentation for DOS compatibility
//  issues.
//
//

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

//
//  Function setupScreen
//
//  This function clears the screen and sets it up for our simple
//  text reader
//

void setupScreen (HANDLE hConsole, COORD size)
    {
    COORD coord  = { 0, 0};
    DWORD read;

    FillConsoleOutputAttribute (hConsole, 0x1F,size.X*size.Y,coord, &read);
    FillConsoleOutputCharacterA(hConsole, ' ' ,size.X*size.Y,coord, &read);

    FillConsoleOutputAttribute (hConsole, 0x70,size.X,coord, &read);
    FillConsoleOutputCharacterA(hConsole, ' ' ,size.X,coord, &read);

    WriteConsoleOutputCharacter(hConsole, " Exit Up Down PgUp PgDn",24,coord,&read);
    coord.X= 2;
    FillConsoleOutputAttribute(hConsole, 0x74,1,coord,&read);
    coord.X= 6;
    FillConsoleOutputAttribute(hConsole, 0x74,1,coord,&read);
    coord.X= 9;
    FillConsoleOutputAttribute(hConsole, 0x74,1,coord,&read);
    }


//
//  Function drawText
//
//  This function puts the text up the screen from a given line
//

int drawText(char * text, int tsize, HANDLE hConsole, COORD scrSize, int line)
    {
    int i;
    char * p;

    if (line<0) line = 0;

    // skip the first 'line' lines

    for (i=0, p=text;i<line && p < text+tsize;i++)
	{
	while (*p!=13 && p < text+tsize) p++;
	p++;
	}
    line = i;

    // write the next scrSize.Y lines

    for (i=1;i<scrSize.Y;i++)
	{
	int j=0;
	COORD coord  = { 0, 0};
	DWORD read;

	if (p < text+tsize)
	    {
	    if (*p==10) p++;
	    for (j=0;j < scrSize.X && p+j<text+tsize && p[j]!=10 && p[j]!=13; j++);
	    }

	coord.Y = (WORD) i;

	// write line

	FillConsoleOutputCharacterA(hConsole, ' ' ,scrSize.X,coord, &read);
	if (j) WriteConsoleOutputCharacter(hConsole, p,j,coord,&read);

	while (*p!=13 && p < text+tsize) p++;
	p++;

	}

    return line;

    }

//
//  Function main
//
// the main function with the key/mouse event loop
//

int main(int argc, char ** argv)
    {
    HANDLE  hOut;
    HANDLE  hIn;
    CONSOLE_SCREEN_BUFFER_INFO b;
    CONSOLE_CURSOR_INFO ci;
    COORD coord;
    DWORD cmode;
    DWORD line = 0;
    DWORD lineNow = -1;
    CONSOLE_CURSOR_INFO ci2;
    void * s;
    FILE * f;
    char * text;
    int tsize;
    BOOL exit = FALSE;

    // open and read the file to view

    if (argc!=2)
	{
	printf("usage: screen32 filename\n");
	return -1;
	}

    if (NULL == (f = fopen(argv[1], "rb")))
	{
	fprintf(stderr, "Can't open '%s'\n", argv[1]);
	return -1;
	}

    fseek(f, 0, SEEK_END);
    tsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    if (NULL == (text = malloc(tsize)))
	{
	fclose(f);
	fprintf(stderr, "Out of memory\n");
	return -1;
	}

    fread(text, tsize, 1, f);
    fclose(f);

    // open the console for reading/writing

    hIn  = CreateFile("CONIN$" ,GENERIC_READ+GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if (hIn==INVALID_HANDLE_VALUE)
		{
	fprintf(stderr, "Can't open CONIN$\n");
	return -1;
		}

    hOut = CreateFile("CONOUT$",GENERIC_READ+GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if (hOut==INVALID_HANDLE_VALUE)
		{
		CloseHandle(hIn);
	fprintf(stderr, "Can't open CONOUT$\n");
	return -1;
		}

    //  save the user screen, settings and cursor

    GetConsoleScreenBufferInfo(hOut, &b);
    GetConsoleCursorInfo(hOut, &ci);
    GetConsoleMode(hIn, &cmode);

    s = malloc(b.dwSize.X*b.dwSize.Y*4);

    if (s)
	{
	SMALL_RECT r = { 0,0,20000,20000};
	coord.X= 0;coord.Y =0;
	ReadConsoleOutput(hOut, s, b.dwSize, coord, &r);
	}
    else
	{
	fprintf(stderr, "Out of memory\n");
	free(text);
	return -1;
	}

    // turn on mouse support. Also shows the mouse under DPMI32

    SetConsoleMode(hIn, cmode|ENABLE_MOUSE_INPUT);

    // hide the text cursor

    GetConsoleCursorInfo(hOut, &ci2);
    ci2.bVisible = FALSE;
    SetConsoleCursorInfo(hOut, &ci2);

    // clear screen

    setupScreen(hOut, b.dwSize);

    // main loop.

    while ( !exit)
	{
	INPUT_RECORD    inpRec;
	DWORD read;

	// draw text if changed

	if (lineNow != line)
	    {
	    line = drawText(text, tsize,  hOut, b.dwSize, line);
	    lineNow = line;
	    }

	// get keyboard/mouse event

	ReadConsoleInputA(hIn,&inpRec,1,&read);

	switch(inpRec.EventType)
	  {
	  case KEY_EVENT:

	    // note: only accept key down events!

	    if (inpRec.Event.KeyEvent.bKeyDown)
		{
		// unlike in DOS, AsciiChar is defined for Alt-key
		// combinations too. This way here we will accept
		// x, Shift-X, Alt-x, Alt-X. For Ctrl-X the ascii
		// code would be 0x1A.

		switch (inpRec.Event.KeyEvent.uChar.AsciiChar)
		    {
		    case 'x':
		    case 'X':
			exit = TRUE;
			break;
		    case 'D':
		    case 'd':
			line++;
			break;
		    case 'U':
		    case 'u':
			line--;
			break;

		    case 0:
			switch (inpRec.Event.KeyEvent.wVirtualKeyCode)
			    {
			    case VK_UP:
				line--;
				break;
			    case VK_DOWN:
				line++;
				break;
			    case VK_PRIOR:  //page up
				line-=b.dwSize.Y-2;
				break;
			    case VK_NEXT:   //page down
				line+=b.dwSize.Y-2;
				break;
			    case VK_HOME:
				line=0;
				break;
			    case VK_END:
				line=0x7FFFFFFF;
				break;
			    }
		    }
		}
	    break;

	  case MOUSE_EVENT:

	    // note: accept both single and double clicks

	    if (inpRec.Event.MouseEvent.dwEventFlags != MOUSE_MOVED &&
		inpRec.Event.MouseEvent.dwButtonState & 1 &&
		inpRec.Event.MouseEvent.dwMousePosition.Y == 0 )
		{
		int x = inpRec.Event.MouseEvent.dwMousePosition.X;

		if (x>=1 && x <=4)
		    exit = TRUE;
		else if (x>=6 && x <=7)
		    line--;
		else if (x>=9 && x <=12)
		    line++;
		else if (x>=14 && x <=17)
		    line-=b.dwSize.Y-2;
		else if (x>=19 && x <=22)
		    line+=b.dwSize.Y-2;
		}
	    break;
	  }
	}

    // restore user screen and settings and cursor

    SetConsoleMode(hIn, cmode);
    SetConsoleCursorInfo(hOut, &ci);

    if (s)
	{
	SMALL_RECT r = { 0,0,20000,20000};
	coord.X= 0;coord.Y =0;
	WriteConsoleOutput(hOut, s, b.dwSize, coord, &r);
	}

	// clean up

	CloseHandle(hIn);
	CloseHandle(hOut);
    free(text);
    free(s);

    return 0;
    }
