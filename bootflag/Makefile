scanled: scanled.c entry.s
	arm-elf-gcc -fno-builtin -O2 -Ilib -nostdinc -c entry.s
	arm-elf-gcc -fno-builtin -O1 -Ilib -c scanled.c
	arm-elf-gcc -fno-builtin -O2 -Ilib -nostdlib -Wl,-N,-Ttext,1900 -o scanled.exec entry.o scanled.o -lgcc
	arm-elf-objcopy -O binary scanled.exec scanled.bin

autoexec: autoexec.c entry.s
	arm-elf-gcc -fno-builtin -O2 -Ilib -nostdinc -c entry.s
	arm-elf-gcc -fno-builtin -O1 -Ilib -c autoexec.c
	arm-elf-gcc -fno-builtin -O2 -Ilib -nostdlib -Wl,-N,-Ttext,1900 -o autoexec.exec entry.o autoexec.o -lgcc
	arm-elf-objcopy -O binary autoexec.exec autoexec.bin

firfile: scanled
	decrypt20D scanled.bin scanled.fir
	
all: firfile autoexec

clean:
	rm *.o
	rm *.exec
