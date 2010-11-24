entry:
	arm-elf-gcc -fno-builtin -O2 -Ilib -nostdinc -c entry.s

bootflg2: bootflg2.c entry
	arm-elf-gcc -fno-builtin -O1 -Ilib -c bootflg2.c
	arm-elf-gcc -fno-builtin -O2 -Ilib -nostdlib -Wl,-N,-Ttext,1900 -o bootflg2.exec entry.o bootflg2.o -lgcc
	arm-elf-objcopy -O binary bootflg2.exec bootflg2.bin

autoexec: autoexec.c entry
	arm-elf-gcc -fno-builtin -O1 -Ilib -c autoexec.c
	arm-elf-gcc -fno-builtin -O2 -Ilib -nostdlib -Wl,-N,-Ttext,1900 -o autoexec.exec entry.o autoexec.o -lgcc
	arm-elf-objcopy -O binary autoexec.exec autoexec.bin

firfile: bootflg2
	decrypt20D bootflg2.bin bootflg2.fir
	
all: firfile autoexec
