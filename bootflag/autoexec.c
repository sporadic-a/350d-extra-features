delay(int i)
{
	while(--i) {
		int j = (1 << 8);
		while (--j) {
			asm("NOP");asm("NOP");asm("NOP");asm("NOP");
		}
	}
}

#define LEDRED (*((int*)0xC02200A0))
#define LEDBLUE (*((int*)0xC0220000))
#define LEDON   0x46
#define LEDOFF  0x44

#define FW_ADDRESS      0xFF810000

void main() {
	for (; ; ) {
		LEDRED = LEDON;
		delay(1 << 14);
		LEDRED = LEDOFF;
		delay(1 << 14);
		LEDBLUE = LEDON;
		delay(1 << 14);
		LEDBLUE = LEDOFF;
		delay(1 << 14);
	}
}
