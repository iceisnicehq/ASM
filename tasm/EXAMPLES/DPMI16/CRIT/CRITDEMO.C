/*
	Copyright (c) 1993,1994 by Borland International Inc.
	All Rights Reserved.
*/

#include <conio.h>
#include <stdio.h>
#include <process.h>

void HookCritHandlers(void);
void UnhookCritHandlers(void);

extern int far critErr;




FILE * c_fopen(const char * __path, const char * __mode)
	{
	FILE * f;
	int retry;

	critErr = 0;


	do  {
		f = fopen (__path, __mode);

		if (f==NULL)
			{
			int ch = 0;

			printf("Critical Error: (A)bort, (R)etry, (F)ail ");
			while (!ch)
				{
				switch (ch = getch())
					{
					case 'r':
					case 'R':
						retry = 1;
						break;

					case 'A':
					case 'a':
						exit(-1);

					case 'F':
					case 'f':
						retry = 0;
						break;

					default:
						ch = 0;
					}
				}
			printf ("%c\n", ch);

			}

		} while ( (f==NULL) && retry);


	return f;

	}



int main(int argc, char ** argv)
	{
	FILE * fh;

	if (argc==1)
		return 0;

	HookCritHandlers();

	fh= c_fopen(argv[1], "rb");

	if (fh)
		{
		printf ("Open successful\n");
		fclose(fh);
		}
	else
		{
		printf ("Open failed\n");
		}

	UnhookCritHandlers();

	return 0;

	}
