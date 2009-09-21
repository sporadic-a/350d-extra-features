entry:
	arm-elf-gcc -fno-builtin -O2 -Ilib -nostdinc -c entry.s

bootflag: bootflag.c entry
	arm-elf-gcc -fno-builtin -O1 -Ilib -c bootflag.c
	arm-elf-gcc -fno-builtin -O2 -Ilib -nostdlib -Wl,-N,-Ttext,1900 -o bootflag.exec entry.o bootflag.o -lgcc
	arm-elf-objcopy -O binary bootflag.exec bootflag.bin

autoexec: autoexec.c entry
	arm-elf-gcc -fno-builtin -O1 -Ilib -c autoexec.c
	arm-elf-gcc -fno-builtin -O2 -Ilib -nostdlib -Wl,-N,-Ttext,1900 -o autoexec.exec entry.o autoexec.o -lgcc
	arm-elf-objcopy -O binary autoexec.exec autoexec.bin

firfile: bootflag
	decrypt20D bootflag.bin bootflag.fir
	
all: firfile autoexec

clean:
	rm *.o
	rm *.exec
