#include "headers.h"

void COPY() {
	int i;
	long* from	= (long*)0x800000;
	long* to	= (long*)0x7f0000;
	for (i = 0; i < 4000; i++)
		to[i] = from[i];
}

extern void my_IntercomHandler();

int my_InitializeIntercom() {
	printf("InitializeIntercom\n");
	InitIntercomData(my_IntercomHandler);
	CreateIntercomSem();
	return 0;
}
