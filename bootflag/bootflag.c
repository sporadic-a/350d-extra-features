#define LEDRED			(*((int*)0xC02200A0))
#define LEDBLUE			(*((int*)0xC0220000))
#define LEDON			0x46
#define LEDOFF			0x44

#define FW_ADDRESS      0xFF810000
#define BOOT_FLAG		0xf8000000

#define BOOTLOADER_FROM	0xffff25f4
#define BOOTLOADER_TO	0x00100000
#define BOOTLOADER_SIZE	0x3300
#define FLAG_BUF_SIZE	(0x80 / sizeof(int))

typedef void (*ft_read_bootflag)(void* buf1, void* buf2);
typedef void (*ft_write_bootflag)(void* buf1, void* buf2);

void Delay(int i) {
	while(--i) {
		int j = (1 << 8);
		while (--j) {
			asm("NOP");asm("NOP");asm("NOP");asm("NOP");
		}
	}
}

// copy bootloader to 0x00100000
void Copy() {
	int *from = (int*)BOOTLOADER_FROM;
	int *to = (int*)BOOTLOADER_TO;
	int i = 0;
	for (; i < BOOTLOADER_SIZE; i++, from++, to++)
		*to = *from;
}

// zero memory
void Zero(int* buf, int size) {
	int i = 0;
	for (; i < size; i++)
		buf = 0;
}

/* *, -1,  * run(1) from cf
   *,  0,  0 run(1) from cf
  -1,  0, -1 run(5) from flash

original flag in camera: 0,0,-1
*/

//fopen 0xffa41664

void main()
{
	ft_read_bootflag read_bootflag;
	ft_write_bootflag write_bootflag;
	int buf1[FLAG_BUF_SIZE];
	int buf2[FLAG_BUF_SIZE];
	int *boot_flag = (int*)BOOT_FLAG;

	Copy();
	read_bootflag = (ft_read_bootflag)0x001059b0;
	write_bootflag = (ft_write_bootflag)0x0010586c;
	Delay(1 << 14);

	Zero(buf1, FLAG_BUF_SIZE);
	Zero(buf2, FLAG_BUF_SIZE);
	read_bootflag(buf1, buf2);

	//if (buf1[0] == 0 && buf1[1] == 0 && buf1[2] == -1)
	if (buf1[0] != boot_flag[0] || buf1[1] != boot_flag[1] || buf1[2] != boot_flag[2]) {
		LEDRED = LEDON;
	} else {
		LEDBLUE = LEDON;
		Delay(1 << 14);

		LEDBLUE = LEDON;
		LEDRED = LEDON;
		// enable / disable boot from cf card
		if (buf1[1] == 0)
			buf1[1] = -1;
		else
			buf1[1] = 0;
		// write to flash
		write_bootflag(buf1, buf2);
		Delay(1 << 14);
		LEDBLUE = LEDOFF;
		LEDRED = LEDOFF;
		Delay(1 << 14);

		// verify, blue = ok
		//if (boot_flag[0] == 0 && boot_flag[1] == -1 && boot_flag[2] == -1)
		if (buf1[0] == boot_flag[0] && buf1[1] == boot_flag[1] && buf1[2] == boot_flag[2])
			LEDBLUE = LEDON;
		else
			LEDRED = LEDON;
	}

	for (;;);
}
