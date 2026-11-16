#include "headers.h"

// ADDON_SIZE and ADDON_BASE come from the Makefile.
#if ADDON_SIZE % 4
#error ADDON_SIZE must be a multiple of 4
#endif

// Move the add-on from where the camera loaded it to the address it was
// linked to run at. The two ranges sit side by side below and above
// 0x800000, so they never overlap.
void COPY() {
	int i;
	long* from	= (long*)0x800000;
	long* to	= (long*)ADDON_BASE;

	//for (i = 0; i < 0x1400; i++)
	//	to[i] = from[i];

	for (i = 0; i < ADDON_SIZE / 4; i++)
		to[i] = from[i];
}

extern void my_IntercomHandler();

int my_InitializeIntercom() {
	printf("InitializeIntercom\n");			//disabled by ck
	InitIntercomData(my_IntercomHandler);
	CreateIntercomSem();
	return 0;
}
