#define LEDRED			(*((int*)0xC02200A0))
#define LEDBLUE			(*((int*)0xC0220000))
#define LEDON			0x46
#define LEDOFF			0x44

#define FW_ADDRESS      0xFF810000
#define BOOT_FLAG	0xf8000000
#define VERSION1	0xff81044c
#define VERSION2	0xff9822ec

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
		buf[i] = 0;
}

/* *, -1,  * run(1) from cf
   *,  0,  0 run(1) from cf
  -1,  0, -1 run(5) from flash

original flag in camera: 0,0,-1
*/


void main()
{

	LEDBLUE = LEDOFF;
	LEDRED = LEDOFF;
	Delay(1 << 15);

	int *version1 = (int*)VERSION1;
	int *version2 = (int*)VERSION2;

	ft_read_bootflag read_bootflag;
	ft_write_bootflag write_bootflag;
	int buf1[FLAG_BUF_SIZE];
	int buf2[FLAG_BUF_SIZE];
	int *boot_flag = (int*)BOOT_FLAG;

	// if m = 0 there are no errors so far
	int m = 0;

	// check for key bytes at specific locations in v1.0.3
	if (version1[0] != 0x2f322e31 || version1[1] != 0x00000035) m = 1;
	if (version2[0] != 0x006e0061 || version2[1] != 0x006e006f) m = 1;

	// Boot flags at F8000000 must be 0,0,-1 or 0,-1,-1
	if (boot_flag[0] != 0 || boot_flag[2] != -1) m = 1;
	if (boot_flag[1] != 0 && boot_flag[1] != -1) m = 1;
	if (m != 0) goto stop;

	//flash blue light if version and initial flags are as expected
	LEDBLUE = LEDON;
	Delay(1 << 15);
	LEDBLUE = LEDOFF;
	Delay(1 << 15);

	Copy();
	read_bootflag = (ft_read_bootflag)0x001059b0;
	write_bootflag = (ft_write_bootflag)0x0010586c;

	Zero(buf1, FLAG_BUF_SIZE);
	Zero(buf2, FLAG_BUF_SIZE);

	// call Read routine - all tests ok so far
	read_bootflag(buf1, buf2);

	//test flags again - should still be legal values
	if (boot_flag[0] != 0 || boot_flag[2] != -1) m = 1;
	if (boot_flag[1] != 0 && boot_flag[1] != -1) m = 1;

	//test if Read routine properly copied flags to buf1
	if (buf1[0] != boot_flag[0] || buf1[1] != boot_flag[1] || buf1[2] != boot_flag[2]) m=1;

	//if Read tests fail, end here with red light on - hopefully no damage done
stop:	if (m != 0) LEDRED = LEDON;
	else {

		//flash blue light 2nd time if Read routine appears to have worked
		LEDBLUE = LEDON;
		Delay(1 << 15);
		LEDBLUE = LEDOFF;
		Delay(1 << 15);

		//both lights on during write
		LEDBLUE = LEDON;
		LEDRED = LEDON;

		// toggle boot flag - enable/disable boot from cf card
		if (buf1[1] == 0)
			buf1[1] = -1;
		else
			buf1[1] = 0;

		// write to flash
		write_bootflag(buf1, buf2);
		Delay(1 << 15);
		LEDBLUE = LEDOFF;
		LEDRED = LEDOFF;
		Delay(1 << 15);

		// verify, blue = ok
		if (buf1[0] == boot_flag[0] && buf1[1] == boot_flag[1] && buf1[2] == boot_flag[2])
			LEDBLUE = LEDON;
		else
			LEDRED = LEDON;

	}

	for (;;);
}
