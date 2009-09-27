#include "headers.h"

//Button Value
// 0x89:		power on
// 0x92:		close battery door
// 0xa0:		menu
// 0xa1:		info.	(400D: jump?)
// 0xa4:		play
// ?0xa5:		trash(?)
// 0xa6:		set
// 0xb1:		up
// 0xb2:		down
// 0xb3:		right
// 0xb4:		left
// 0xb8:		direct print
// ?0xb9:		disp. ? or focus frame
// ?0xba:		drive mode
// ?0xbb:		AV+-
// 0x90-0x93:	Mode Dial Tv Av M aDep

// Camera Mode
// 350D: 0x142b8
// 400D: 0x16b60
//---------------0x142b8 store value (same as 0x25E20 for 400D) ---------------------
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

//SendToIntercom(0x1,1,1); //(0x0,1,2);  Zonedial mode P TV AV....
//SendToIntercom(0x2,1,1); //(0x2,1,0);  Meter mode Eval, Center... 
//SendToIntercom(0x3,1,1); //(0x3,1,0);  Flash ex comp 
//SendToIntercom(0x4,1,1); //(0x4,1,0);  drive mode 
//SendToIntercom(0x5,1,1); //(0x5,1,0);  WB chose 
//SendToIntercom(0x6,1,1); //(0x6,1,0);  MF manual focus
//SendToIntercom(0x7,1,1); // AF point selecttion: can extend some special: 7, 27, 41,47, 49, 73, 81,87, 97, 101, 105,113, 120,121, 116, 127, 135,
//					139,165,168,169, 175, 185,   
//SendToIntercom(0x8,2,1); //Tv value:
//SendToIntercom(0xA,1,1); //AV comp ex selecttion: can extend some special  
//SendToIntercom(0xB,1,1); // ISO set 
//SendToIntercom(0xC,1,1); // Red eye 
//SendToIntercom(0xD,1,1); //AEB BRK  
//SendToIntercom(0xE,1,1); //WB BRK 
//SendToIntercom(0xF,1,1); // Beep
//SendToIntercom(22,1,1); // LCDBrightness 
//SendToIntercom(34,1,1); // RAW only, L+RAW, L only
//SendToIntercom(35,1,1); // S,M,L. Combine no.34 to M+RAW OK, S+RAW not ok
//SendToIntercom(36,1,1); // JPG Fine or Medium quality.
//SendToIntercom(43,1,1); // wb+-
//SendToIntercom(44~~>49,1,1); // Cf1-->6
//SendToIntercom(54,1,1); // Cf7
//SendToIntercom(56,1,1); // Cf8
//SendToIntercom(0x3C,1,0); //(0x3C,1,1); CFn10 
//SendToIntercom(0x3B,1,1); //(0x3B,1,0); CFn11 
//SendToIntercom(0x39,1,1); //(0x39,1,0);  CFn9 
//SendToIntercom(0x59,1,1); // Clear camera seting  
//SendToIntercom(0x61,1,1); // Start update firmware   
//SendToIntercom(0x6B,1,1); // Full CF viewfinder message
//SendToIntercom(0x6C,1,1); // Change Battery dialog
//SendToIntercom(0x6D,1,1); // Burst counter set 

//-------------------------0x1f60, 400D: 0x1cf0
// read by eventproc_SetAeMode etc., no write code found
// 0x1f60+0x00: AeMode
// 0x1f60+0x02: MeteringMode
// 0x1f60+0x06: DriveMode
// 0x1f60+0x0a: AFMode
// 0x1f60+0x0e: TvValue
// 0x1f60+0x10: AvValue  
// 0x1f60+0x12: AvCompValue 
// 0x1f60+0x14: IsoValue

//--------------------------
// 400D: 0x6D58:   =1 Display on, =0 disp off

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

#define releaseCount			(*((int*)(0xe7e0 + 0x0c)))

#define isInFactoryMode			(*((int*)0x15284))				// != 0 when in factory mode
#define isBeepInit				(*((int*)0x398c))
#define isDigitalGainEnabled	(*((int*)0x3df8))
#define handleMainMenuDlg		(*((int*)0x22ac))				// Main menu Dialog handle
#define handleMainMenuDlg2		(*((int*)0x22bc))				// Main menu Dialog 2 handle
#define handleSubMenuDlg		(*((int*)0x22b8))				// Sub menu Dialog handle
#define handleInfoCreativeDlg	(*((int*)0x2304))				// Info creative Dialog handle

#define MESSAGE_DP				0x01
#define MESSAGE_ISO				0x02

#define	BUTTON_MENU				0xa0
#define	BUTTON_INFO				0xa1
#define	BUTTON_PLAY				0xa4
#define	BUTTON_SET				0xa6
#define	BUTTON_UP				0xb1
#define	BUTTON_DOWN				0xb2
#define	BUTTON_RIGHT			0xb3
#define	BUTTON_LEFT				0xb4
#define	BUTTON_DP				0xb8

int* hMyTaskMessQue;

//void MyGlobalStdSet () {
//	int f1 = -1;
//	for (; f1 == -1; ) {
//		f1 = FIO_OpenFile("A:/STDOUT.TXT", (O_CREAT | O_WRONLY), 0644);
//		if (f1 == -1)
//			SleepTask(100);
//	}
//	FIO_SeekFile(f1, 0, SEEK_END);
//	ioGlobalStdSet(STD_OUT, f1);
//}

void PrintReleaseCount () {
	int fout = FIO_OpenFile("A:/shutter.txt", (O_CREAT | O_WRONLY), 0644);
	if (fout != -1) {
		char* buf[64];
//		FIO_SeekFile(fout, 0, SEEK_END);
		sprintf(buf, "ReleaseCount:\t%d\n\r", releaseCount);
		FIO_WriteFile(fout, buf, strlen(buf));
		FIO_CloseFile(fout);
	}
}

int lastButton = 0;
unsigned long lastTick = 0;

void SwitchIso(int button) {
	int isoVal = cameraMode.Iso;
//	int cfn[] = {8, 1};
//	eventproc_SetCustomFunc(cfn);
	int tick = tickGet();
	if ((lastTick != 0) && ((tick - lastTick) < 20UL) && (lastButton == button))
		return;
	LEDBLUE = LEDON;
	lastButton = button;
	if (tick == 0)
		lastTick = 1;
	else
		lastTick = tick;
	if (isoVal == 0x6f)
		isoVal = 0x70;
	switch (button) {
		case BUTTON_LEFT:
		{
			switch (isoVal & 0x0f) {
				case 0x00:
				case 0x03:
				case 0x08:
				case 0x0b:
					isoVal -= 0x03;
					break;
				case 0x05:
				case 0x0d:
					isoVal -= 0x02;
					break;
			}
			break;
		}
		case BUTTON_RIGHT:
		{
			switch (isoVal & 0x0f) {
				case 0x00:
				case 0x05:
				case 0x08:
				case 0x0d:
					isoVal += 0x03;
					break;
				case 0x03:
				case 0x0b:
					isoVal += 0x02;
					break;
			}
			break;
		}
		case BUTTON_UP:
		{
			isoVal += 0x08;
			break;
		}
		case BUTTON_DOWN:
		{
			isoVal -= 0x08;
			break;
		}
	}
	if (isoVal > 0x70)
		isoVal = 0x48;
	if (isoVal < 0x48)
		isoVal = 0x70;
	if (isoVal == 0x70)
		isoVal = 0x6f;
	SendToIntercom(0x2a + 8 - 1, 1, 1);				// enable CFn.8 for ISO H
	eventproc_RiseEvent("SetIsoValue", isoVal);		// eventproc_SetIsoValue(&isoVal);
//	my_ShowInfoCreativeDlg();
//	if (cameraMode.BeepOn != 0)
//		Beep(0);
	LEDBLUE = LEDOFF;
}

const char* my_GetISOString(int isoVal) {
	switch (isoVal) {
		case 0x00: return "Auto";
		case 0x28: return "6";
		case 0x2b: return "8";
		case 0x2d: return "10";
		case 0x30: return "12";
		case 0x33: return "16";
		case 0x35: return "20";
		case 0x38: return "25";
		case 0x3b: return "32";
		case 0x3d: return "40";
		case 0x40: return "50";
		case 0x43: return "64";
		case 0x45: return "80";
		case 0x48: return "100";
		case 0x4b: return "125";
		case 0x4d: return "160";
		case 0x50: return "200";
		case 0x53: return "250";
		case 0x55: return "320";
		case 0x58: return "400";
		case 0x5b: return "500";
		case 0x5d: return "640";
		case 0x60: return "800";
		case 0x63: return "1000";
		case 0x65: return "1250";
		case 0x68: return "1600";
		case 0x6b: return "2000";
		case 0x6d: return "2500";
		case 0x6f: return "3200";
	}
	return "";
}

void MyTask() {
//	MyGlobalStdSet();
	for (;;) {
		int* pMessage = 0;
		ReceiveMessageQueue(hMyTaskMessQue, &pMessage, 0);
//		TryReceiveMessageQueue(hMyTaskMessQue, &pMessage, 0);
//		TryReceiveMessageQueue(hMyTaskMessQue, &pMessage, 0);
		if (pMessage == 0)
			continue;
		switch (pMessage[0]) {
			case MESSAGE_DP:
				if (handleMainMenuDlg != 0) {						// Main menu Dialog opened
					// int colorVal = 10000;
					LEDBLUE = LEDON;
					// EnterFactoryMode();							// nothing happened	// eventproc_RiseEvent("EnterFactoryMode");
					// eventproc_RiseEvent("ShowColorBar", 0);
					// eventproc_RiseEvent("DispIsoMode", 0);
					// eventproc_RiseEvent("PrintICUInfo", 0);		// print ReleaseCount
					// eventproc_SetColor(&colorVal);
					PrintReleaseCount();
					// ShowImageInfoDlg();
					// my_ShowInfoCreativeDlg();
					if (cameraMode.BeepOn != 0)
						Beep(0);
					LEDBLUE = LEDOFF;
				}
				break;
			case MESSAGE_ISO:
				SwitchIso(pMessage[1]);
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
    switch (ptr[1]) {
		case BUTTON_DP:
			SendMyMessage(MESSAGE_DP, ptr[1]);
			break;
		case BUTTON_UP:
		case BUTTON_DOWN:
		case BUTTON_LEFT:
		case BUTTON_RIGHT:
			if (handleInfoCreativeDlg != 0) {
				SendMyMessage(MESSAGE_ISO, ptr[1]);
				return;
			}
			break;
	}

	IntercomHandler(r0, ptr);
}
