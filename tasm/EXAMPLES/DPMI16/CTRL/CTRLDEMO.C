/*
	Copyright (c) 1993,1994 by Borland International Inc.
	All Rights Reserved.
*/

#include <stdio.h>

void HookCtrlHandlers(void);
void UnhookCtrlHandlers(void);

extern int far ctrlC;
extern int far ctrlBreak;



int main(void)
	{
	int i;
	int ret = 0;

	HookCtrlHandlers();

	for (i=0;i<10000;i++)
		{
		printf("%.5d ",i);
		if (ctrlBreak)
			{
			printf ("Ctrl-Break pressed, terminating\n");
			break;
			}
		if (ctrlC)
			{
			printf ("Ctrl-C pressed, terminating\n");
			break;
			}
		}




	UnhookCtrlHandlers();

	return ret;

	}
