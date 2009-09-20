#include "headers.h"

//Button Value
// 0xa0:		menu
// 0xa1:		info.
// 0xa4:		play
// 0xa5:		trash(?)
// 0xa6:		set
// 0xb1:		up
// 0xb2:		down
// 0xb3:		left
// 0xb4:		right
// 0xb8:		direct print
// 0xb9:		disp. ? or focus frame
// 0xba:		drive mode
// 0xbb:		AV+-
// 0x90-0x93:	Mode Dial Tv Av M aDep

// Camera Mode
// 350D: 0x142b8
//AeValue address 0x142b8+0x00
	//0: P
	//1: Tv
	//2: Av
	//3: M
	//4: unknow1
	//5: A-DEP
	//8: Full Auto (Green rectangular) 
//MesureValue address 0x142b8+0x04
	//3 Spot 
	//0 Evaluative
//Drive mode address 0x142b8+0x0c
	//0 Single shooting
	//1 Continuous shooting
//? +0x18	AF set
//AvComp Value address 0x142b8+0x24
//IsoValue address 0x142b8+0x28
//? +0x38	0 beep off, 1 beep on
//? +0x60
//? +0x90
//? +0xa8

// 0xe7e0 MainPreserveData
// +0x00:
// +0x02:
// +0x04:
// +0x06:
// +0x08:
// +0x0c:	ReleaseCount
// +0x10:
// +0x11:

//-----------------------==SetPropertie
// SendToIntercom(int key, int oldVal?, int newVal)
// key:
// 0x01:	AE mode						oldVal,newVal
// 0x02:	Metering mode				oldVal,newVal
// 0x02:	AF mode						oldVal,newVal?
// 0x04:	Drive mode					oldVal,newVal
// 0x08:	Tv							oldVal,newVal
// 0x09:	Av							oldVal,newVal
// 0x0a:	AVComp value				oldVal,newVal
// 0x0b:	Iso							oldVal,newVal
// 0x1c:	SelectProcParam				1,newVal
// 0x1d:	SetProcParam: Contrast		4,newVal
// 0x1e:	SetProcParam: Sharpness		4,newVal
// 0x1f:	SetProcParam: ColorGain		4,newVal
// 0x20:	SetProcParam: Hue			4,newVal
// 0x21:	Color space					1,newVal
// 0x2a-3b	CFn1-18						1,newVal
// 0x3d:	see eventproc_SendCommand	*
// 0x50:
// 0x51:
// 0x52:	Shutdown					0,0
// 0x53:	Release						0,0
// 0x54:
// 0x55:
// 0x59:
// 0x5a:
// 0x5f:
// 0x60:
// 0x61:
// 0x62:
// 0x69:
// 0x6b:
// 0x6d:	Release						1,1
// 0x73:

//-------------------------0x1f60
// read by eventproc_SetAeMode, no write code found
// 0x1f60+0x00: AeMode
// 0x1f60+0x02: MeteringMode
// 0x1f60+0x06: DriveMode
// 0x1f60+0x0a: AFMode
// 0x1f60+0x0e: TvValue
// 0x1f60+0x10: AvValue  
// 0x1f60+0x12: AvCompValue 
// 0x1f60+0x14: IsoValue

typedef struct {
	int AEMode;			// 0x00
	int MeteringMode;	// 0x04
	int unknown1[1];	// 0x08
	int DriveMode;		// 0x0c
	int unknown2[2];	// 0x10
	int AFSet;			// 0x18
	int unknown3[2];	// 0x1c
	int AvComp;			// 0x24
	int Iso;			// 0x28
	int unknown4[3];	// 0x2c
	int BeepOn;			// 0x38
} type_CAMERA_MODE;

static const int meteringModeSpot = 3;
static const int meteringModeEvaluative = 0;

#define cameraMode				(*((type_CAMERA_MODE*)0x142b8))	// CameraMode struct

#define isInFactoryMode			(*((int*)0x15284))				// != 0 when in factory mode
#define isBeepInit				(*((int*)0x398c))
#define isDigitalGainEnabled	(*((int*)0x3df8))
#define isMainMenuDlgOpened		(*((int*)0x22ac))				// Main menu Dialog opened flag

#define MESSAGE_BUTTON_DP		0x01

int* hMyTaskMessQue;

#define STD_IN					0
#define STD_OUT					1
#define STD_ERR					2

void MyGlobalStdSet () {
	int f1 = -1;
	for (; f1 == -1; ) {
		f1 = FIO_CreateFile("A:/STDOUT.TXT");
		if (f1 == -1)
			SleepTask(100);
	}
	ioGlobalStdSet(STD_OUT, f1);	// ioGlobalStdSet(STD_ERR, f1);
}


void MyTask() {
	int* pMessage;

	for (;;) {
		ReceiveMessageQueue(hMyTaskMessQue, &pMessage, 0);
		TryReceiveMessageQueue(hMyTaskMessQue, &pMessage, 0);
		switch (pMessage[0]) {
			case MESSAGE_BUTTON_DP:
				if (isMainMenuDlgOpened != 0) {					// Main menu Dialog opened
					//eventproc_RiseEvent("ShowColorBar", 0);
					//eventproc_RiseEvent("DispIsoMode", 0);
					int isoVal = 0x6f;
					MyGlobalStdSet();
					SendToIntercom(0x2a + 8 - 1, 1, 1);				// enable CFn.8 for ISO H
					// EnterFactoryMode();							// nothing happened	// eventproc_RiseEvent("EnterFactoryMode");
					eventproc_RiseEvent("SetIsoValue", isoVal);		// eventproc_SetIsoValue(&isoVal);
					eventproc_RiseEvent("PrintICUInfo", 0);			// print ReleaseCount
					if (cameraMode.BeepOn != 0)
						Beep(0);
					break;
				}
				break;
		}
		MainHeapFree(pMessage);
	}
}

void CreateMyTask() {
	hMyTaskMessQue = (int*)CreateMessageQueue("MyTaskMessQue", 0x40);
	CreateTask("MyTask", 0x19, 0x2000, MyTask, 0);
}

void SendMyMessage(int param0, int param1) {
	int* pMessage = (int*)MainHeapAlloc(sizeof(int) * 2);
	pMessage[0] = param0;
	pMessage[1] = param1;
	TryPostMessageQueue(hMyTaskMessQue, pMessage, 0);
}

void my_IntercomHandler(int r0, char* ptr) {
    if (ptr[1] == 0xb8) {						// Press Dp to set Iso
		SendMyMessage(MESSAGE_BUTTON_DP, 0);
	}

	IntercomHandler(r0, ptr);
}
