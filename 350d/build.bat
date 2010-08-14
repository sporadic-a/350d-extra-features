@echo off

set path=..\..\;..\..\bin;..\..\arm-elf;..\..\arm-elf\bin;%PATH%

del *.o
del *.elf
del AUTOEXEC.BIN


arm-elf-gcc -c -o entry.o entry.S
arm-elf-gcc -c -o entry_subs.o entry_subs.S
arm-elf-gcc -c -o gui.o gui.S

arm-elf-gcc -nostdlib -march=armv5te -fno-builtin -c -o init.o init.c
arm-elf-gcc -nostdlib -march=armv5te -fno-builtin -c -o main.o main.c

arm-elf-gcc -nostdlib -march=armv5te -fno-builtin -Wl,-T,link.script -o AUTOEXEC.arm.elf entry.o entry_subs.o gui.o init.o main.o link.script

arm-elf-objcopy -O binary AUTOEXEC.arm.elf AUTOEXEC.BIN

