/*
 * EOS 350D mod project
 * Original work by maiohwmai and cyrustam
 * Further mods by crazyklaus and MilanC
 */

#include "headers.h"
#include "user_settings.txt"

typedef struct {
        int AEMode;			// 0x00
        int MeteringMode;		// 0x04
        int unknown1[1];		// 0x08
        int DriveMode;			// 0x0c
        int WB;				// 0x10
        int AF;				// 0x14
        int AFPoint;			// 0x18
        int TvVal;			// 0x1c
        int AvVal;			// 0x20
        int AvComp;			// 0x24
        int Iso;			// 0x28
        int unknown4[1];		// 0x2c
        int Aeb;			// 0x30
        int unknown5[1];		// 0x34
        int BeepOn;			// 0x38
        int ColorTemp;			// 0x3c
        int unknown6[12];		// 0x40,0x44,0x48,0x4c,0x50,0x54,0x58,0x5c,0x60,0x64,0x68,0x6c
        int ParaSetOne;			// 0x70
        int ParaSetTwo;			// 0x74
        int ParaSetThree;		// 0x78
        int ParaSetBW;			// 0x7c
        int unknown7a[5];		// 0x80,0x84,0x88,0x8c,0x90,
        int menupos;			// 0x94
        int unknown7b[9];		// 0x98,0x9c,0xa0,0xa4,0xa8,0xac,0xb0,0xb4,0xb8 
        int CfEmitFlash;		// 0xbc
        int CfExtendIso;		// 0xc0 
        int CfAebSeq;			// 0xc4
        int CfSiIndicate;		// 0xc8
        int CfMenuPos;			// 0xcc
        int CfMirrorUpLock;		// 0xd0
        int CfFpselMethod;		// 0xd4
        int CfFlashMetering;		// 0xd8
        int CfFlashSyncRear;		// 0xdc
        int CfSafetyShift;		// 0xe0
        int CfLensButton;		// 0xe4
        int CfOriginalEval;		// 0xe8
} type_CAMERA_MODE;

int fn=0;
int* hMyTaskMessQue;

char buff[7];
char buff2[4];

int TLapseInterval=10;
int TLapseFrames=20;
int RawMode=1;
int JpgQual=0;
int JpgSize=0;

#define cameraMode				(*((type_CAMERA_MODE*)0x142b8))		// CameraMode struct
#define releaseCount				(*((int*)(0xe7e0 + 0x0c)))
#define isInFactoryMode				(*((int*)0x15284))			// != 0 when in factory mode
#define isBeepInit				(*((int*)0x398c))
#define isDigitalGainEnabled			(*((int*)0x3df8))
#define handleMainMenuDlg			(*((int*)0x22ac))			// Main menu Dialog handle
#define handleMainMenuDlg2			(*((int*)0x22bc))			// Main menu Dialog 2 handle
#define handleSubMenuDlg			(*((int*)0x22b8))			// Sub menu Dialog handle
#define handleInfoCreativeDlg			(*((int*)0x2304))			// Info creative Dialog handle
#define MESSAGE_DP				0x01
#define MESSAGE_ISO				0x02
#define MESSAGE_SPOT				0x03
#define MESSAGE_ZI				0x04
#define MESSAGE_AEB				0x05
#define MESSAGE_UD				0x06
#define MESSAGE_LR				0x07
#define MESSAGE_ZO				0x08
#define	BUTTON_MENU				0xa0
#define	BUTTON_INFO				0xa1
#define	BUTTON_PLAY				0xa4
#define	BUTTON_SET				0xa6
#define	BUTTON_ZI				0xa7
#define	BUTTON_ZO				0xa8
#define	BUTTON_UP				0xb1
#define	BUTTON_DOWN				0xb2
#define	BUTTON_RIGHT				0xb3
#define	BUTTON_LEFT				0xb4
#define	BUTTON_DP				0xb8

#define QUOTE_(x) #x		//macros I need to get the numbers in the menu right in english version
#define QUOTE(x) QUOTE_(x)	//(I skipped the numbers in the german translation to have more space)

char* aebH2S(int aeb){
	int aebone=0;
	int aebtwo=0;
	switch(aeb&0xf8){
	   case 0x08:
	      aebone=1;
	      break;
	   case 0x10:
	      aebone=2;
	      break;
	   case 0x18:
	      aebone=3;
	      break;
	   case 0x20:
	      aebone=4;
	      break;
	   case 0x28:
	      aebone=5;
	      break;
	   case 0x30:
	      aebone=6;
	}
	switch(aeb&0x07){
	   case 3:
	      aebtwo=3;
	      break;
	   case 4:
	      aebtwo=5;
	      break;
	   case 5:
	      aebtwo=7;
	}
	sprintf(buff,"%d.%d",aebone,aebtwo);
	return buff;
}

char* avcompH2S(int avcomp){
	int avone=0;
	int avtwo=0;
	if (avcomp < 0x80) {
		switch(avcomp&0xf8){
			case 0x08:
				avone=1;
				break;
			case 0x10:
				avone=2;
		  }
		switch(avcomp&0x07){
			case 3:
				avtwo=3;
				break;
			case 4:
				avtwo=5;
				break;
			case 5:
				avtwo=7;
		  }
		sprintf(buff2,"%d.%d",avone,avtwo);
	  }
	else {
		switch((0x100-avcomp)&0xf8) {
			case 0x08:
				avone=1;
				break;
			case 0x10:
				avone=2;
		  }
		switch((0x100-avcomp)&0x07) {
			case 3:
				avtwo=3;
				break;
			case 4:
				avtwo=5;
				break;
			case 5:
				avtwo=7;
		  }
		sprintf(buff2,"-%d.%d",avone,avtwo);
	  }
	return buff2;
}

inline char* colortempH2S(int colortemp){
  sprintf(buff, "%dK", colortemp);
  return buff;
}

inline char* ShutterCountH2S(int tmpshut){
  sprintf(buff, "%d", tmpshut);
  return buff;
}

/* this function copies the strings a and b to the destination string c */
inline void concat( char *a, char *b, char *c){
        while( *a )  {
                *c = *a; ++a, ++c;
        }
        while( *b )  {
                *c = *b; ++b, ++c;
        }
        *c = '\0';
}

#ifdef POSITION_ISO
void SwitchIso(int button) {
	int isoVal = cameraMode.Iso;
	LEDSON
	if (isoVal == 0x6f)
		isoVal = 0x70;
	switch (button) {
		case BUTTON_RIGHT:
			switch (isoVal & 0xf) {
				case 3:
				case 0xb:
					isoVal+=2;
					break;
				default:
					isoVal+=3;
			}
			break;
		case BUTTON_LEFT:
			switch (isoVal & 0xf) {
				case 5:
				case 0xd:
					isoVal-=2;
					break;
				default: 
					isoVal-=3;
			}
			break;
	}
	if (isoVal > 0x70)
		isoVal = 0x48;
	if (isoVal < 0x48)
		isoVal = 0x70;
	if (isoVal == 0x70)
		isoVal = 0x6f;
	SendToIntercom(0x2a + 8 - 1, 1, 1);				// enable CFn.8 for ISO H
	eventproc_RiseEvent("SetIsoValue", isoVal);		// eventproc_SetIsoValue(&isoVal);
	LEDSOFF
}
#endif

#ifdef POSITION_AEB
void SwitchAeb(int button) {
	int aebVal = cameraMode.Aeb;
	LEDSON
	switch (button) {
		case BUTTON_LEFT:
			if (aebVal != 0)
				switch (aebVal & 0xf) {
					case 0x00:
					case 0x03:
					case 0x08:
					case 0x0b:
						aebVal-=3;
						break;
					default:
						aebVal--;
				  }
				break;
		case BUTTON_RIGHT:
			if (aebVal != 0x35)
				switch (aebVal & 0xf) {
					case 0x00:
					case 0x05:
					case 0x08:
					case 0x0d:
						aebVal+=3;
						break;
					default:
						aebVal++;
                        }
          }
        SendToIntercom(0xd,1,aebVal);
	LEDSOFF
}
#endif

#ifdef POSITION_CT
void SwitchColorTemp(int button) {
	int ColorTempVal = cameraMode.ColorTemp;
	LEDSON
	switch (button) {
		case BUTTON_LEFT:
			ColorTempVal-=0x64;	
			break;
		case BUTTON_RIGHT:
			ColorTempVal+=0x64;
          }
        if(ColorTempVal>0x2af8)
		ColorTempVal=0x2af8;
        if(ColorTempVal<0x708)
		ColorTempVal=0x708;
        SendToIntercom(0x10, 2, ColorTempVal);
        LEDSOFF
}
#endif

#ifdef POSITION_METERING
void SwitchMetering(int button) {
	int TmpVal = cameraMode.MeteringMode;
	LEDSON
	switch (button) {
		case BUTTON_LEFT:
			if(TmpVal)			//when not 0 decrease
				TmpVal--;	
			break;
		case BUTTON_RIGHT:
			if(TmpVal<3)			//if smaller than 3, increase
				TmpVal++;
          }
        SendToIntercom(0x02, 1, TmpVal);
        LEDSOFF
}
#endif

#ifdef POSITION_WB
void SwitchWB(int button) {
	int TmpVal = cameraMode.WB;
	LEDSON
	switch (button) {
		case BUTTON_LEFT:
			if(TmpVal)
				TmpVal--;	
			break;
		case BUTTON_RIGHT:
			if(TmpVal<0x08)
				TmpVal++;
	  }
	SendToIntercom(0x05, 1, TmpVal);
	LEDSOFF
}
#endif

#ifdef POSITION_TLAPSE
void SwitchTimeLapse(int button) {
	int i=0;
	switch (button) {
		case BUTTON_SET:
			while (i < TLapseFrames) {
				i++;
				SendToIntercom(0x53,0,0);
				SleepTask(TLapseInterval*1000);
			  }
			break;
		case BUTTON_LEFT:
			if (TLapseInterval>1) TLapseInterval--;
			break;
		case BUTTON_RIGHT:
			TLapseInterval += 1;
			break;
		case BUTTON_MENU:
			TLapseFrames *= 2;
			break;
		case BUTTON_INFO:
			if (TLapseFrames>1)
				TLapseFrames /= 2;
			break;
		case BUTTON_PLAY: TLapseFrames++;
	  }
}
#endif

#ifdef POSITION_RAWMODE
void SwitchRawMode(int button)
  {
	switch (button) {
		case BUTTON_LEFT:
			RawMode = (RawMode%3)+1;
			break;
		case BUTTON_SET:
			JpgQual = (JpgQual+1)&0x1;
			break;
		case BUTTON_RIGHT:
			JpgSize = (JpgSize+1)%3;
	  }
	SendToIntercom(0x22,1,RawMode);
	SendToIntercom(0x24,1,JpgQual);
	SendToIntercom(0x23,1,JpgSize);
  }
#endif

int CfnVal(int button, int curr, int max, int min) {
        switch (button) {
                case BUTTON_LEFT:
                        if(curr!=min)
					curr--;
			break;
                case BUTTON_RIGHT:
                        if(curr!=max)
					curr++;
        }
	return curr;
}

int ToggleCfnVal(int button, int curr) {				//allows ON/OFF only options to be toggled with BOTH left and right button
	if ((button==BUTTON_LEFT)||(button==BUTTON_RIGHT))
		curr^=1;
	return curr;
}

const char* my_GetISOString(int isoVal) {
	switch (isoVal) {
		case 0x00: return "Auto";
/*		case 0x28: return "6";
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
		case 0x45: return "80";*/
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

char *aebstr;
const char* my_GetAEBString(int aebVal) {
	int AvCompVal = cameraMode.AvComp;
	int AebVal = cameraMode.Aeb;
	char *aebstr1=avcompH2S(AvCompVal);
	char *aebstr2=" +- ";
	char *aebstr3=aebH2S(AebVal);
	char *tmpaebstr;
	concat(aebstr1, aebstr2, tmpaebstr);
	concat(tmpaebstr, aebstr3, aebstr); 
	return aebstr;
}

// !!IMPORTANT!!
// NEXT 3 FUNCTIONS: GERMAN VERSION, LOOK DOWN FOR ENGLISH VERSION

#ifdef DEUTSCH
char *colortempstr;
const char* my_GetColorTempString() {
	concat("FarbTemp:", colortempH2S(cameraMode.ColorTemp), colortempstr);
	return colortempstr;
}

char *dynamicstr;
const char* my_GetDynamicString() { 
	switch (fn) {
#ifdef POSITION_ISO
		case POSITION_ISO-1:
#endif
#ifdef POSITION_CT
		case POSITION_CT-1:
#endif
#ifdef POSITION_AEB
		case POSITION_AEB-1: return "einstellen r/l";
#endif
#ifdef POSITION_WB
		case POSITION_WB-1:
			switch (cameraMode.WB){
				case 0: return "AutoWeissabgl.";
				case 1: return "Tageslicht";
				case 2: return "Wolkig";
				case 3: return "Gluehlampe";
				case 4: return "Leuchtstoff";
				case 5: return "Blitz";
				case 6: return "Manuell";
				case 7: return "Schatten";
				default: return "Farbtemperatur";
			}
#endif
#ifdef POSITION_METERING
		case POSITION_METERING-1:
			switch(cameraMode.MeteringMode){
				case 0: return "Mehrfeldmessung";
				case 1: return "Integralmessung";
				case 2: return "Selektivmessung";
				case 3: return "Spotmessung";
			}
#endif
#ifdef POSITION_BSEQ
		case POSITION_BSEQ-1:
			switch(cameraMode.CfAebSeq){
				case 0: return "Sequenz 0-+,Aus";
				case 1: return "Sequenz 0-+,Ein";
				case 2: return "Sequenz -0+,Aus";
				case 3: return "Sequenz -0+,Ein";
			}
#endif
#ifdef POSITION_SI
		case POSITION_SI-1:
			if (cameraMode.CfSiIndicate) 
				return "Aus";
			else
				return "Ein";
#endif
#ifdef POSITION_MENU
		case POSITION_MENU-1:
			switch(cameraMode.CfMenuPos){
				case 0: return "Standard";
				case 1: return "immer merken";
				default: return "immer Menue 1";
			}
#endif
#ifdef POSITION_SAFESHIFT
		case POSITION_SAFESHIFT-1:
			if (cameraMode.CfSafetyShift) 
				return "Ein";
			else
				return "Aus";
#endif
#ifdef POSITION_AFBUTTON
		case POSITION_AFBUTTON-1:
			switch(cameraMode.CfLensButton){
				case 0: return "AF Stop";
				case 1: return "AF Start";
				case 2: return "AELock";
				case 3: return "AF Point";
				case 4: return "OneShot/AISer.";
				default: return "IS Start";
			}
#endif
#ifdef POSITION_DATA
		case POSITION_DATA-1:
			if (cameraMode.CfOriginalEval) 
				return "Ein";
			else
				return "Aus";
#endif
#ifdef POSITION_FLASH
		case POSITION_FLASH-1:
			if (cameraMode.CfEmitFlash) 
				return "Aus";
			else
				return "Ein";
#endif
#ifdef POSITION_COUNTER
		case POSITION_COUNTER-1:
			concat(ShutterCountH2S(releaseCount), " Ausloesg", dynamicstr);
			return dynamicstr;
#endif
#ifdef POSITION_TLAPSE
		case POSITION_TLAPSE-1:
			sprintf(dynamicstr, "%d x %ds", TLapseFrames, TLapseInterval);
			return dynamicstr;
#endif
#ifdef POSITION_RAWMODE
		case POSITION_RAWMODE-1:
			switch(RawMode) {
				case 1: return "nur JPG";
				case 2: return "nur RAW";
				default: return "RAW+JPEG";
			  }
#endif
		default:
			return "";
	}
}

const char* my_GetCurSetString() {
	switch (fn) {
#ifdef POSITION_ISO
		case POSITION_ISO-1: return "ISO-Wert";
#endif
#ifdef POSITION_WB
		case POSITION_WB-1: return "Weissabgleich";
#endif
#ifdef POSITION_CT
		case POSITION_CT-1: return "Farbtemperatur";
#endif
#ifdef POSITION_METERING
		case POSITION_METERING-1: return "BeliMessmethode";
#endif
#ifdef POSITION_AEB
		case POSITION_AEB-1: return "AEB";
#endif
#ifdef POSITION_BSEQ
		case POSITION_BSEQ-1: return "AEBSeq/Merken";
#endif
#ifdef POSITION_SI
		case POSITION_SI-1: return "AF-Punkte(rot)";
#endif
#ifdef POSITION_MENU
		case POSITION_MENU-1: return "Menue-Position";
#endif
#ifdef POSITION_SAFESHIFT
		case POSITION_SAFESHIFT-1: return "AutoKorr Av/Tv";
#endif
#ifdef POSITION_AFBUTTON
		case POSITION_AFBUTTON-1: return "?AFKnopf";
#endif
#ifdef POSITION_DATA
		case POSITION_DATA-1: return "ZusatzdatenRAW";
#endif
#ifdef POSITION_FLASH
		case POSITION_FLASH-1: return "Blitz Ein/Aus";
#endif
#ifdef POSITION_COUNTER
		case POSITION_COUNTER-1: return "intern.Zaehler";
#endif
// TODO: translate
#ifdef POSITION_TLAPSE
		case POSITION_TLAPSE-1: return "Zeitraffer";
#endif
#ifdef POSITION_RAWMODE
		case POSITION_RAWMODE-1: return "Bildqualitaet";
#endif
		default: return "Fehler";
	}
}

#else

// NOW THE ENGLISH PART (next 3 functions), LOOK ABOVE FOR GERMAN VERSION

char *colortempstr;
const char* my_GetColorTempString() {
	concat("CTemp:", colortempH2S(cameraMode.ColorTemp), colortempstr);
	return colortempstr;
}

char *dynamicstr;
const char* my_GetDynamicString() { 
	switch (fn) {
#ifdef POSITION_WB
		case POSITION_WB-1:
			switch (cameraMode.WB){
				case 0: return "AWB";
				case 1: return "Daylight";
				case 2: return "Cloudy";
				case 3: return "Tungsten";
				case 4: return "Fluorescent";
				case 5: return "Flash";
				case 6: return "Custom";
				case 7: return "Shade";
				default: return "ColorTemp";
			}
#endif
#ifdef POSITION_METERING
		case POSITION_METERING-1:
			switch(cameraMode.MeteringMode){
				case 0: return "Evaluative";
				case 1: return "Center W.";
				case 2: return "Partial";
				case 3: return "Spot";
			}
#endif
#ifdef POSITION_BSEQ
		case POSITION_BSEQ-1:
			switch(cameraMode.CfAebSeq){
				case 0: return "0-+,On";
				case 1: return "0-+,Off";
				case 2: return "-0+,On";
				case 3: return "-0+,Off";
			}
#endif
#ifdef POSITION_SI
		case POSITION_SI-1:
			if (cameraMode.CfSiIndicate) 
				return "Off";
			else
				return "On";
#endif
#ifdef POSITION_MENU
		case POSITION_MENU-1:
			switch(cameraMode.CfMenuPos){
				case 0: return "Prev/Top(PwdDw)";
				case 1: return "Previous";
				default: return "Top Menu";
			}
#endif
#ifdef POSITION_SAFESHIFT
		case POSITION_SAFESHIFT-1:
			if (cameraMode.CfSafetyShift) 
				return "On";
			else
				return "Off";
#endif
#ifdef POSITION_AFBUTTON
		case POSITION_AFBUTTON-1:
			switch(cameraMode.CfLensButton){
				case 0: return "AF Stop";
				case 1: return "AF Start";
				case 2: return "AELock";
				case 3: return "AF Point";
				case 4: return "OneShot/AISer.";
				default: return "IS Start";
			}
#endif
#ifdef POSITION_DATA
		case POSITION_DATA-1:
			if (cameraMode.CfOriginalEval) 
				return "On";
			else
				return "Off";
#endif
#ifdef POSITION_FLASH
		case POSITION_FLASH-1:
			if (cameraMode.CfEmitFlash) 
				return "Off";
			else
				return "On";
#endif
#ifdef POSITION_COUNTER
		case POSITION_COUNTER-1:
			return ShutterCountH2S(releaseCount);
#endif
#ifdef POSITION_TLAPSE
		case POSITION_TLAPSE-1:
			sprintf(dynamicstr, "%d x %ds", TLapseFrames, TLapseInterval);
			return dynamicstr;
#endif
#ifdef POSITION_RAWMODE
		case POSITION_RAWMODE-1:
			switch(RawMode) {
				case 1: return "JPEG only";
				case 2: return "Raw only";
				default: return "Raw+JPEG";
			  }
#endif
		default:
			return "";
	}
}

const char* my_GetCurSetString() {
	switch (fn) {
#ifdef POSITION_ISO
		case POSITION_ISO-1: return QUOTE(POSITION_ISO)":ISO";
#endif
#ifdef POSITION_WB
		case POSITION_WB-1: return QUOTE(POSITION_WB)":WB";
#endif
#ifdef POSITION_CT
		case POSITION_CT-1: return QUOTE(POSITION_CT)":CTemp";
#endif
#ifdef POSITION_METERING
		case POSITION_METERING-1: return QUOTE(POSITION_METERING)":Metering";
#endif
#ifdef POSITION_AEB
		case POSITION_AEB-1: return QUOTE(POSITION_AEB)":AEB";
#endif
#ifdef POSITION_BSEQ
		case POSITION_BSEQ-1: return QUOTE(POSITION_BSEQ)":CF-BSeq/AutoC";
#endif
#ifdef POSITION_SI
		case POSITION_SI-1: return QUOTE(POSITION_SI)":CF-SI Disp.";
#endif
#ifdef POSITION_MENU
		case POSITION_MENU-1: return QUOTE(POSITION_MENU)":CF-Menu Posi.";
#endif
#ifdef POSITION_SAFESHIFT
		case POSITION_SAFESHIFT-1: return QUOTE(POSITION_SAFESHIFT)":CF-Safe.Shift";
#endif
#ifdef POSITION_AFBUTTON
		case POSITION_AFBUTTON-1: return QUOTE(POSITION_AFBUTTON)":?CF-L.AFButt";
#endif
#ifdef POSITION_DATA
		case POSITION_DATA-1: return QUOTE(POSITION_DATA)":CF-Decis.Da";
#endif
#ifdef POSITION_FLASH
		case POSITION_FLASH-1: return QUOTE(POSITION_FLASH)":CF-Flash";
#endif
#ifdef POSITION_COUNTER
		case POSITION_COUNTER-1: return QUOTE(POSITION_COUNTER)":Shutt.Count";
#endif
#ifdef POSITION_TLAPSE
		case POSITION_TLAPSE-1: return QUOTE(POSITION_TLAPSE)":TimeLapse";
#endif
#ifdef POSITION_RAWMODE
		case POSITION_RAWMODE-1: return QUOTE(POSITION_RAWMODE)":Raw+JPEG";
#endif
		default: return "?";
	}
}
#endif

void MyTask() {
	int mesmode=3;
	int isRelease=0;
	while (1) {
		int* pMessage = 0;
		ReceiveMessageQueue(hMyTaskMessQue, &pMessage, 0); //	TryReceiveMessageQueue(hMyTaskMessQue, &pMessage, 0);
		if (!pMessage)
			continue;
		switch (pMessage[0]) {
		
#ifdef SHORTCUTS
/*
cameraMode.menupos seems to count the menus beginning with 0x?1 at the top of every page
and 0x0? for the first page

0x01 means first page, first submenu (Quality)
0x13 means second page, third submenu
0x33 means fourth page, third submenu

etc
*/
			case MESSAGE_ZO:									// zoom-out button pressed in ...
				if (handleMainMenuDlg){
					if (isRelease)
						isRelease=0;
					else{
						switch (cameraMode.menupos){
							case 1:								// ...quality menu
								SendToIntercom(0x30,1,cameraMode.CfEmitFlash^1);	//toggle flash on/off
								if(cameraMode.CfEmitFlash)				//flash red led for enabled, blue for disabled flash
									LEDRED=LEDON;
								else
									LEDBLUE=LEDON;
								SleepTask(150);
								LEDRED=LEDOFF,LEDBLUE=LEDOFF;
								break;
							case 4: 							// ...AF menu
								SendToIntercom(0x16, 1, 1);			// set LCD brightness to min
								break;
							case 6:								// ...ISO menu
								eventproc_RiseEvent("SetIsoValue", 0x48);	// set ISO 100
								eventproc_RiseEvent("PressMenuButton", 0);	// close menu and
								eventproc_RiseEvent("PressSetButton", 0);	// open menu again to show new setting
								SHORTCUTSIGNAL
								break;
							case 0x13:							// ...WB menu
								SendToIntercom(5, 1, 0);			// set AWB
								eventproc_RiseEvent("PressMenuButton", 0);	// close menu and
								eventproc_RiseEvent("PressSetButton", 0);	// open menu again to show new setting
								SHORTCUTSIGNAL
						}
			                	isRelease=1;
					}
				}
				break;
			case MESSAGE_ZI:									// zoom-in button pressed in ...
		                if (handleMainMenuDlg) {
					if(isRelease)
						isRelease=0;
					else{
						switch (cameraMode.menupos){
							case 1:								// ...quality menu
								eventproc_RiseEvent("PressMenuButton", 0);	//go to parameter menu
								eventproc_RiseEvent("PressJumpButton", 0);
								eventproc_RiseEvent("PressUpButton", 0);
								eventproc_RiseEvent("PressSetButton", 0);
								break;
							case 4:								// ...AF menu
								SendToIntercom(0x16, 1, 5);			// set LCD brightness to max
								break;
							case 5:								// ...metering menu
								eventproc_SetMesMode(&mesmode);			// Spot metering mode
								SHORTCUTSIGNAL
								break;
							case 6:								// ...ISO menu
								SendToIntercom(0x2a + 8 - 1, 1, 1);		// enable CFn.8 for ISO H
								eventproc_RiseEvent("SetIsoValue", 0x6f);	// set ISO 3200
								SHORTCUTSIGNAL
								break;
							case 0x13:							// ...WB menu
								SendToIntercom(5, 1, 8);		 	//set WB ColorTemp
								SHORTCUTSIGNAL
						}
					isRelease=1;
					}
				}
				break;
			case MESSAGE_DP:									//direct print button pressed in ...
				if (handleMainMenuDlg) {
					switch (cameraMode.menupos){
						case 1:									// ...quality menu
							SendToIntercom(0x39,1,cameraMode.CfSafetyShift^1);	//toggle safety shift
							if(cameraMode.CfSafetyShift)				//flash red led for enabled, blue for disabled SS
								LEDBLUE=LEDON;
							else
								LEDRED=LEDON;
							SleepTask(150);
							LEDRED=LEDOFF,LEDBLUE=LEDOFF;
							break;
						case 4:									// ...AF menu
							SendToIntercom(0x16,1,3);				//set LCD brightness to 3
							break;
						case 5:									// ...metering menu
							SendToIntercom(0x35,1,cameraMode.CfMirrorUpLock^1);	//toggle mirror lock-up
							if(cameraMode.CfMirrorUpLock)				//flash red led for enabled, blue for disabled MLU
								LEDBLUE=LEDON;
							else
								LEDRED=LEDON;
							SleepTask(150);
							LEDRED=LEDOFF,LEDBLUE=LEDOFF;
							break;
						case 6:									// ...ISO menu
							eventproc_RiseEvent("SetIsoValue", 0x58);		//set ISO 400
							eventproc_RiseEvent("PressMenuButton", 0);		//close menu and
							eventproc_RiseEvent("PressSetButton", 0);		//open menu again to show new setting
							SHORTCUTSIGNAL
							break;
						case 0x13:								// ...WB menu
							SendToIntercom(5,1,6);					// Set WB to manual using last taken picture as reference
							eventproc_RiseEvent("PressMenuButton",0);	// go from wb submenu to menu page 2
							eventproc_RiseEvent("PressDownButton",0);
							eventproc_RiseEvent("PressDownButton",0);	//go down to manual wb image selection
							eventproc_RiseEvent("PressSetButton",0);
							eventproc_RiseEvent("PressSetButton",0);	//select last image on card
							eventproc_RiseEvent("PressUpButton",0);
							eventproc_RiseEvent("PressUpButton",0);
							eventproc_RiseEvent("PressSetButton",0);	//go back to WB menu
							SHORTCUTSIGNAL
					}
				}
				break;
#endif

			case MESSAGE_UD:
				if (isRelease)
					isRelease=0;
				else{
					switch(pMessage[1]){
						case BUTTON_DOWN:
							if(fn)
								fn--;
							else
								fn=POSITION_LAST-1;
							SendToIntercom(0xd,1,cameraMode.Aeb);
							break;
						case BUTTON_UP:
							if(fn<POSITION_LAST-1)
								fn++;
							else
								fn=0;
							SendToIntercom(0xd,1,cameraMode.Aeb);
							break;
					}
					isRelease=1;
				}
				break;
			case MESSAGE_LR:
				if (isRelease)
						isRelease=0;
				else{
					switch(fn){
#ifdef POSITION_ISO
						case POSITION_ISO-1:
							SwitchIso(pMessage[1]);
							break;
#endif
#ifdef POSITION_WB
						case POSITION_WB-1:
							SwitchWB(pMessage[1]);
							SendToIntercom(0xd,1,cameraMode.Aeb);
							break;
#endif
#ifdef POSITION_CT
						case POSITION_CT-1:
							SwitchColorTemp(pMessage[1]);
							SendToIntercom(0xd,1,cameraMode.Aeb);
							break;
#endif
#ifdef POSITION_METERING
						case POSITION_METERING-1:
							SwitchMetering(pMessage[1]);
							SendToIntercom(0xd,1,cameraMode.Aeb);
							break;
#endif
#ifdef POSITION_AEB
						case POSITION_AEB-1:
							SwitchAeb(pMessage[1]);
							break;
#endif
#ifdef POSITION_BSEQ
						case POSITION_BSEQ-1: // BSeq/AutoC
							SendToIntercom(0x32, 1, CfnVal(pMessage[1],cameraMode.CfAebSeq,3,0));
							SendToIntercom(0xd,1,cameraMode.Aeb);
							break;
#endif
#ifdef POSITION_SI
						case POSITION_SI-1: //Si display
							SendToIntercom(0x33, 1, ToggleCfnVal(pMessage[1],cameraMode.CfSiIndicate));
							SendToIntercom(0xd,1,cameraMode.Aeb);
							break;
#endif
#ifdef POSITION_MENU
						case POSITION_MENU-1: //menu Position
							SendToIntercom(0x34, 1, CfnVal(pMessage[1],cameraMode.CfMenuPos,2,0));
							SendToIntercom(0xd,1,cameraMode.Aeb);
							break;
#endif
#ifdef POSITION_SAFESHIFT
						case POSITION_SAFESHIFT-1: //Safety Shift
							SendToIntercom(0x39, 1, ToggleCfnVal(pMessage[1],cameraMode.CfSafetyShift));
							SendToIntercom(0xd,1,cameraMode.Aeb);
							break;
#endif
#ifdef POSITION_AFBUTTON
						case POSITION_AFBUTTON-1: //Lens AF Button
							SendToIntercom(0x3a, 1, CfnVal(pMessage[1],cameraMode.CfLensButton,5,0));
							SendToIntercom(0xd,1,cameraMode.Aeb);
							break;
#endif
#ifdef POSITION_DATA
						case POSITION_DATA-1: //  Decision Data
							SendToIntercom(0x3b, 1, ToggleCfnVal(pMessage[1],cameraMode.CfOriginalEval));
							SendToIntercom(0xd,1,cameraMode.Aeb);
							break;
#endif
#ifdef POSITION_FLASH
						case POSITION_FLASH-1: //  Flash
							SendToIntercom(0x30, 1, ToggleCfnVal(pMessage[1],cameraMode.CfEmitFlash));
							SendToIntercom(0xd,1,cameraMode.Aeb);
							break;
#endif
#ifdef POSITION_TLAPSE
						case POSITION_TLAPSE-1: // Time-lapse
							SwitchTimeLapse(pMessage[1]);
							SendToIntercom(0xd,1,cameraMode.Aeb);
							break;
#endif
#ifdef POSITION_RAWMODE
						case POSITION_RAWMODE-1: // Raw toggle
							SwitchRawMode(pMessage[1]);
							SendToIntercom(0xd,1,cameraMode.Aeb);
							break;
#endif
						default:
							break;
					}
					isRelease=1;
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
    switch (ptr[1]) {
		case BUTTON_UP:
		case BUTTON_DOWN:
			if (handleInfoCreativeDlg) {
				SendMyMessage(MESSAGE_UD, ptr[1]);
				return;
			}
			break;
		case BUTTON_LEFT:
		case BUTTON_RIGHT:
		case BUTTON_SET:
#ifdef POSITION_TLAPSE
		case BUTTON_MENU:
		case BUTTON_INFO:
		case BUTTON_PLAY:
#endif
			if (handleInfoCreativeDlg) {
				SendMyMessage(MESSAGE_LR, ptr[1]);
				return;
			}
			break;
		case BUTTON_ZO:
			SendMyMessage(MESSAGE_ZO, ptr[1]);
			break;
		case BUTTON_DP:
			SendMyMessage(MESSAGE_DP, ptr[1]);
			break;
		case BUTTON_ZI:
			SendMyMessage(MESSAGE_ZI, ptr[1]);
			break;
	}

	IntercomHandler(r0, ptr);
}
