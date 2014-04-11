/*
 * EOS 350D mod project
 * Original work by maiohwmai and cyrustam
 * Further mods by crazyklaus, MilanC and zestoi
 * Modded by GezuS on 2014-01-14 : 
 * "Black&White 0,0,N,N" shortcut and "Parameter 3" shortcut in AF menu 
 * "POSITION_BW" added in Custom menu to switch between "B/W" and "Parameter 3"
 * "POSITION_NOISE" added in Custom menu to enable/disable Automatic Noise Reduction
 * Added "POSITION_BW" and "POSITION_NOISE" to preset config to have them load/save with presets
 * Added Function to switch LCD brightness level 1 to 5 (AF menu, then press direct-print button multiple times) 
 */

#include "headers.h"
#include "user_settings.txt"

typedef struct {
	int AEMode;					// 0x00
	int MeteringMode;			// 0x04
	int FlashExComp;			// 0x08
	int DriveMode;				// 0x0c
	int WB;						// 0x10
	int AF;						// 0x14
	int AfPoint;				// 0x18
	int TvVal;					// 0x1c
	int AvVal;					// 0x20
	int AvComp;					// 0x24
	int Iso;					// 0x28
	int RedEyeRed;  			// 0x2c
	int Aeb;					// 0x30
	int WbBracketing;			// 0x34
	int Beep;					// 0x38
	int ColorTemp;				// 0x3c
	int IsoExpand;				// 0x40
	int AutoPowerOff;			// 0x44
	int ViewType;				// 0x48		what does this mean?
	int ReviewTime;				// 0x4c
	int AutoRotate;				// 0x50
	int LcdBrightness;			// 0x54
	int DateTime;				// 0x58
	int FileNumbering;			// 0x5c
	int Language;				// 0x60
	int VideoSystem;			// 0x64
	int Communication;			// 0x68
	int Parameter;				// 0x6c
	int ParaSetOne;				// 0x70
	int ParaSetTwo;				// 0x74
	int ParaSetThree;			// 0x78
	int ParaSetBW;				// 0x7c
	int ColorSpace;				// 0x80
	int QualityRaw;				// 0x84
	int QualitySize;			// 0x88
	int QualityFine;			// 0x8c
	int MenuPosCf;				// 0x90
	int MenuPos;				// 0x94
	int WBCompGM;				// 0x98
	int WBCompAB;				// 0x9c
	int ShootWithoutCard;		// 0xa0
	int CfSetButton;			// 0xa4
	int CfNoiseReduction;		// 0xa8
	int CfFlashSyncTimeAv;		// 0xac
	int CfShutterAeButton;		// 0xb0
	int CfAfAssistBeam;			// 0xb4
	int CfSettingSteps;			// 0xb8
	int CfEmitFlash;			// 0xbc
	int CfExtendIso;			// 0xc0 
	int CfAebSeq;				// 0xc4
	int CfSiIndicate;			// 0xc8
	int CfMenuPos;				// 0xcc
	int CfMLU;					// 0xd0
	int CfAfpSelection;			// 0xd4
	int CfFlashMetering;		// 0xd8
	int CfFlashSyncRear;		// 0xdc
	int CfSafetyShift;			// 0xe0
	int CfLensButton;			// 0xe4
	int CfOriginalEval;			// 0xe8
	int BatteryLevel; 			// 0xec
	int BatteryType; 			// 0xf0
	int PrintUserProperty;		// 0xf4		what does this mean?
	int PrinterProperty;		// 0xf8		and this?
	int ClusterSize;			// 0xfc
	int RemainCluster;			// 0x100
	int FileID;					// 0x104
	int LensMinAv;				// 0x108
	int LensMaxAv;				// 0x10c
	int EfLens;					// 0x110
	int AvailShot;				// 0x114
	int QrevImgProp;			// 0x118	and this?
	int PrinterConnectType;		// 0x11c
	int MWDatRed;				// 0x120	this
	int MWDatGreen1;			// 0x124	this too
	int MWDatGreen2;			// 0x128	also
	int MWDatBlue;				// 0x12c	yeah...
	int VideoJackConnect;		// 0x130
	int IncompImg;      		// 0x134	and this
	int BusyFlag;       		// 0x138
} type_CAMERA_MODE;


int  fn=0;
int* hMyTaskMessQue;
int  itmp=0;
int  itmp2=0;
int  itmp3=0;

#ifdef POSITION_PRESETS
int  buff2[35];
char *buff = (char*)buff2;
#else
char buff[18];
int  buff2[6];
#endif

char buff3[5]; // only used by FecStr()

// variables that are only needed for some menu options declared in user_settings.txt

#define cameraMode				(*((type_CAMERA_MODE*)0x142b8))		// CameraMode struct
#define releaseCount			(*((int*)(0xe7e0 + 0x0c)))
#define isInFactoryMode			(*((int*)0x15284))			// != 0 when in factory mode
#define isBeepInit				(*((int*)0x398c))
#define isDigitalGainEnabled	(*((int*)0x3df8))
#define handleMainMenuDlg		(*((int*)0x22ac))			// Main menu Dialog handle
#define handleMainMenuDlg2		(*((int*)0x22bc))			// Main menu Dialog 2 handle
#define handleSubMenuDlg		(*((int*)0x22b8))			// Sub menu Dialog handle
#define handleInfoCreativeDlg	(*((int*)0x2304))			// Info creative Dialog handle
#define MESSAGE_DP				0x01
#define MESSAGE_ZI				0x04
#define MESSAGE_AEB				0x05
#define MESSAGE_UD				0x06
#define MESSAGE_LR				0x07
#define MESSAGE_ZO				0x08
#define	BUTTON_MENU				0xa0
#define	BUTTON_INFO				0xa1
//#define BUTTON_JUMP			0xa2 ?
#define	BUTTON_PLAY				0xa4
//#define BUTTON_TRASH			0xa5 ?
#define	BUTTON_SET				0xa6
#define	BUTTON_ZI				0xa7
#define	BUTTON_ZO				0xa8
#define	BUTTON_UP				0xb1
#define	BUTTON_DOWN				0xb2
#define	BUTTON_RIGHT			0xb3
#define	BUTTON_LEFT				0xb4
#define	BUTTON_DP				0xb8

#define QUOTE_(x) #x		//macros I need to get the numbers in the menu right in english version
#define QUOTE(x) QUOTE_(x)	//(I skipped the numbers in the german translation to have more space)

#ifdef NOSETTINGSFILE
void ReadSettings(){}
void WriteSettings(){}
#else
void ReadSettings(){
	int settings = FIO_OpenFile("A:/config/settings", O_RDONLY, 644);
	if(settings != -1) {
		itmp = FIO_ReadFile(settings, (char *)buff2, sizeof(int)*5);
		#ifdef POSITION_AFPOINTS_1
		AfPointSet1 = buff2[0];
		#endif
		#ifdef POSITION_AFPOINTS_2
		AfPointSet2 = buff2[1];
		#endif
		#ifdef POSITION_AFPOINTS_3
		AfPointSet3 = buff2[2];
		#endif
		#ifdef POSITION_TLAPSE
		TLapseInterval = buff2[3];
		TLapseFrames = buff2[4];
		#endif
		FIO_CloseFile(settings);
	}
}

void WriteSettings(){
	int settings = FIO_OpenFile("A:/config/settings", O_CREAT|O_WRONLY , 644);
	if(settings != -1) {
		#ifdef POSITION_AFPOINTS_1
		buff2[0]= AfPointSet1;
		#endif
		#ifdef POSITION_AFPOINTS_2
		buff2[1]= AfPointSet2;
		#endif
		#ifdef POSITION_AFPOINTS_3
		buff2[2]= AfPointSet3;
		#endif
		#ifdef POSITION_TLAPSE
		buff2[3] = TLapseInterval;
		buff2[4] = TLapseFrames;
		#endif
		FIO_WriteFile(settings, (char*)buff2, sizeof(int)*5);
		FIO_CloseFile(settings);
	}
}
#endif

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
	eventproc_SetIsoValue(&isoVal);
	LEDSOFF
}
#endif

#ifdef POSITION_AEB
void SwitchAeb(int button) {
	int aebVal = cameraMode.Aeb;
	LEDSON
	switch (button) {
		case BUTTON_LEFT:
			if (aebVal)
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
			ColorTempVal-=100;	
			break;
		case BUTTON_RIGHT:
			ColorTempVal+=100;
          }
        if(ColorTempVal>11000)
		ColorTempVal=1800;
        if(ColorTempVal<1800)
		ColorTempVal=11000;
        SendToIntercom(0x10, 2, ColorTempVal);
        LEDSOFF
}
#endif

#ifdef POSITION_TLAPSE
void SwitchTimeLapse(int button) {
	int i, OldLcdBrightness, OldReviewTime;
	switch (button) {
#ifdef USEPLAYFORSET
		case BUTTON_PLAY:
#else
		case BUTTON_SET:
#endif
			for (OldLcdBrightness=cameraMode.LcdBrightness,		//save brightness and review time values
			OldReviewTime=cameraMode.ReviewTime,
			cameraMode.ReviewTime=0xff,							//set brightness to min and review time to hold
			SendToIntercom(0x16,1,1),
			SendToIntercom(0x53,0,0),
			i=1; i<TLapseFrames; i++) {
				SleepTask(TLapseInterval*1000);
				SendToIntercom(0x53,0,0);
			}
			cameraMode.ReviewTime=OldReviewTime;				//restore brightness and review time settings when timelapse sequence is done
			SendToIntercom(0x16,1,OldLcdBrightness);
			SHORTCUTSIGNAL
			break;

		case BUTTON_LEFT:
			if (TLapseInterval>1) TLapseInterval--;
			break;

		case BUTTON_RIGHT:
			TLapseInterval++;
			break;

		case BUTTON_MENU:
			TLapseFrames *= 2;
			break;

		case BUTTON_INFO:
			if (TLapseFrames>1) TLapseFrames /= 2;
			break;

#ifdef USEPLAYFORSET
		case BUTTON_SET:
#else
		case BUTTON_PLAY:
#endif
			TLapseFrames++;
			break;

	  }
}
#endif

#ifdef POSITION_TLAPSERAMP
//
// not completed as unable to fire the shutter when in bulb mode
//
void SwitchTimeLapseRamp(int button) {
	switch (button) {
		case BUTTON_MENU:
			if (TLapseRLevel < 4) TLapseRLevel++;
			else TLapseRLevel = -4;
			break;

		case BUTTON_PLAY:
			if (TLapseRMax < 10) TLapseRMax++;
			else TLapseRMax = 1;
			break;

		case BUTTON_LEFT:
			if (TLapseRFrames > 100) TLapseRFrames -= 100;
			break;

		case BUTTON_RIGHT:
			if (TLapseRFrames < 1000) TLapseRFrames += 100;
			break;
	}
}
#endif

#ifdef POSITION_RAWMODE
void SwitchRawMode(int button) {
	switch (button) {
		case BUTTON_LEFT:
			RawMode = (cameraMode.QualityRaw % 3) + 1;
			break;

		case BUTTON_SET:
			JpgQual = (cameraMode.QualityFine) ^= 1;
			break;

		case BUTTON_RIGHT:
			JpgSize = (cameraMode.QualitySize + 1) % 3;
	}

	SendToIntercom(0x22, 1, RawMode);
	SendToIntercom(0x24, 1, JpgQual);
	SendToIntercom(0x23, 1, JpgSize);
}
#endif

#if defined(POSITION_AFPOINTS_1)||defined(POSITION_AFPOINTS_2)||defined(POSITION_AFPOINTS_3)
int cursor;
void SetAFPoints(int button, unsigned int * afpoints)
	{
	switch(button) {
		case BUTTON_LEFT:
			if (cursor)
				cursor--;
			else
				cursor=6;
			break;
		case BUTTON_RIGHT:
			cursor=(cursor+1)%7;
			break;
#ifdef USEPLAYFORSET
		case BUTTON_PLAY:
#else
		case BUTTON_SET:
#endif
			switch(cursor) {
				case 0:
					*afpoints^=4;
					break;
				case 1:
					*afpoints^=0x10;
					break;
				case 2:
					*afpoints^=0x40;
					break;
				case 3:
					*afpoints^=1;
					break;
				case 4:
					*afpoints^=0x20;
					break;
				case 5:
					*afpoints^=8;
					break;
				case 6:
					*afpoints^=2;
					break;
				}
		}
	}

char* AfPointStr(unsigned int afpoints)
	{
	ReadSettings();
	buff[7]=0;
	
	if(afpoints&4)	buff[0]=0x54;
	else	buff[0]=0x20;
		
	if(afpoints&0x10)	buff[1]=0x4C;
	else	buff[1]=0x20;
		
	if(afpoints&0x40)	buff[2]=0x4C;
	else	buff[2]=0x20;
		
	if(afpoints&1)	buff[3]=0x43;
	else	buff[3]=0x20;
		
	if(afpoints&0x20)	buff[4]=0x52;
	else	buff[4]=0x20;
		
	if(afpoints&8)	buff[5]=0x52;
	else	buff[5]=0x20;

	if(afpoints&2)	buff[6]=0x42;
	else	buff[6]=0x20;
				
	buff[cursor]+=0x20;		//highlight cursor position, changes uppercase characters to lowercase and space to @
	return buff;
	}
#endif

#ifdef POSITION_TEST
int testcounter=0;
int outputcounter=0;
char* testoutput() {
//	time_t testint;
//	time(&testint);
		sprintf(buff, "%u %d%d%d%d %X %X", cameraMode.AEMode, cameraMode.Language, cameraMode.BatteryLevel, cameraMode.BatteryType, cameraMode.EfLens, cameraMode.LensMinAv, cameraMode.LensMaxAv);
	return buff;
}

char* displayCameraMode(int counter) {
	int *startaddr;
	startaddr=&cameraMode.AEMode + counter;
	sprintf(buff, "%X - %X", (startaddr - &cameraMode.AEMode), (startaddr + 3*sizeof(int) - &cameraMode.AEMode) );
	return buff;
}

char* displayCameraModeValues(int counter) {
	int *startaddr;
	startaddr=&cameraMode.AEMode + counter;
	sprintf(buff,"%X %X %X",*startaddr,*(startaddr+1),startaddr[2]);
	return buff;
}

void WriteMoreStuff() {
sprintf(buff, "A:/output%u%u.txt",outputcounter/10, outputcounter++);
int outputfile = FIO_OpenFile(buff, O_CREAT|O_WRONLY , 644);
if(outputfile!=-1) {
	FIO_WriteFile(outputfile,&cameraMode.AEMode,0x100*sizeof(int));
	FIO_CloseFile(outputfile);
	}
}
#endif

#ifdef POSITION_REVIEWTIME
int ChangeReviewTime(int button) {
	int temp;
	if (cameraMode.ReviewTime==0xff)
		temp=-1;
	else
		temp=cameraMode.ReviewTime;
	temp=CfnVal(button,temp,REVIEWTIMEMAX,-1);
	if (temp==-1)
		temp=0xff;
	return temp;
}
#endif

#if defined(POSITION_FLASHEXCOMP)||defined(POSITION_FEB)||defined(POSITION_AVCOMP)||defined(POSITION_EAEB)
int FecHex(int setting) {			//setting goes from 0 to 48, one value for every possible flashexcomp value if I counted right (-6, -5.7, -5.5, -5.3, -5 ... 0 ... +6)
	int temp1,temp2, temp3=0;
	
	temp1=setting/8;						//temp1 will be 0 for -6, 1 for -4, 2 for -2, 3 for 0 ... 6 for +6
	temp2=setting%8;						//temp2 will be 0 for -6, 1 for -5.7, 2 for -5.5, 3 for -5.3, 4 for -5, 5 for -4.7 ... 0 for -4 ...
	
	switch(temp1) {							//now create the hex value, upper byte
		case 0:
		case 1:
		case 2:	temp3=0xd0+(temp1*0x10);	//temp3 will be 0xd0 for temp1==0, 0xe0 for temp1==1, 0xf0 for temp1==2
				break;
		case 4:
		case 5:
		case 6:	temp3=(temp1-3)*0x10;		//temp3 will be 0x10 for temp1==4, 0x20 for temp1==5, 0x30 for temp1==6
	}
	switch(temp2) {							//lower byte
		case 1:
		case 2:
		case 3: temp3|=2+temp2;				//temp3 lower byte will be 3 for temp2==1, 4 for temp2==2, 5 for temp2==3
				break;
		case 4: temp3|=8;
				break;
		case 5:
		case 6:
		case 7: temp3|=6+temp2;				//temp3 lower byte will be b for temp2==5, c for temp2==6, d for temp2==7
	}
	
	return temp3;							//this should be it PS: YEEEEHA, it works!
}

int HexFec(int hex) {
	int i=0;

	unsigned short map[] = {
		0xd0, 0xd3, 0xd4, 0xd5, 0xd8, 0xdb, 0xdc, 0xdd, 0xe0, 0xe3, 
		0xe4, 0xe5, 0xe8, 0xeb, 0xec, 0xed, 0xf0, 0xf3, 0xf4, 0xf5, 
		0xf8, 0xfb, 0xfc, 0xfd, 0x00, 0x03, 0x04, 0x05, 0x08, 0x0b, 
		0x0c, 0x0d, 0x10, 0x13, 0x14, 0x15, 0x18, 0x1b, 0x1c, 0x1d, 
		0x20, 0x23, 0x24, 0x25, 0x28, 0x2b, 0x2c, 0x2d, 0x30
	};

	for (i=0; i<=48; i++) {
		if (map[i] == hex) {
			return i;
		}
	}

	return 0;
}

char *FecStr(int setting, int prefix) {
	int neg = 0;

	unsigned char *map[] = { 
		"0.0",
		"0.3", "0.5", "0.7", "1.0", "1.3", "1.5", "1.7", "2.0", 
		"2.3", "2.5", "2.7", "3.0", "3.3", "3.5", "3.7", "4.0",
		"4.3", "4,5", "4.7", "5.0", "5.3", "5.5", "5.7", "6.0",
		"6.3", "6.5", "6.7"
	};

	setting -= 24;

	if (setting < 0) {
		neg = 1;
		setting *= -1;
	}

	if (setting >= 0 && setting <= 27) {
		if (prefix) {
			sprintf(buff3, "%c%s", neg ? '-' : '+', map[setting]);
			return buff3;
		}
		else {
			return map[setting];
		}
	}
	else {
		return "?";
	}
}
#endif

#ifdef POSITION_FEB
void SwitchFeb(int button) {

	int FebOrgReviewTime, FebOrgAv, tmpidx, i;

	switch(button) {
		case BUTTON_SET:

			// save old settings
			FebOrgReviewTime = cameraMode.ReviewTime;
			FebOrgAv = cameraMode.FlashExComp;
			cameraMode.ReviewTime = 0xff;

			// delay 2 seconds to start
			for (i=0; i<4; i++) {
				LEDBLUE=LEDON;
				SleepTask(250);
				LEDBLUE=LEDOFF;
				SleepTask(250);
			}

			SendToIntercom(0x53,0,0);
			SleepTask(3000); //delay for system to save picture

			i = 0;
			while (i < (FebFrames-1)/2) {

				tmpidx = HexFec(FebOrgAv)+(FebELevel*(i+1));
				if (tmpidx > 48) tmpidx = 48;

				SendToIntercom(0x3, 1, FecHex(tmpidx) ); 
				SendToIntercom(0x53, 0, 0);
				SleepTask(5000); //delay for system to save picture

				tmpidx = HexFec(FebOrgAv)-(FebELevel*(i+1));
				if (tmpidx < 0) tmpidx = 0;

				SendToIntercom(0x3, 1, FecHex(tmpidx) ); 
				SendToIntercom(0x53, 0, 0);
				SleepTask(5000); //delay for system to save picture
				i++;

			}
		
			// restore settings
			cameraMode.ReviewTime = FebOrgReviewTime;
			SendToIntercom(0x3, 1, FebOrgAv);
			SHORTCUTSIGNAL
			break;

		case BUTTON_MENU:
			if (FebFrames <= 41) FebFrames += 2;
			break;

		case BUTTON_PLAY:
			if (FebFrames > 3) FebFrames -= 2;
			break;

		case BUTTON_RIGHT:
			if (FebELevel < 24) FebELevel += 1;
			break;

		case BUTTON_LEFT:
			if (FebELevel > 1) FebELevel -= 1;
			break;
	}
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

char* my_GetAEBString(int aebVal) {
	sprintf(buff, "%s +- %s", FecStr(HexFec(cameraMode.AvComp), 1), FecStr(HexFec(cameraMode.Aeb), 0));
	return buff;
}

/*
char* my_GetAEBString(int aebVal) {
	int avone=0, avtwo=0, aebone=0, aebtwo=0, temp;
	if (cameraMode.AvComp < 0xd0) {
		temp=cameraMode.AvComp;
		buff[0]='+';
		}
	else {
		temp=(0x100-cameraMode.AvComp);
		buff[0]='-';
		}
		switch(temp&0xf0){
			case 0x10:
				avone=2;
				break;
			case 0x20:
				avone=4;
				break;
			case 0x30:
				avone=6;
		}
		if(temp&0x08)
			avone++;
		switch(temp&0x07){
			case 3:
				avtwo=3;
				break;
			case 4:
				avtwo=5;
				break;
			case 5:
				avtwo=7;
		}
	switch(cameraMode.Aeb&0xf0){
	   case 0x10:
	      aebone=2;
	      break;
	   case 0x20:
	      aebone=4;
	      break;
	   case 0x30:
	      aebone=6;
	}
	if((cameraMode.Aeb&0x08)==8)
		aebone++;
	switch(cameraMode.Aeb&0x07){
	   case 3:
	      aebtwo=3;
	      break;
	   case 4:
	      aebtwo=5;
	      break;
	   case 5:
	      aebtwo=7;
	}
	sprintf(buff+1,"%u.%u +- %u.%u", avone, avtwo, aebone,aebtwo);
	return buff;
}
*/

#if defined(POSITION_CONTRAST)||defined(POSITION_SHARPNESS)||defined(POSITION_SATURATION)||defined(POSITION_COLORTONE)
char *GetContrastString(int val) {
	if (val > 128) val = (256 - val) * -1;
	sprintf(buff, "%d", val);
	return buff;
}
#endif

//
// adjust contrast
//

#ifdef POSITION_CONTRAST
void SwitchContrast(int button) {
	int TmpVal = cameraMode.ParaSetThree;
	int TmpVal2 = cameraMode.ParaSetThree&0xff;

	itmp = TmpVal2;
	if (itmp > 128) itmp = (256 - itmp) * -1;

	LEDBLUE = LEDON;
	switch (button) {
		case BUTTON_LEFT: 
			if (itmp > -2) {
				if(TmpVal2==0x00) 
					SendToIntercom(0x1f, 4, ( (cameraMode.ParaSetThree&0xff000000)+(cameraMode.ParaSetThree&0xff0000)+(cameraMode.ParaSetThree&0xff00)+0xff ) );
				else 
					SendToIntercom(0x1f, 4, ( (cameraMode.ParaSetThree&0xff000000)+(cameraMode.ParaSetThree&0xff0000)+(cameraMode.ParaSetThree&0xff00)+((unsigned)(cameraMode.ParaSetThree&0xff)-0x01) ) );
			}
			break;

		case BUTTON_RIGHT:
			if (itmp < 2) {
				if(TmpVal2==0xff) 
					SendToIntercom(0x1f, 4, ( (cameraMode.ParaSetThree&0xff000000)+(cameraMode.ParaSetThree&0xff0000)+(cameraMode.ParaSetThree&0xff00)+0x00 ) );
				else
					SendToIntercom(0x1f, 4, ( (cameraMode.ParaSetThree&0xff000000)+(cameraMode.ParaSetThree&0xff0000)+(cameraMode.ParaSetThree&0xff00)+((unsigned)(cameraMode.ParaSetThree&0xff)+0x01) ) );
			}
			break;
	}

	LEDBLUE = LEDOFF;
}
#endif

//
// adjust sharpness
//

#ifdef POSITION_SHARPNESS
void SwitchSharpness(int button) {
	int TmpVal = cameraMode.ParaSetThree;
	int TmpVal2 = (cameraMode.ParaSetThree&0xff00)>>8;

	itmp = TmpVal2;
	if (itmp > 128) itmp = (256 - itmp) * -1;

	LEDBLUE = LEDON;
	switch (button) {
		case BUTTON_LEFT:
			if (itmp > -9) {
				if(TmpVal2==0x00) {
					SendToIntercom(0x1f, 4, ( (cameraMode.ParaSetThree&0xff000000)+(cameraMode.ParaSetThree&0xff0000)+(0xff00)+(cameraMode.ParaSetThree&0xff) ) );
				} 
				else {
					SendToIntercom(0x1f, 4, ( (cameraMode.ParaSetThree&0xff000000)+(cameraMode.ParaSetThree&0xff0000)+((unsigned)(cameraMode.ParaSetThree&0xff00)-0x0100)+(cameraMode.ParaSetThree&0xff) ) );
				}
			}
			break;

		case BUTTON_RIGHT:
			if (itmp < 9) {
				if(TmpVal2==0xff) {
					SendToIntercom(0x1f, 4, ( (cameraMode.ParaSetThree&0xff000000)+(cameraMode.ParaSetThree&0xff0000)+(0x0000)+(cameraMode.ParaSetThree&0xff) ) );
				} 
				else {
					SendToIntercom(0x1f, 4, ( (cameraMode.ParaSetThree&0xff000000)+(cameraMode.ParaSetThree&0xff0000)+((unsigned)(cameraMode.ParaSetThree&0xff00)+0x0100)+(cameraMode.ParaSetThree&0xff) ) );
				}
			}
			break;
	}

	LEDBLUE = LEDOFF;
}
#endif

//
// adjust saturation
//

#ifdef POSITION_SATURATION
void SwitchSaturation(int button) {
	int TmpVal = cameraMode.ParaSetThree;
	int TmpVal2 = (cameraMode.ParaSetThree&0xff0000)>>16;

	itmp = TmpVal2;
	if (itmp > 128) itmp = (256 - itmp) * -1;

	LEDBLUE = LEDON;

	switch (button) {
		case BUTTON_LEFT:
			if (itmp > -2) {
				if(TmpVal2==0x00) {
					SendToIntercom(0x1f, 4, ( (cameraMode.ParaSetThree&0xff000000)+(0xff0000)+(cameraMode.ParaSetThree&0xff00)+(cameraMode.ParaSetThree&0xff) ) );
				} 
				else {
					SendToIntercom(0x1f, 4, ( (cameraMode.ParaSetThree&0xff000000)+((unsigned)(cameraMode.ParaSetThree&0xff0000)-0x010000)+(cameraMode.ParaSetThree&0xff00)+(cameraMode.ParaSetThree&0xff) ) );
				}
			}
			break;

		case BUTTON_RIGHT:
			if (itmp < 2) {
				if(TmpVal2==0xff) {
					SendToIntercom(0x1f, 4, ( (cameraMode.ParaSetThree&0xff000000)+(0x000000)+(cameraMode.ParaSetThree&0xff00)+(cameraMode.ParaSetThree&0xff) ) );
				} 
				else {
					SendToIntercom(0x1f, 4, ( (cameraMode.ParaSetThree&0xff000000)+((unsigned)(cameraMode.ParaSetThree&0xff0000)+0x010000)+(cameraMode.ParaSetThree&0xff00)+(cameraMode.ParaSetThree&0xff) ) );
				}
			}
			break;
	}

	LEDBLUE = LEDOFF;
}
#endif

//
// adjust color tone
//

#ifdef POSITION_COLORTONE
void SwitchColorTone(int button) {
	int TmpVal = cameraMode.ParaSetThree;
	int TmpVal2 = (cameraMode.ParaSetThree&0xff000000)>>24;

	itmp = TmpVal2;
	if (itmp > 128) itmp = (256 - itmp) * -1;

	LEDBLUE = LEDON;

	switch (button) {
		case BUTTON_LEFT:
			if (itmp > -9) {
				if(TmpVal2==0x00) {
					SendToIntercom(0x1f, 4, ( (0xff000000)+(cameraMode.ParaSetThree&0xff0000)+(cameraMode.ParaSetThree&0xff00)+(cameraMode.ParaSetThree&0xff) ) );
				} 
				else {
					SendToIntercom(0x1f, 4, ( ((unsigned)(cameraMode.ParaSetThree&0xff000000)-0x01000000)+(cameraMode.ParaSetThree&0xff0000)+(cameraMode.ParaSetThree&0xff00)+(cameraMode.ParaSetThree&0xff) ) );
				}
			}
			break;

		case BUTTON_RIGHT:
			if (itmp < 9) {
				if(TmpVal2==0xff) {
					SendToIntercom(0x1f, 4, ( (0x00000000)+(cameraMode.ParaSetThree&0xff0000)+(cameraMode.ParaSetThree&0xff00)+(cameraMode.ParaSetThree&0xff) ) );
				} 
				else {
					SendToIntercom(0x1f, 4, ( ((unsigned)(cameraMode.ParaSetThree&0xff000000)+0x01000000)+(cameraMode.ParaSetThree&0xff0000)+(cameraMode.ParaSetThree&0xff00)+(cameraMode.ParaSetThree&0xff) ) );
				}
			}
			break;
	}

	LEDBLUE = LEDOFF;
}
#endif

#ifdef POSITION_PRESETS
char *GetPresetName(char *def) {
#ifdef PRESET_EDITOR
	if (EditorActive) {
		// copy current buffer and hilite selected char
		sprintf(buff, "%13s", EditorBuf);
		buff[EditorPos] = EditorAlphaSel[EditorChar];
	}
	else {
#endif
		sprintf((char*)buff, "A:/presets/%02d.txt", UserPreset);
		itmp = FIO_OpenFile(buff, O_RDONLY, 644);
		strcpy(buff, "");
		if (itmp != -1) {
			if ((itmp2 = FIO_ReadFile(itmp, (char *)buff, 13))) {
				for (itmp3=0; itmp3<itmp2; itmp3++) {
					if (buff[itmp3] == '\r' || buff[itmp3] == '\n') {
						buff[itmp3] = ' ';
					}
				}
				buff[itmp3] = 0x0;
			}
			FIO_CloseFile(itmp);
		}
		if (strcmp(def, "") && strlen(buff) == 0) {
			sprintf(buff, "A:/presets/%02d", UserPreset);
			itmp = FIO_OpenFile(buff, O_RDONLY, 644);
			sprintf(buff, "%s %d %c", def, UserPreset, itmp == -1 ? ' ' : '*');
			if (itmp != -1) FIO_CloseFile(itmp);
		}
#ifdef PRESET_EDITOR
	}
#endif
	return buff;
}

void SwitchPresets(int button) {
#ifdef PRESET_EDITOR
	if (EditorActive) {
		switch (button) {
			case BUTTON_MENU:
				LEDBLUE = LEDOFF;
				EditorActive = 0;
				break;

			case BUTTON_SET:
				sprintf(buff, "A:/presets/%02d.txt", UserPreset);
				itmp = FIO_OpenFile(buff, O_CREAT|O_WRONLY , 644);
				if (itmp == -1) return;
				LEDBLUE = LEDON;
				FIO_WriteFile(itmp, EditorBuf, 13);
				FIO_CloseFile(itmp);
				LEDBLUE = LEDOFF;
				EditorActive = 0;
				break;

			case BUTTON_LEFT:
				if (EditorPos <= 0)
					EditorPos = 12;
				else
					EditorPos--;
				break;

			case BUTTON_RIGHT:
				if (EditorPos >= 12)
					EditorPos = 0;
				else
					EditorPos++;
				break;

			case BUTTON_UP:
				if (EditorChar >= 39)
					EditorChar = 0;
				else
					EditorChar++;
				EditorBuf[EditorPos] = EditorAlpha[EditorChar];
				break;

			case BUTTON_DOWN:
				if (EditorChar <= 0)
					EditorChar = 39;
				else
					EditorChar--;
				EditorBuf[EditorPos] = EditorAlpha[EditorChar];
				break;
		}
		switch (button) {
			case BUTTON_LEFT:
			case BUTTON_RIGHT:
				for (itmp=0; itmp<37; itmp++) {
					if (EditorAlpha[itmp] == EditorBuf[EditorPos]) {
						EditorChar = itmp;
					}
				}
				break;
		}
	}
	else {
#endif
		switch (button) {
			#ifdef PRESET_EDITOR
			case BUTTON_MENU:
				sprintf(EditorBuf, "%-13s", GetPresetName(""));
				EditorBuf[13] = 0x0;
				EditorActive = 1;
				EditorPos = 0;
				EditorChar = 0;

				for (itmp=0; itmp<37; itmp++) {
					if (EditorAlpha[itmp] == EditorBuf[EditorPos]) {
						EditorChar = itmp;
					}
				}

				LEDBLUE = LEDON;
				break;
			#endif
				
			case BUTTON_LEFT:
				if (UserPreset == 1) 
					UserPreset = MAX_PRESETS;
				else
					UserPreset--;
				break;

			case BUTTON_RIGHT:
				if (UserPreset == MAX_PRESETS) 
					UserPreset = 1;
				else
					UserPreset++;
				break;

			case BUTTON_SET:
				sprintf(buff, "A:/presets/%02d", UserPreset);
				itmp = FIO_OpenFile(buff, O_CREAT|O_WRONLY , 644);
				if (itmp == -1) return;
	
				LEDBLUE = LEDON;
	
				#ifdef POSITION_AFPOINTS_1
					buff2[0] = AfPointSet1;
				#else
					buff2[0] = 0;
				#endif
				#ifdef POSITION_AFPOINTS_2
					buff2[1] = AfPointSet2;
				#else
					buff2[1] = 0;
				#endif
				#ifdef POSITION_AFPOINTS_3
					buff2[2] = AfPointSet3;
				#else
					buff2[2] = 0;
				#endif
				#ifdef POSITION_TLAPSE
					buff2[3] = TLapseInterval;
					buff2[4] = TLapseFrames;
				#else
					buff2[3] = 0;
					buff2[4] = 0;
				#endif
				buff2[5] = cameraMode.QualityRaw;
				buff2[6] = cameraMode.QualityFine;
				buff2[7] = cameraMode.QualitySize;
				buff2[8] = cameraMode.Iso;
				buff2[9] = cameraMode.WB;
				buff2[10] = cameraMode.ColorTemp;
				buff2[11] = cameraMode.MeteringMode;
				buff2[12] = cameraMode.AvComp;
				buff2[13] = cameraMode.FlashExComp;
				buff2[14] = cameraMode.Aeb;
				buff2[15] = cameraMode.CfAebSeq;
				buff2[16] = cameraMode.CfLensButton;
				buff2[17] = cameraMode.CfAfpSelection;
				buff2[18] = cameraMode.CfExtendIso;
				buff2[19] = cameraMode.CfAebSeq;
				buff2[20] = cameraMode.CfSafetyShift;
				buff2[21] = cameraMode.CfEmitFlash;
				buff2[22] = cameraMode.ParaSetOne;
				buff2[23] = cameraMode.ParaSetTwo;
				buff2[24] = cameraMode.ParaSetThree;
				#ifdef POSITION_FEB
					buff2[25] = FebFrames;
					buff2[26] = FebELevel;
				#else
					buff2[25] = 0;
					buff2[26] = 0;
				#endif
				#ifdef POSITION_EAEB
					buff2[27] = EaebFrames;
					buff2[28] = EaebELevel;
				#else
					buff2[27] = 0;
					buff2[28] = 0;
				#endif
	
				buff2[29] = cameraMode.AvVal;
				buff2[30] = cameraMode.TvVal;
				buff2[31] = cameraMode.AEMode;
	
				buff2[32] = cameraMode.DriveMode;
				buff2[33] = cameraMode.AfPoint;
				
				#ifdef POSITION_BW
					buff2[34] = cameraMode.Parameter;
				#else
					buff2[34] = 4;		//Parameter 3
				#endif
				
				#ifdef POSITION_NOISE
					buff2[35] =	cameraMode.CfNoiseReduction;
				#else
					buff2[35] = 0;		//Off
				#endif
	
				FIO_WriteFile(itmp, buff2, sizeof(buff2));
				FIO_CloseFile(itmp);
	
				LEDBLUE=LEDOFF;
	
				break;
	
			case BUTTON_PLAY:
	
				sprintf(buff, "A:/presets/%02d", UserPreset);
				itmp = FIO_OpenFile(buff, O_RDONLY, 644);
				if (itmp == -1) return;
	
				LEDBLUE=LEDON;
	
				for (itmp2=0; itmp2<36; itmp2++) {
					buff2[itmp2] = 0;
				}
	
				itmp2 = FIO_ReadFile(itmp, (char *)buff2, sizeof(buff2));
				FIO_CloseFile(itmp);
	
				#ifdef POSITION_AFPOINTS_1
					AfPointSet1 = buff2[0];
				#endif
				#ifdef POSITION_AFPOINTS_2
					AfPointSet2 = buff2[1];
				#endif
				#ifdef POSITION_AFPOINTS_3
					AfPointSet3 = buff2[2];
				#endif
				#ifdef POSITION_TLAPSE
					TLapseInterval = buff2[3];
					TLapseFrames = buff2[4];
				#endif
				cameraMode.QualityRaw = buff2[5];
				cameraMode.QualityFine = buff2[6];
				cameraMode.QualitySize = buff2[7];
	
				cameraMode.WB = buff2[9];
				cameraMode.ColorTemp = buff2[10];
				cameraMode.MeteringMode = buff2[11];
				cameraMode.Aeb = buff2[14];
				cameraMode.CfAebSeq = buff2[15];
				cameraMode.CfLensButton = buff2[16];
				cameraMode.CfAfpSelection = buff2[17];
				cameraMode.CfExtendIso = buff2[18];
				cameraMode.CfAebSeq = buff2[19];
				cameraMode.CfSafetyShift = buff2[20];
				cameraMode.CfEmitFlash = buff2[21];
				cameraMode.ParaSetOne = buff2[22];
				cameraMode.ParaSetTwo = buff2[23];
				
				// iso
				itmp = buff2[8];
				eventproc_SetIsoValue(&itmp);
	
				// avcomp
				SendToIntercom(0xa, 1, buff2[12]);
	
				// fec
				SendToIntercom(0x3, 1, buff2[13]);
	
				// contrast/sharpness/saturation/colortone (send set3 which will also update gui for sets 1+2 if needed)
				SendToIntercom(0x1f, 4, buff2[24]);
	
				#ifdef POSITION_FEB
					FebFrames = buff[25];
					FebELevel = buff[26];
				#endif
	
				#ifdef POSITION_EAEB
					EaebFrames = buff2[27];
					EaebELevel = buff2[28];
				#endif
				
				#ifdef POSITION_BW
				//	cameraMode.Parameter = buff2[34];
					SendToIntercom(0x1c, 1, buff2[34]);	      // select Parameter BW
				//	SendToIntercom(0x20, 4, 0x00000000 );     // set Parameter BW to 0,0,N,N
				#endif
				
				#ifdef POSITION_NOISE
				//	cameraMode.CfNoiseReduction = buff2[35];
					SendToIntercom(0x2b, 1, buff2[35]);
				#endif
					
				// av
				SendToIntercom(0x9, 1, buff2[29]);
	
				// tv
				SendToIntercom(0x8, 1, buff2[30]);
	
				// ae mode
				SendToIntercom(0x1, 1, buff2[31]);
	
				// drive mode
				SendToIntercom(0x4, 1, buff2[32]);
	
				// af point mode
				SendToIntercom(0x7, 1, buff2[33]);
	
				WriteSettings();
				LEDBLUE = LEDOFF;
				break;
		}
#ifdef PRESET_EDITOR
	}
#endif
}
#endif

#ifdef POSITION_EAEB
void SwitchEaeb(int button) {

	int EaebOrgReviewTime, EaebOrgAv, tmpidx, i;

	switch (button) {
		case BUTTON_SET:

			EaebOrgAv = cameraMode.AvComp;
			EaebOrgReviewTime = cameraMode.ReviewTime;

			// delay 2 seconds to start
			for (i=0; i<4; i++) {
				LEDBLUE=LEDON;
				SleepTask(250);
				LEDBLUE=LEDOFF;
				SleepTask(250);
			}

			SendToIntercom(0x53, 0, 0);
			SleepTask(5000);

			i = 0;
			while (i < (EaebFrames-1)/2) {

				tmpidx = HexFec(EaebOrgAv)+(EaebELevel*(i+1));
				if (tmpidx > 48) tmpidx = 48;

				SendToIntercom(0x0a, 1, FecHex(tmpidx) ); 
				SendToIntercom(0x53, 0, 0);
				SleepTask(5000); //delay for system to save picture

				tmpidx = HexFec(EaebOrgAv)-(EaebELevel*(i+1));
				if (tmpidx < 0) tmpidx = 0;

				SendToIntercom(0x0a, 1, FecHex(tmpidx) ); 
				SendToIntercom(0x53, 0, 0);
				SleepTask(5000); //delay for system to save picture
				i++;
			}

			// restore settings
			SendToIntercom(0x0a, 1, EaebOrgAv);
			cameraMode.ReviewTime = EaebOrgReviewTime;
			break;

		case BUTTON_MENU:
			if (EaebFrames <= 41) EaebFrames += 2;
			break;

		case BUTTON_PLAY:
			if (EaebFrames > 3) EaebFrames -= 2;
			break;

		case BUTTON_RIGHT:
			if (EaebELevel < 24) EaebELevel += 1;
			break;

		case BUTTON_LEFT:
			if (EaebELevel > 1) EaebELevel -= 1;
			break;
	}
}
#endif

/////////////////////////////////////////////////////////////////////////
//
// !!IMPORTANT!!
// NEXT 3 FUNCTIONS: GERMAN VERSION, LOOK DOWN FOR ENGLISH VERSION
//
/////////////////////////////////////////////////////////////////////////

#ifdef DEUTSCH
const char* my_GetColorTempString() {
	sprintf(buff, "FarbTemp:%uK", cameraMode.ColorTemp);
	return buff;
}

const char* my_GetDynamicString() { 
	switch (fn) {
#ifdef POSITION_ISO
		case POSITION_ISO-1:
#endif
#ifdef POSITION_CT
		case POSITION_CT-1:
#endif
#ifdef POSITION_FLASHEXCOMP
		case POSITION_FLASHEXCOMP-1:
#endif
#ifdef POSITION_AVCOMP
		case POSITION_AVCOMP-1:
#endif
#ifdef POSITION_AEB
		case POSITION_AEB-1:
#endif
#if defined(POSITION_ISO)||defined(POSITION_CT)||defined(POSITION_FLASHEXCOMP)||defined(POSITION_AVCOMP)||defined(POSITION_AEB)
			return "einstellen r/l";
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
#ifdef POSITION_BW
		case POSITION_BW-1:
			switch(cameraMode.Parameter){
				case 4: return "Standard";
				case 5: return "B/W";
			}
#endif
#ifdef POSITION_NOISE
		case POSITION_NOISE-1:
			if (cameraMode.CfNoiseReduction) 
				return "Aus";
			else
				return "Ein";
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
			sprintf(buff, "%u Ausloesg", releaseCount);
			return buff;
#endif
#ifdef POSITION_TLAPSE
		case POSITION_TLAPSE-1:
			ReadSettings();
			sprintf(buff, "%u x %us", TLapseFrames, TLapseInterval);
			return buff;
#endif
#ifdef POSITION_TLAPSERAMP
		case POSITION_TLAPSERAMP-1:
			sprintf(buff, "%c%s/%d", TLapseRLevel == 1 ? '+' : TLapseRLevel == -1 ? '-' : ' ', FecStr(HexFec(TLapseRLevel+24), 0), TLapseRFrames);
			return buff;
			break;
#endif
#ifdef POSITION_RAWMODE
		case POSITION_RAWMODE-1:
			sprintf(buff, "%s %s %s",
				cameraMode.QualityRaw == 1 ? "JPEG" : cameraMode.QualityRaw == 2 ? "Raw" : "Raw+JPEG",
				cameraMode.QualityFine ? "N" : "F",
				cameraMode.QualitySize == 0 ? "L" : cameraMode.QualitySize == 1 ? "M" : "S");
			return buff;
#endif
#ifdef POSITION_AFPOINTS_1
		case POSITION_AFPOINTS_1-1:
			return AfPointStr(AfPointSet1);
#endif
#ifdef POSITION_AFPOINTS_2
		case POSITION_AFPOINTS_2-1:
			return AfPointStr(AfPointSet2);
#endif
#ifdef POSITION_AFPOINTS_3
		case POSITION_AFPOINTS_3-1:
			return AfPointStr(AfPointSet3);
#endif
#ifdef POSITION_TEST
		case POSITION_TEST-1:
			return testoutput();
		case POSITION_TEST:
			return displayCameraModeValues(testcounter); 
#endif
#ifdef POSITION_AFPSELECTION
		case POSITION_AFPSELECTION-1:
			switch(cameraMode.CfAfpSelection) {
				default: return "Standard";
				case 1: return "Rad aus";
				case 2: return "Wahl blockiert";
			}
#endif
#ifdef POSITION_REVIEWTIME
		case POSITION_REVIEWTIME-1:
			switch(cameraMode.ReviewTime) {
				case 0: return "Aus";
				case 0xff: return "Halten";
				default: sprintf(buff, "%u s", cameraMode.ReviewTime);
					return buff;
			}
#endif
#ifdef POSITION_AUTOPOWEROFF
		case POSITION_AUTOPOWEROFF-1:
			if(cameraMode.AutoPowerOff) {
				sprintf(buff,"%d min",cameraMode.AutoPowerOff);
				return buff;
			}
			else return "Aus";
#endif
#ifdef POSITION_FEB
		case POSITION_FEB-1:
			sprintf(buff, "%s x%d +-%s", FecStr(HexFec(cameraMode.FlashExComp), 1), FebFrames, FecStr(FebELevel+24, 0));
			return buff;
#endif
#ifdef POSITION_CONTRAST
		case POSITION_CONTRAST-1:
			return GetContrastString(cameraMode.ParaSetThree&0xff);
#endif
#ifdef POSITION_SHARPNESS
		case POSITION_SHARPNESS-1:
			return GetContrastString((cameraMode.ParaSetThree&0xff00)>>8);
#endif
#ifdef POSITION_SATURATION
		case POSITION_SATURATION-1:
			return GetContrastString((cameraMode.ParaSetThree&0xff0000)>>16);
#endif
#ifdef POSITION_COLORTONE
		case POSITION_COLORTONE-1:
			return GetContrastString((cameraMode.ParaSetThree&0xff000000)>>24);
#endif
#ifdef POSITION_PRESETS
		case POSITION_PRESETS-1:
			return GetPresetName("preset");
#endif
#ifdef POSITION_EAEB
		case POSITION_EAEB-1:
			sprintf(buff, "%s x%d +-%s", FecStr(HexFec(cameraMode.AvComp), 1), EaebFrames, FecStr(EaebELevel+24, 0));
			return buff;
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
#ifdef POSITION_EAEB
		case POSITION_EAEB-1: return "eAEB";
#endif
#ifdef POSITION_SI
		case POSITION_SI-1: return "AF-Punkte(rot)";
#endif
#ifdef POSITION_MENU
		case POSITION_MENU-1: return "Menue-Position";
#endif
#ifdef POSITION_BW
		case POSITION_BW-1: return "B/W";
#endif
#ifdef POSITION_NOISE
		case POSITION_NOISE-1: return "Noise";
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
#ifdef POSITION_TLAPSE
		case POSITION_TLAPSE-1: return "Zeitraffer";
#endif
#ifdef POSITION_TLAPSERAMP
		case POSITION_TLAPSERAMP-1: return "Ramp";
#endif
#ifdef POSITION_RAWMODE
		case POSITION_RAWMODE-1: return "Bildqualitaet";
#endif
#ifdef POSITION_AFPOINTS_1
		case POSITION_AFPOINTS_1-1: return "AF-Punkte 1";
#endif
#ifdef POSITION_AFPOINTS_2
		case POSITION_AFPOINTS_2-1: return "AF-Punkte 2";
#endif
#ifdef POSITION_AFPOINTS_3
		case POSITION_AFPOINTS_3-1: return "AF-Punkte 3";
#endif
#ifdef POSITION_TEST
		case POSITION_TEST-1: sprintf(buff,"%u",isInFactoryMode); return buff;
		case POSITION_TEST: return displayCameraMode(testcounter);
#endif
#ifdef POSITION_AFPSELECTION
		case POSITION_AFPSELECTION-1: return "AFPunktAuswahl";
#endif
#ifdef POSITION_REVIEWTIME
		case POSITION_REVIEWTIME-1: return "Rueckschauzeit";
#endif
#ifdef POSITION_AUTOPOWEROFF
		case POSITION_AUTOPOWEROFF-1: return "AutoAbschalten";
#endif
#ifdef POSITION_FLASHEXCOMP
		case POSITION_FLASHEXCOMP-1: return "Blitzbe. Korr.";
#endif
#ifdef POSITION_AVCOMP
		case POSITION_AVCOMP-1:	return "Belichtg. Korr.";
#endif
#ifdef POSITION_FEB
		case POSITION_FEB-1: return "Blitzbe. Reihe";
#endif
#ifdef POSITION_CONTRAST
		case POSITION_CONTRAST-1: return QUOTE(POSITION_CONTRAST)":Contrast";
#endif
#ifdef POSITION_SHARPNESS
		case POSITION_SHARPNESS-1: return QUOTE(POSITION_SHARPNESS)":Sharpness";
#endif
#ifdef POSITION_SATURATION
		case POSITION_SATURATION-1: return QUOTE(POSITION_SATURATION)":Saturation";
#endif
#ifdef POSITION_COLORTONE
		case POSITION_COLORTONE-1: return QUOTE(POSITION_COLORTONE)":Color Tone";
#endif
#ifdef POSITION_PRESETS
		case POSITION_PRESETS-1: return QUOTE(POSITION_PRESETS)":Presets";
#endif
		default: return "?";
	}
}

#else

/////////////////////////////////////////////////////////////////////////
//
// NOW THE ENGLISH PART (next 3 functions), LOOK ABOVE FOR GERMAN VERSION
//
/////////////////////////////////////////////////////////////////////////

char *colortempstr;
const char* my_GetColorTempString() {
	sprintf(buff, "CTemp: %uK", cameraMode.ColorTemp);
	return buff;
}

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
#ifdef POSITION_BW
		case POSITION_BW-1:
			switch(cameraMode.Parameter){
				case 4: return "Parameter 3";
				case 5: return "B/W";
			}
#endif
#ifdef POSITION_NOISE
		case POSITION_NOISE-1:
			if (cameraMode.CfNoiseReduction) 
				return "On";
			else
				return "Off";
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
			sprintf(buff,"%u",releaseCount);
			return buff;
#endif
#ifdef POSITION_TLAPSE
		case POSITION_TLAPSE-1:
			ReadSettings();
			sprintf(buff, "%u x %us", TLapseFrames, TLapseInterval);
			return buff;
#endif
#ifdef POSITION_TLAPSERAMP
		case POSITION_TLAPSERAMP-1:
			sprintf(buff, "%s/%d m%d", FecStr(TLapseRLevel+24, 1), TLapseRFrames, TLapseRMax);
			return buff;
#endif
#ifdef POSITION_RAWMODE
		case POSITION_RAWMODE-1:
			sprintf(buff, "%s %s %s",
				cameraMode.QualityRaw == 1 ? "JPEG" : cameraMode.QualityRaw == 2 ? "Raw" : "Raw+JPEG",
				cameraMode.QualityFine ? "N" : "F",
				cameraMode.QualitySize == 0 ? "L" : cameraMode.QualitySize == 1 ? "M" : "S");
			return buff;
#endif
#ifdef POSITION_AFPOINTS_1
		case POSITION_AFPOINTS_1-1:
			return AfPointStr(AfPointSet1);
#endif
#ifdef POSITION_AFPOINTS_2
		case POSITION_AFPOINTS_2-1:
			return AfPointStr(AfPointSet2);
#endif
#ifdef POSITION_AFPOINTS_3
		case POSITION_AFPOINTS_3-1:
			return AfPointStr(AfPointSet3);
#endif
#ifdef POSITION_AFPSELECTION
		case POSITION_AFPSELECTION-1:
			switch(cameraMode.CfAfpSelection) {
				default: return "Standard";
				case 1: return "Wheel off";
				case 2: return "blocked";
			}
#endif
#ifdef POSITION_REVIEWTIME
		case POSITION_REVIEWTIME-1:
			switch(cameraMode.ReviewTime) {
				case 0: return "Off";
				case 0xff: return "Hold";
				default: sprintf(buff, "%u s", cameraMode.ReviewTime);
					return buff;
			}
#endif
#ifdef POSITION_AUTOPOWEROFF
		case POSITION_AUTOPOWEROFF-1:
			if(cameraMode.AutoPowerOff) {
				sprintf(buff,"%d min",cameraMode.AutoPowerOff);
				return buff;
			}
			else return "Off";
#endif
#ifdef POSITION_FEB
		case POSITION_FEB-1:
			sprintf(buff, "%s x%d +-%s", FecStr(HexFec(cameraMode.FlashExComp), 1), FebFrames, FecStr(FebELevel+24, 0));
			return buff;
#endif
#ifdef POSITION_CONTRAST
		case POSITION_CONTRAST-1:
			return GetContrastString(cameraMode.ParaSetThree&0xff);
#endif
#ifdef POSITION_SHARPNESS
		case POSITION_SHARPNESS-1:
			return GetContrastString((cameraMode.ParaSetThree&0xff00)>>8);
#endif
#ifdef POSITION_SATURATION
		case POSITION_SATURATION-1:
			return GetContrastString((cameraMode.ParaSetThree&0xff0000)>>16);
#endif
#ifdef POSITION_COLORTONE
		case POSITION_COLORTONE-1:
			return GetContrastString((cameraMode.ParaSetThree&0xff000000)>>24);
#endif
#ifdef POSITION_PRESETS
		case POSITION_PRESETS-1:
			return GetPresetName("preset");
#endif
#ifdef POSITION_EAEB
		case POSITION_EAEB-1:
			sprintf(buff, "%s x%d +-%s", FecStr(HexFec(cameraMode.AvComp), 1), EaebFrames, FecStr(EaebELevel+24, 0));
			return buff;
#endif
#ifdef POSITION_TEST
		case POSITION_TEST-1:
			return testoutput();
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
#ifdef ALT_EN_LABELS
		case POSITION_CT-1: return QUOTE(POSITION_CT)":Kelvin";
#else 
		case POSITION_CT-1: return QUOTE(POSITION_CT)":CTemp";
#endif
#endif
#ifdef POSITION_METERING
		case POSITION_METERING-1: return QUOTE(POSITION_METERING)":Metering";
#endif
#ifdef POSITION_AEB
		case POSITION_AEB-1: return QUOTE(POSITION_AEB)":AEB";
#endif
#ifdef POSITION_BSEQ
#ifdef ALT_EN_LABELS
		case POSITION_BSEQ-1: return QUOTE(POSITION_BSEQ)":AEB Mode";
#else
		case POSITION_BSEQ-1: return QUOTE(POSITION_BSEQ)":CF-BSeq/AutoC";
#endif
#endif
#ifdef POSITION_EAEB
		case POSITION_EAEB-1: return QUOTE(POSITION_EAEB)":eAEB";
#endif
#ifdef POSITION_SI
#ifdef ALT_EN_LABELS
		case POSITION_SI-1: return QUOTE(POSITION_SI)":Focus Dots";
#else
		case POSITION_SI-1: return QUOTE(POSITION_SI)":CF-SI Disp.";
#endif
#endif
#ifdef POSITION_MENU
#ifdef ALT_EN_LABELS
		case POSITION_MENU-1: return QUOTE(POSITION_MENU)":Menu Pos";
#else
		case POSITION_MENU-1: return QUOTE(POSITION_MENU)":CF-Menu Posi.";
#endif
#endif
#ifdef POSITION_BW
#ifdef ALT_EN_LABELS
		case POSITION_BW-1: return QUOTE(POSITION_BW)":Black/White";
#else
		case POSITION_BW-1: return QUOTE(POSITION_BW)":B/W";
#endif
#endif
#ifdef POSITION_NOISE
#ifdef ALT_EN_LABELS
		case POSITION_NOISE-1: return QUOTE(POSITION_NOISE)":Noise Red";
#else
		case POSITION_NOISE-1: return QUOTE(POSITION_NOISE)":Noise";
#endif
#endif
#ifdef POSITION_SAFESHIFT
#ifdef ALT_EN_LABELS
		case POSITION_SAFESHIFT-1: return QUOTE(POSITION_SAFESHIFT)":Safe Shift";
#else
		case POSITION_SAFESHIFT-1: return QUOTE(POSITION_SAFESHIFT)":CF-Safe.Shift";
#endif
#endif
#ifdef POSITION_AFBUTTON
#ifdef ALT_EN_LABELS
		case POSITION_AFBUTTON-1: return QUOTE(POSITION_AFBUTTON)":AF Button";
#else
		case POSITION_AFBUTTON-1: return QUOTE(POSITION_AFBUTTON)":CF-L.AFButt";
#endif
#endif
#ifdef POSITION_DATA
#ifdef ALT_EN_LABELS
		case POSITION_DATA-1: return QUOTE(POSITION_DATA)":Extra EXIF";
#else
		case POSITION_DATA-1: return QUOTE(POSITION_DATA)":CF-Decis.Da";
#endif
#endif
#ifdef POSITION_FLASH
#ifdef ALT_EN_LABELS
		case POSITION_FLASH-1: return QUOTE(POSITION_FLASH)":Flash";
#else
		case POSITION_FLASH-1: return QUOTE(POSITION_FLASH)":CF-Flash";
#endif
#endif
#ifdef POSITION_COUNTER
#ifdef ALT_EN_LABELS
		case POSITION_COUNTER-1: return QUOTE(POSITION_COUNTER)":Count";
#else
		case POSITION_COUNTER-1: return QUOTE(POSITION_COUNTER)":Shutt.Count";
#endif
#endif
#ifdef POSITION_TLAPSE
		case POSITION_TLAPSE-1: return QUOTE(POSITION_TLAPSE)":TimeLapse";
#endif
#ifdef POSITION_TLAPSERAMP
		case POSITION_TLAPSERAMP-1: return "TL Ramp";
#endif
#ifdef POSITION_RAWMODE
		case POSITION_RAWMODE-1: return QUOTE(POSITION_RAWMODE)":Raw+JPEG";
#endif
#ifdef POSITION_AFPOINTS_1
		case POSITION_AFPOINTS_1-1: return QUOTE(POSITION_AFPOINTS_1)":AF Points1";
#endif
#ifdef POSITION_AFPOINTS_2
		case POSITION_AFPOINTS_2-1: return QUOTE(POSITION_AFPOINTS_2)":AF Points2";
#endif
#ifdef POSITION_AFPOINTS_3
		case POSITION_AFPOINTS_3-1: return QUOTE(POSITION_AFPOINTS_3)":AF Points3";
#endif
#ifdef POSITION_AFPSELECTION
#ifdef ALT_EN_LABELS
		case POSITION_AFPSELECTION-1: return QUOTE(POSITION_AFPSELECTION)":AF PointSel";
#else
		case POSITION_AFPSELECTION-1: return QUOTE(POSITION_AFPSELECTION)":AFPSelection";
#endif
#endif
#ifdef POSITION_REVIEWTIME
		case POSITION_REVIEWTIME-1: return QUOTE(POSITION_REVIEWTIME)":Review Time";
#endif
#ifdef POSITION_AUTOPOWEROFF
#ifdef ALT_EN_LABELS
		case POSITION_AUTOPOWEROFF-1: return QUOTE(POSITION_AUTOPOWEROFF)":Power Off";
#else
		case POSITION_AUTOPOWEROFF-1: return QUOTE(POSITION_AUTOPOWEROFF)":AutoPowerOff";
#endif
#endif
#ifdef POSITION_FLASHEXCOMP
#ifdef ALT_EN_LABELS
		case POSITION_FLASHEXCOMP-1: return QUOTE(POSITION_FLASHEXCOMP)":FEC";
#else
		case POSITION_FLASHEXCOMP-1: return QUOTE(POSITION_FLASHEXCOMP)":FlashExComp";
#endif
#endif
#ifdef POSITION_AVCOMP
#ifdef ALT_EN_LABELS
		case POSITION_AVCOMP-1:	return QUOTE(POSITION_AVCOMP)":AV Comp";
#else
		case POSITION_AVCOMP-1:	return QUOTE(POSITION_AVCOMP)":AVComp";
#endif
#endif
#ifdef POSITION_FEB
#ifdef ALT_EN_LABELS
		case POSITION_FEB-1: return QUOTE(POSITION_FEB)":eFEB";
#else
		case POSITION_FEB-1: return QUOTE(POSITION_FEB)":FlashBracket";
#endif
#endif
#ifdef POSITION_CONTRAST
		case POSITION_CONTRAST-1: return QUOTE(POSITION_CONTRAST)":Contrast";
#endif
#ifdef POSITION_SHARPNESS
		case POSITION_SHARPNESS-1: return QUOTE(POSITION_SHARPNESS)":Sharpness";
#endif
#ifdef POSITION_SATURATION
		case POSITION_SATURATION-1: return QUOTE(POSITION_SATURATION)":Saturation";
#endif
#ifdef POSITION_COLORTONE
		case POSITION_COLORTONE-1: return QUOTE(POSITION_COLORTONE)":Color Tone";
#endif
#ifdef POSITION_PRESETS
		case POSITION_PRESETS-1: return QUOTE(POSITION_PRESETS)":Presets";
#endif
		default: return "?";
	}
}
#endif

/////////////////////////////////////////////////////////////////////////
//
// MyTask() - main processing loop for our thread
//
// cameraMode.MenuPos seems to count the menus beginning with 0x?1 at the top of every page
// and 0x0? for the first page
//
// 0x01 means first page, first submenu (Quality)
// 0x13 means second page, third submenu
// 0x33 means fourth page, third submenu
//
// etc
//
/////////////////////////////////////////////////////////////////////////

void MyTask() {
	const int mesmode=3;
	int isRelease=0;
	int infoJumped=0;
	
	SleepTask(1000);
#if !defined(NOSETTINGSFILE)
	ReadSettings();
#endif
#ifdef RESETFLASHEXCOMP
	SendToIntercom(3,1,0);
#endif
	SendToIntercom(0x31, 1, 1);					// enable CFn.8 for ISO H
	
	
	//after the initialisations are done, do this forever
	while (1) {									
	
		int* pMessage = (int*)MainHeapAlloc(sizeof(int) * 2);
		//	TryReceiveMessageQueue(hMyTaskMessQue, &pMessage, 0);
		ReceiveMessageQueue(hMyTaskMessQue, &pMessage, 0); 		
		
		//begin loop from top if no keypress is there to react on
		if (!pMessage)											
			continue;
			
		//else go on
		switch (pMessage[0]) {									
		
			/////////////////////////////////////////////////////////
			//
			// shortcuts
			//

#ifdef SHORTCUTS
			// zoom-out button pressed in ...
			case MESSAGE_ZO:										
				if (handleMainMenuDlg){
					if (isRelease)
						isRelease=0;
					else{
						switch (cameraMode.MenuPos){
#ifdef POSITION_AFPOINTS_1
							// ...quality menu
							case 1:										
								// set AF point set 1
								SendToIntercom(7,1,AfPointSet1);			
								SHORTCUTSIGNAL
								break;
#endif
#ifdef POSITION_TEST
							case 2:
								WriteMoreStuff();
								Beep(0);
								break;
#endif
							// ...beep menu
							case 3:										
								//toggle flash on/off
								SendToIntercom(0x30,1,cameraMode.CfEmitFlash^1);	
								//flash red led for enabled, blue for disabled flash
								if(cameraMode.CfEmitFlash)					
									LEDRED=LEDON;
								else
									LEDBLUE=LEDON;
								SleepTask(150);
								LEDRED=LEDOFF,LEDBLUE=LEDOFF;
								break;

							// ...AF menu
							case 4: 		
								// select Parameter B/W and set BW 0,0,N,N
								SendToIntercom(0x1c, 1, 5);		// select Parameter BW 16,16,N,N
								SendToIntercom(0x20, 4, 0x00000000 ); // set Parameter BW to 0,0,N,N
								break;

							// ...metering menu
							case 5:										
								// goto beep menu
								eventproc_RiseEvent("PressMenuButton", 0);	
								eventproc_RiseEvent("PressUpButton", 0);
								eventproc_RiseEvent("PressUpButton", 0);
								eventproc_RiseEvent("PressSetButton", 0);
								break;

							// ...ISO menu
							case 6:										
								// set ISO 100
								eventproc_RiseEvent("SetIsoValue", 0x48);	
								// close menu and
								eventproc_RiseEvent("PressMenuButton", 0);	
								// open menu again to show new setting
								eventproc_RiseEvent("PressSetButton", 0);	
								SHORTCUTSIGNAL
								break;

							// ...WB menu
							case 0x13:									
								// set AWB
								SendToIntercom(5, 1, 0);					
								// close menu and
								eventproc_RiseEvent("PressMenuButton", 0);	
								// open menu again to show new setting
								eventproc_RiseEvent("PressSetButton", 0);	
								SHORTCUTSIGNAL
						}
			                	isRelease=1;
					}
				}
				break;

			// zoom-in button pressed in ...
			case MESSAGE_ZI:										
		                if (handleMainMenuDlg) {
					if(isRelease)
						isRelease=0;
					else{
						switch (cameraMode.MenuPos){
#ifdef POSITION_AFPOINTS_2
							// ...quality menu
							case 1:										
								// set AF point set 2
								SendToIntercom(7,1,AfPointSet2);			
								SHORTCUTSIGNAL
								break;
#endif
#ifdef POSITION_TEST
/*							case 2:
								eventproc_Shutdown();
								break;*/
#endif
							// ...beep menu
							case 3:										
								//toggle safety shift
								SendToIntercom(0x39,1,cameraMode.CfSafetyShift^1);	
								//flash red led for enabled, blue for disabled SS
								if(cameraMode.CfSafetyShift)				
									LEDBLUE=LEDON;
								else
									LEDRED=LEDON;
								SleepTask(150);
								LEDRED=LEDOFF,LEDBLUE=LEDOFF;
								break;

							// ...AF menu
							case 4:		
								// set Parameter 3 
								SendToIntercom(0x1c, 1, 4);					
								break;

							// ...metering menu
							case 5:										
								// Spot metering mode
								eventproc_SetMesMode(&mesmode);					
								SHORTCUTSIGNAL
								break;

							// ...ISO menu
							case 6:										
								// set ISO 3200
								eventproc_RiseEvent("SetIsoValue", 0x6f);	
								SHORTCUTSIGNAL
								break;

							// ...WB menu
							case 0x13:									
								//set WB ColorTemp
								SendToIntercom(5, 1, 8);		 			
								SHORTCUTSIGNAL
						}
						isRelease=1;
					}
				}
				break;

			//
			// direct print button pressed in ...
			//

			case MESSAGE_DP:										
				if (handleMainMenuDlg) {
					switch (cameraMode.MenuPos){

#ifdef POSITION_AFPOINTS_3
						// ...quality menu
						case 1:											
							//set AF point set 3
							SendToIntercom(7,1,AfPointSet3);				
							SHORTCUTSIGNAL
							break;
#endif

#ifdef POSITION_TEST
						case 2:
							EnterFactoryMode();
							Beep(0);
							break;
#endif
						// ...beep menu
						case 3:											
							//toggle mirror lock-up
							SendToIntercom(0x35,1,cameraMode.CfMLU^1);		

							//flash red led for enabled, blue for disabled MLU
							if(cameraMode.CfMLU)							
								LEDBLUE=LEDON;
							else
								LEDRED=LEDON;
							SleepTask(150);
							LEDRED=LEDOFF,LEDBLUE=LEDOFF;
							break;

						// ...AF menu
						case 4:												
							//Rotate LCD brightness 1,2,3,4,5 (NEW)	
							if (cameraMode.LcdBrightness>=5)
								SendToIntercom(0x16,1,1);
							else
								SendToIntercom(0x16,1,(cameraMode.LcdBrightness+1));
							break;

						// ...metering menu
						case 5:											
							//go to parameter menu
							eventproc_RiseEvent("PressMenuButton", 0);			
							eventproc_RiseEvent("PressJumpButton", 0);
							eventproc_RiseEvent("PressUpButton", 0);
							eventproc_RiseEvent("PressSetButton", 0);
							break;

						// ...ISO menu
						case 6:											
							//set ISO 400
							eventproc_RiseEvent("SetIsoValue", 0x58);		
							//close menu and
							eventproc_RiseEvent("PressMenuButton", 0);		
							//open menu again to show new setting
							eventproc_RiseEvent("PressSetButton", 0);		
							SHORTCUTSIGNAL
							break;

						// ...WB menu
						case 0x13:										
							// Set WB to manual (using last taken picture as reference)
							SendToIntercom(5,1,6);							
							// go from wb submenu to menu page 2
							eventproc_RiseEvent("PressMenuButton",0);		
							eventproc_RiseEvent("PressDownButton",0);
							//go down to manual wb image selection
							eventproc_RiseEvent("PressDownButton",0);		
							eventproc_RiseEvent("PressSetButton",0);
							//select last image on card
							eventproc_RiseEvent("PressSetButton",0);		
							eventproc_RiseEvent("PressUpButton",0);
							eventproc_RiseEvent("PressUpButton",0);
							//go back to WB menu
							eventproc_RiseEvent("PressSetButton",0);		
							SHORTCUTSIGNAL
					}
				}
				break;
#endif // SHORTCUTS


			//////////////////////////////////////////////////////
			//
			// menu options
			//

			case MESSAGE_UD:
				if (isRelease)
					isRelease=0;
#ifdef POSITION_PRESETS
				else if ((pMessage[1] == BUTTON_UP || pMessage[1] == BUTTON_DOWN) && fn == POSITION_PRESETS-1 && EditorActive) {
					SwitchPresets(pMessage[1]);
					SendToIntercom(0xd,1,cameraMode.Aeb);
					isRelease=1;
				}
#endif
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

#ifdef INFOJUMPS_COUNT
#ifdef POSITION_TLAPSE
				else if (pMessage[1] == BUTTON_INFO && fn != POSITION_TLAPSE-1) {
#else
				else if (pMessage[1] == BUTTON_INFO) {
#endif
					infoJumped = 0;
					for (itmp=0; itmp < INFOJUMPS_COUNT; itmp++) {
						if (!infoJumped) {
							if (fn < INFOJUMPS[itmp]-1) {
								fn = INFOJUMPS[itmp]-1;
								infoJumped = 1;
							}
						}
					}
					if (!infoJumped) fn = 0;
					SendToIntercom(0xd,1,cameraMode.Aeb);
				}
#else
				else if (0) {
					// in case not using info jumps
				}
#endif // INFOJUMPS_COUNT
				else{
					switch(fn){
#ifdef POSITION_ISO
						case POSITION_ISO-1:
							SwitchIso(pMessage[1]);
							break;
#endif
#ifdef POSITION_WB
						case POSITION_WB-1:
							SendToIntercom(5,1,CfnVal(pMessage[1],cameraMode.WB,8,0));
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
							SendToIntercom(2,1,CfnVal(pMessage[1],cameraMode.MeteringMode,3,0));
							SendToIntercom(0xd,1,cameraMode.Aeb);
							break;
#endif
#ifdef POSITION_AEB
						case POSITION_AEB-1:
							SwitchAeb(pMessage[1]);
							break;
#endif
#ifdef POSITION_BSEQ
						// BSeq/AutoC
						case POSITION_BSEQ-1: 
							SendToIntercom(0x32, 1, CfnVal(pMessage[1],cameraMode.CfAebSeq,3,0));
							SendToIntercom(0xd,1,cameraMode.Aeb);
							break;
#endif
#ifdef POSITION_SI
						//Si display
						case POSITION_SI-1: 
							SendToIntercom(0x33, 1, ToggleCfnVal(pMessage[1],cameraMode.CfSiIndicate));
							SendToIntercom(0xd,1,cameraMode.Aeb);
							break;
#endif
#ifdef POSITION_MENU
						//menu Position
						case POSITION_MENU-1: 
							SendToIntercom(0x34, 1, CfnVal(pMessage[1],cameraMode.CfMenuPos,2,0));
							SendToIntercom(0xd,1,cameraMode.Aeb);
							break;
#endif
#ifdef POSITION_BW
						//Black and White Select Parameter (switch between "Parameter 3" and "B/W")
						case POSITION_BW-1: 
							SendToIntercom(0x1c, 1, CfnVal(pMessage[1],cameraMode.Parameter,5,4));
							SendToIntercom(0xd,1,cameraMode.Aeb);
							break;
#endif
#ifdef POSITION_NOISE
						//Automatic Noise Reduction (Cf2)
						case POSITION_NOISE-1: 
							SendToIntercom(0x2b, 1, ToggleCfnVal(pMessage[1],cameraMode.CfNoiseReduction));
							SendToIntercom(0xd,1,cameraMode.Aeb);
							break;
#endif

#ifdef POSITION_SAFESHIFT
						//Safety Shift
						case POSITION_SAFESHIFT-1: 
							SendToIntercom(0x39, 1, ToggleCfnVal(pMessage[1],cameraMode.CfSafetyShift));
							SendToIntercom(0xd,1,cameraMode.Aeb);
							break;
#endif
#ifdef POSITION_AFBUTTON
						//Lens AF Button
						case POSITION_AFBUTTON-1: 
							SendToIntercom(0x3a, 1, CfnVal(pMessage[1],cameraMode.CfLensButton,5,0));
							SendToIntercom(0xd,1,cameraMode.Aeb);
							break;
#endif
#ifdef POSITION_DATA
						//  Decision Data
						case POSITION_DATA-1: 
							SendToIntercom(0x3b, 1, ToggleCfnVal(pMessage[1],cameraMode.CfOriginalEval));
							SendToIntercom(0xd,1,cameraMode.Aeb);
							break;
#endif
#ifdef POSITION_FLASH
						//  Flash
						case POSITION_FLASH-1: 
							SendToIntercom(0x30, 1, ToggleCfnVal(pMessage[1],cameraMode.CfEmitFlash));
							SendToIntercom(0xd,1,cameraMode.Aeb);
							break;
#endif
#ifdef POSITION_TLAPSE
						// Time-lapse
						case POSITION_TLAPSE-1: 
							SwitchTimeLapse(pMessage[1]);
							WriteSettings();
							SendToIntercom(0xd,1,cameraMode.Aeb);
							break;
#endif
#ifdef POSITION_TLAPSERAMP
						// Time-lapse Ramping
						case POSITION_TLAPSERAMP-1: 
							SwitchTimeLapseRamp(pMessage[1]);
							SendToIntercom(0xd,1,cameraMode.Aeb);
							break;
#endif
#ifdef POSITION_RAWMODE
						// Raw toggle
						case POSITION_RAWMODE-1: 
							SwitchRawMode(pMessage[1]);
							SendToIntercom(0xd,1,cameraMode.Aeb);
							break;
#endif
#ifdef POSITION_AFPOINTS_1
						//AF points set 1 setup
						case POSITION_AFPOINTS_1-1: 
							SetAFPoints(pMessage[1],&AfPointSet1);
							WriteSettings();
							SendToIntercom(0xd,1,cameraMode.Aeb);
							break;
#endif
#ifdef POSITION_AFPOINTS_2
						//AF points set 2 setup
						case POSITION_AFPOINTS_2-1: 
							SetAFPoints(pMessage[1],&AfPointSet2);
							WriteSettings();
							SendToIntercom(0xd,1,cameraMode.Aeb);
							break;
#endif
#ifdef POSITION_AFPOINTS_3
						//AF points set 3 setup
						case POSITION_AFPOINTS_3-1: 
							SetAFPoints(pMessage[1],&AfPointSet3);
							WriteSettings();
							SendToIntercom(0xd,1,cameraMode.Aeb);
							break;
#endif
#ifdef POSITION_TEST
						case POSITION_TEST-1:
							if(pMessage[1]==BUTTON_SET)
								{cameraMode.EfLens^=1;Beep(0);}
							SendToIntercom(0xd,1,cameraMode.Aeb);
							break;
						case POSITION_TEST:
							if(pMessage[1]==BUTTON_LEFT)
								testcounter-=3;
							else if(pMessage[1]==BUTTON_RIGHT)
								testcounter+=3;
							SendToIntercom(0xd,1,cameraMode.Aeb);
							break;
#endif
#ifdef POSITION_AFPSELECTION
						case POSITION_AFPSELECTION-1:
							SendToIntercom(0x36,1,CfnVal(pMessage[1],cameraMode.CfAfpSelection,2,0));
							SendToIntercom(0xd,1,cameraMode.Aeb);
							break;
#endif
#ifdef POSITION_REVIEWTIME
						case POSITION_REVIEWTIME-1:
							SendToIntercom(0x14,1,ChangeReviewTime(pMessage[1]));
							SendToIntercom(0xd,1,cameraMode.Aeb);
							break;
#endif
#ifdef POSITION_AUTOPOWEROFF
						case POSITION_AUTOPOWEROFF-1:
							SendToIntercom(0x12,1,CfnVal(pMessage[1],cameraMode.AutoPowerOff,AUTOPOWEROFFMAX,0));
							SendToIntercom(0xd,1,cameraMode.Aeb);
							break;
#endif
#ifdef POSITION_FLASHEXCOMP
						case POSITION_FLASHEXCOMP-1:
							//SendToIntercom(3,1,FecHex(fec=CfnVal(pMessage[1],fec,48,0)));
							SendToIntercom(3, 1, FecHex( CfnVal(pMessage[1], HexFec(cameraMode.FlashExComp), 48, 0) ));
							break;
#endif
#ifdef POSITION_AVCOMP
						case POSITION_AVCOMP-1:
							//SendToIntercom(0xa,1,FecHex(avc=CfnVal(pMessage[1],avc,48,0)));
							SendToIntercom(0xa, 1, FecHex( CfnVal(pMessage[1], HexFec(cameraMode.AvComp), 48, 0) ));
							SendToIntercom(0xd, 1, cameraMode.Aeb);
#endif
#ifdef POSITION_FEB
						case POSITION_FEB-1:
							SwitchFeb(pMessage[1]);
							SendToIntercom(0xd,1,cameraMode.Aeb);
							break;
#endif
#ifdef POSITION_CONTRAST
						case POSITION_CONTRAST-1:
							SwitchContrast(pMessage[1]);
							SendToIntercom(0xd,1,cameraMode.Aeb);
							break;
#endif
#ifdef POSITION_SHARPNESS
						case POSITION_SHARPNESS-1:
							SwitchSharpness(pMessage[1]);
							SendToIntercom(0xd,1,cameraMode.Aeb);
							break;
#endif
#ifdef POSITION_SATURATION
						case POSITION_SATURATION-1:
							SwitchSaturation(pMessage[1]);
							SendToIntercom(0xd,1,cameraMode.Aeb);
							break;
#endif
#ifdef POSITION_COLORTONE
						case POSITION_COLORTONE-1:
							SwitchColorTone(pMessage[1]);
							SendToIntercom(0xd,1,cameraMode.Aeb);
							break;
#endif
#ifdef POSITION_PRESETS
						case POSITION_PRESETS-1:
							SwitchPresets(pMessage[1]);
							SendToIntercom(0xd,1,cameraMode.Aeb);
							break;
#endif
#ifdef POSITION_EAEB
						case POSITION_EAEB-1:
							SwitchEaeb(pMessage[1]);
							SendToIntercom(0xd,1,cameraMode.Aeb);
							break;
#endif
					}

					//isRelease is only needed for the zoom-in, zoom-out, left, right, up and down buttons
					if(pMessage[1]==BUTTON_LEFT||pMessage[1]==BUTTON_RIGHT) {
						//using it for the other ones too, you need to press them two times
						isRelease=1;												
					}
				}
				break;
		}
		MainHeapFree(pMessage);
	}
}

//
// test thread function
//

#ifdef POSITION_TEST
void TestTask()
	{
	while(1){
	SleepTask(5000);
	Beep(0);
	}
}
#endif

//
// create our thread
//

void CreateMyTask() {
	hMyTaskMessQue = (int*)CreateMessageQueue("MyTaskMessQue", 0x40);
	CreateTask("MyTask", 0x19, 0x2000, MyTask, 0);
#ifdef POSITION_TEST
	CreateTask("TestTask", 0x19, 0x1000, TestTask, 0);
#endif
}

void SendMyMessage(int param0, int param1) {
	int* pMessage = (int*)MainHeapAlloc(sizeof(int) * 2);
	pMessage[0] = param0;
	pMessage[1] = param1;
	TryPostMessageQueue(hMyTaskMessQue, pMessage, 0);
	MainHeapFree(pMessage);
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
		case BUTTON_PLAY:
#if defined(POSITION_TLAPSE)||defined(POSITION_FEB)
		case BUTTON_MENU:
		case BUTTON_INFO:
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
//		case BUTTON_JUMP:
//			SendMyMessage(MESSAGE_JP, ptr[1]);
//			break;
	}

	IntercomHandler(r0, ptr);
}
