#!/bin/sh
#export PATH=/nonraid/public/multimedia/others/canon_chdk/gcc_chdk/arm-elf/bin:/nonraid/public/multimedia/others/canon_chdk/gcc_chdk/arm-elf:$PATH
export PATH=/:/bin:/arm-elf/bin:/arm-elf:$PATH
rm *.o
rm *.elf
rm AUTOEXEC.BIN
make

