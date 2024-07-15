/*-------------------------------------------------------------*/
/*                                                             */
/*   Turbo Vision Demo                                         */
/*                                                             */
/*   BGI support file for use with Turbo Vision programs. See  */
/*   TVBGI.CPP for an example of how to use this file.         */
/*                                                             */
/*-------------------------------------------------------------*/
/*
 *      Turbo Vision - Version 2.0
 *
 *      Copyright (c) 1994 by Borland International
 *      All Rights Reserved.
 *
 */

#if !defined( __GRAPHAPP_H )
#define __GRAPHAPP_H

#include <tvision\tv.h>

#if !defined( __GRAPHICS_H )
#include <graphics.h>
#endif  // __GRAPHICS_H

typedef void (*DriverPtrFunction)();

Boolean graphActive = False;
DriverPtrFunction driverPtr = 0;
unsigned int driverSize = 0;
char  emptyString[] = "";
char *bgiPath = "e:\\bc4\\bgi";
int driver = DETECT;
int mode = 0;

const MAXSIZE = 80;
const int lastDriver = 10;
char driverName[lastDriver][10] =
    {"CGA",                //  1. CGA
     "CGA",                //  2. MCGA
     "EGAVGA",             //  3. EGA
     "EGAVGA",             //  4. EGA64
     "EGAVGA",             //  5. EGAMONO
     "IBM8514",            //  6. IBM8514
     "HERC",               //  7. HercMono
     "ATT",                //  8. ATT400
     "EGAVGA",             //  9. VGA
     "PC3270"};            // 10. PC3270

Boolean graphAppInit( int, int, char * );
void graphAppDone(void);
Boolean graphicsStart(void);
void graphicsStop(void);

#endif  // __GRAPHAPP_H
