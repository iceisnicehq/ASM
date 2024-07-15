/*
 *
 *      Copyright (c) 1994 by Borland International
 *      All Rights Reserved.
 *
 */

#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <mem.h>
#include "lowmem.h"



int main(void)
	{
	int xSize=getLowMemWord(0x40, 0x4A);
	int ySize=getLowMemByte(0x40, 0x84)+1;
	int i,j;

	void * savedScreen = malloc(xSize*ySize*2);
	if (savedScreen==NULL)
		{
		printf("Out of memory\n");
		return 1;
		}

	copyFromLow(savedScreen, 0xb800, 0, xSize*ySize*2);

	for (i=0;i<xSize;i++)
		for (j=0;j<ySize;j++)
			setLowMemWord(0xb800,(unsigned short)((j*xSize+i)*2),(unsigned short)(0x0700+j+'a'));

	getchar();

	copyToLow(0xb800, 0, savedScreen, xSize*ySize*2);

	return 0;
	}
