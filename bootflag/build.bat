@echo off

set path=..\..\;..\..\bin;..\..\arm-elf;..\..\arm-elf\bin;%PATH%

gmake all

arm-elf-objdump -d bootflg2.o >bootflg2.asm

del *.o
del *.exec
