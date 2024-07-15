
Turbo Vision Paradox Engine Demo

This example requires that you have the Paradox Engine installed.
The project file has the Paradox Engine directory hardcoded, and
you may need to make changes to the project options in order to
build this demo.

The project file contains two targets, one for DOS (Standard) and
one for DOS (16-bit DPMI).  Each target has slightly different
modifications that must be made, please read the following carefully:

====== DOS (Standard) target ======

First, select Options | Project | Directories, and modify the include
and lib paths to include the Paradox Engine directories:

	include: ..\..\..\INCLUDE;<PXE>\C\INCLUDE
	lib:     ..\..\..\LIB;<PXE>\C\LIB

Where <PXE> should be replaced by the drive and path of the root
directory for your Paradox Engine installation (ie, D:\PXENG30).

Second, you need to modify the node for the PXMSG.C file.  This file
is provided in the Paradox Engine installation.  Go to the project
window, and right click on the pxmsg.c node.  Then select "Edit Node
Attributes", and change the name to:

	<PXE>\C\pxmsg

Where <PXE> should be replaced by the drive and path of the root
directory for your Paradox Engine installation (ie, D:\PXENG30).

IMPORTANT: Do not include the .c file extension in the name field.

===== DOS (16-bit DPMI) target =====

First, select Options | Project | Directories, and modify the include
and lib paths to include the Paradox Engine directories:

	include: ..\..\..\INCLUDE;<PXE>\WINDOWS
	lib:     ..\..\..\LIB;<PXE>\WINDOWS

Where <PXE> should be replaced by the drive and path of the root
directory for your Paradox Engine installation (ie, D:\PXENG30).

Second, you need to modify the node for the PXMSG.C file.  This file
is provided in the Paradox Engine installation.  Go to the project
window, and right click on the pxmsg.c node.  Then select "Edit Node
Attributes", and change the name to:

	<PXE>\WINDOWS\pxmsg

Where <PXE> should be replaced by the drive and path of the root
directory for your Paradox Engine installation (ie, D:\PXENG30).

IMPORTANT: Do not include the .c file extension in the name field.

In order to run the DOS (16 bit DPMI) version of the demo, you
must have the Pardox Engine DLL somewhere in your path, or errors
will result at run-time.


