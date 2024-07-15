/*---------------------------------------------------------*/
/*                                                         */
/*   Turbo Vision BGI Support Demo Header File             */
/*                                                         */
/*---------------------------------------------------------*/
/*
 *      Turbo Vision - Version 2.0
 *
 *      Copyright (c) 1994 by Borland International
 *      All Rights Reserved.
 *
 */

#if !defined( __TVBGI_H )
#define __TVBGI_H

#define Uses_TApplication
#define Uses_TMenuBar
#define Uses_TStatusLine
#include <tvision\tv.h>

const
   MAXSIZE = 80,
   MAXPATH = 150;

char pathToDrivers[MAXPATH] = "\\bc4\\BGI";  // Default location of *.BGI files
const
   cmNewWin     = 1000,
   cmSetBGIPath = 1002,
   cmDoGraphics = 1003;

const
   hlChangeDir  = cmChangeDir,        // History list ID
   hlSetBGIPath = cmSetBGIPath;       // History list ID

class TBGIApp : public TApplication
{
public:

    TBGIApp();
    virtual ~TBGIApp();

    virtual void handleEvent( TEvent& Event );
    static TMenuBar *initMenuBar( TRect );
    static TStatusLine *initStatusLine( TRect );
    virtual void outOfMemory();
    void newWin();
    void doGraphics();
    void setBGIPath();
    void changeDir();
    void tile();
    void cascade();

    int appDriver;
    int appMode;
    char *bgiPath;
};

extern Boolean graphActive;
extern void    *driverPtr;
extern unsigned int driverSize;
extern char  emptyString[];
extern char *bgiPath;
extern int driver;
extern int mode;

extern Boolean graphAppInit( int, int, char * );
extern void graphAppDone();
extern Boolean graphicsStart();
extern void graphicsStop();

#endif  // __TVBGI_H
