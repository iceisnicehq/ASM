@echo off
rem
rem  To build examples in a target other than real mode,
rem  add the appropriate define as a parameter to this
rem  batch file.  For example
rem
rem     build -DDOS32
rem
rem  will build all the DOCDEMOS in 32 bit protected mode
rem

make -DSOURCE=tvguid01 %1 %2
make -DSOURCE=tvguid02 %1 %2
make -DSOURCE=tvguid03 %1 %2
make -DSOURCE=tvguid04 %1 %2
make -DSOURCE=tvguid05 %1 %2
make -DSOURCE=tvguid06 %1 %2
make -DSOURCE=tvguid07 %1 %2
make -DSOURCE=tvguid08 %1 %2
make -DSOURCE=tvguid09 %1 %2
make -DSOURCE=tvguid10 %1 %2
make -DSOURCE=tvguid11 %1 %2
make -DSOURCE=tvguid12 %1 %2
make -DSOURCE=tvguid13 %1 %2
make -DSOURCE=tvguid14 %1 %2
make -DSOURCE=tvguid15 %1 %2
make -DSOURCE=tvguid16 %1 %2
make -DSOURCE=tvguid17 %1 %2
make -DSOURCE=tvg17b   %1 %2
make -DSOURCE=tvguid18 %1 %2
make -DSOURCE=tvguid19 %1 %2
make -DSOURCE=tvguid20 -DBGI %1 %2
make -DSOURCE=tvguid21 -DBGI %1 %2
