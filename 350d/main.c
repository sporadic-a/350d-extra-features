/*
 * EOS 350D mod project
 * Original work by maiohwmai and cyrustam
 * Further mods by crazyklaus and MilanC
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


int fn=0;
int* hMyTaskMessQue;

char buff[17];
char buff2[5];
char buff3[4];

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
#define	BUTTON_PLAY				0xa4
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



#if defined(POSITION_AFPOINTS_1)||defined(POSITION_AFPOINTS_2)||defined(POSITION_AFPOINTS_3)||defined(POSITION_TLAPSE)
unsigned int settingsbuff[5];

void ReadSettings(){
#ifdef NOSETTINGSFILE
	return;
#endif
int settings = FIO_OpenFile("A:/settings", O_RDONLY, 644);
if(settings!=-1) {
	FIO_ReadFile(settings, (char *)settingsbuff, sizeof(settingsbuff));
	#ifdef POSITION_AFPOINTS_1
	AfPointSet1=settingsbuff[0];
	#endif
	#ifdef POSITION_AFPOINTS_2
	AfPointSet2=settingsbuff[1];
	#endif
	#ifdef POSITION_AFPOINTS_3
	AfPointSet3=settingsbuff[2];
	#endif
	#ifdef POSITION_TLAPSE
	TLapseInterval=settingsbuff[3];
	TLapseFrames=settingsbuff[4];
	#endif
	FIO_CloseFile(settings);
	}
}

void WriteSettings(){
#ifdef NOSETTINGSFILE
	return;
#endif
int settings = FIO_OpenFile("A:/settings", O_CREAT|O_WRONLY , 644);
if(settings!=-1) {
	#ifdef POSITION_AFPOINTS_1
	settingsbuff[0]= AfPointSet1;
	#endif
	#ifdef POSITION_AFPOINTS_2
	settingsbuff[1]= AfPointSet2;
	#endif
	#ifdef POSITION_AFPOINTS_3
	settingsbuff[2]= AfPointSet3;
	#endif
	#ifdef POSITION_TLAPSE
	settingsbuff[3]=TLapseInterval;
	settingsbuff[4]=TLapseFrames;
	#endif
	FIO_WriteFile(settings, settingsbuff, sizeof(settingsbuff));
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
			if (TLapseFrames>1)
				TLapseFrames /= 2;
			break;
#ifdef USEPLAYFORSET
		case BUTTON_SET:
#else
		case BUTTON_PLAY:
#endif
			TLapseFrames++;
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
			JpgQual ^= 1;
			break;
		case BUTTON_RIGHT:
			JpgSize = (JpgSize+1)%3;
	  }
	SendToIntercom(0x22,1,RawMode);
	SendToIntercom(0x24,1,JpgQual);
	SendToIntercom(0x23,1,JpgSize);
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

#if defined(POSITION_FLASHEXCOMP)||defined(POSITION_FEB)||defined(POSITION_AVCOMP)
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
	
	return temp3;							//this should be it
}
#endif										//PS: YEEEEHA, it works!

#ifdef POSITION_FEB

void SwitchFEB(int button) {
	int OldReviewTime,OldFEC;
	switch(button) {
#ifdef USEPLAYFORSET
		case BUTTON_PLAY:
#else
		case BUTTON_SET:
#endif
			OldReviewTime=cameraMode.ReviewTime;
			OldFEC=cameraMode.FlashExComp;
			cameraMode.ReviewTime=0xff;
			
			SendToIntercom(3,1,FecHex(24-FebDiff));
			SendToIntercom(0x53,0,0);
			
			SleepTask(FebDelay);
				
			SendToIntercom(3,1,FecHex(24));
			SendToIntercom(0x53,0,0);
			
			SleepTask(FebDelay);
			
			SendToIntercom(3,1,FecHex(24+FebDiff));
			SendToIntercom(0x53,0,0);
			
			SleepTask(FebDelay);
				
			cameraMode.ReviewTime=OldReviewTime;
			SendToIntercom(3,1,OldFEC);
			SHORTCUTSIGNAL
			break;
		case BUTTON_LEFT:
			if(FebDiff)
				FebDiff--;
			break;
		case BUTTON_RIGHT:
			if(FebDiff!=24)
				FebDiff++;
			break;
		case BUTTON_MENU:
			FebDelay+=1000;
			break;
		case BUTTON_INFO:
			if(FebDelay!=3000)
				FebDelay-=1000;
	}
}

char* FEBStr() {
	int temp1=0;
	int temp2=0;
	temp1=FebDiff/4;
	switch(FebDiff%4) {
		case 1:
			temp2=3;
			break;
		case 2:
			temp2=5;
			break;
		case 3:
			temp2=7;
	}
	sprintf(buff,"+-%u.%u %ums",temp1,temp2,FebDelay);
	return buff;
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

// !!IMPORTANT!!
// NEXT 3 FUNCTIONS: GERMAN VERSION, LOOK DOWN FOR ENGLISH VERSION

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
#ifdef POSITION_RAWMODE
		case POSITION_RAWMODE-1:
			switch(cameraMode.QualityRaw) {
				case 1: return "nur JPG";
				case 2: return "nur RAW";
				default: return "RAW+JPEG";
			}
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
			return FEBStr();
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
#ifdef POSITION_TLAPSE
		case POSITION_TLAPSE-1: return "Zeitraffer";
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
		default: return "?";
	}
}

#else

// NOW THE ENGLISH PART (next 3 functions), LOOK ABOVE FOR GERMAN VERSION

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
#ifdef POSITION_RAWMODE
		case POSITION_RAWMODE-1:
			switch(RawMode) {
				case 1: return "JPEG only";
				case 2: return "Raw only";
				default: return "Raw+JPEG";
			  }
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
			return FEBStr();
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
#ifdef POSITION_AFPOINTS_1
		case POSITION_AFPOINTS_1-1: return QUOTE(POSITION_AFPOINTS_1)":AF Points 1";
#endif
#ifdef POSITION_AFPOINTS_2
		case POSITION_AFPOINTS_2-1: return QUOTE(POSITION_AFPOINTS_2)":AF Points 2";
#endif
#ifdef POSITION_AFPOINTS_3
		case POSITION_AFPOINTS_3-1: return QUOTE(POSITION_AFPOINTS_3)":AF Points 3";
#endif
#ifdef POSITION_AFPSELECTION
		case POSITION_AFPSELECTION-1: return QUOTE(POSITION_AFPSELECTION)":AFPSelection";
#endif
#ifdef POSITION_REVIEWTIME
		case POSITION_REVIEWTIME-1: return QUOTE(POSITION_REVIEWTIME)":Review Time";
#endif
#ifdef POSITION_AUTOPOWEROFF
		case POSITION_AUTOPOWEROFF-1: return QUOTE(POSITION_AUTOPOWEROFF)":AutoPowerOff";
#endif
#ifdef POSITION_FLASHEXCOMP
		case POSITION_FLASHEXCOMP-1: return QUOTE(POSITION_FLASHEXCOMP)":FlashExComp";
#endif
#ifdef POSITION_AVCOMP
		case POSITION_AVCOMP-1:	return QUOTE(POSITION_AVCOMP)":AVComp";
#endif
#ifdef POSITION_FEB
		case POSITION_FEB-1: return QUOTE(POSITION_FEB)":FlashBracket";
#endif
		default: return "?";
	}
}
#endif

void MyTask() {
	const int mesmode=3;
	int isRelease=0;
	
	SleepTask(1000);
#if defined(POSITION_AFPOINTS_1)||defined(POSITION_AFPOINTS_2)||defined(POSITION_AFPOINTS_3)||defined(POSITION_TLAPSE)
	ReadSettings();
#endif
#ifdef RESETFLASHEXCOMP
	SendToIntercom(3,1,0);
#endif
	SendToIntercom(0x31, 1, 1);					// enable CFn.8 for ISO H
	
	
	while (1) {									//after the initialisations are done, do this forever
	
		int* pMessage = (int*)MainHeapAlloc(sizeof(int) * 2);
		ReceiveMessageQueue(hMyTaskMessQue, &pMessage, 0); 		//	TryReceiveMessageQueue(hMyTaskMessQue, &pMessage, 0);
		
		if (!pMessage)											//begin loop from top if no keypress is there to react on
			continue;
			
		switch (pMessage[0]) {									//else go on
		
#ifdef SHORTCUTS
/*
cameraMode.MenuPos seems to count the menus beginning with 0x?1 at the top of every page
and 0x0? for the first page

0x01 means first page, first submenu (Quality)
0x13 means second page, third submenu
0x33 means fourth page, third submenu

etc
*/
			case MESSAGE_ZO:										// zoom-out button pressed in ...
				if (handleMainMenuDlg){
					if (isRelease)
						isRelease=0;
					else{
						switch (cameraMode.MenuPos){
#ifdef POSITION_AFPOINTS_1
							case 1:										// ...quality menu
								SendToIntercom(7,1,AfPointSet1);			// set AF point set 1
								SHORTCUTSIGNAL
								break;
#endif
#ifdef POSITION_TEST
							case 2:
								WriteMoreStuff();
								Beep(0);
								break;
#endif
							case 3:										// ...beep menu
								SendToIntercom(0x30,1,cameraMode.CfEmitFlash^1);	//toggle flash on/off
								if(cameraMode.CfEmitFlash)					//flash red led for enabled, blue for disabled flash
									LEDRED=LEDON;
								else
									LEDBLUE=LEDON;
								SleepTask(150);
								LEDRED=LEDOFF,LEDBLUE=LEDOFF;
								break;
							case 4: 									// ...AF menu
								SendToIntercom(0x16, 1, 1);					// set LCD brightness to min
								break;
							case 5:										// ...metering menu
								eventproc_RiseEvent("PressMenuButton", 0);	// goto beep menu
								eventproc_RiseEvent("PressUpButton", 0);
								eventproc_RiseEvent("PressUpButton", 0);
								eventproc_RiseEvent("PressSetButton", 0);
								break;
							case 6:										// ...ISO menu
								eventproc_RiseEvent("SetIsoValue", 0x48);	// set ISO 100
								eventproc_RiseEvent("PressMenuButton", 0);	// close menu and
								eventproc_RiseEvent("PressSetButton", 0);	// open menu again to show new setting
								SHORTCUTSIGNAL
								break;
							case 0x13:									// ...WB menu
								SendToIntercom(5, 1, 0);					// set AWB
								eventproc_RiseEvent("PressMenuButton", 0);	// close menu and
								eventproc_RiseEvent("PressSetButton", 0);	// open menu again to show new setting
								SHORTCUTSIGNAL
						}
			                	isRelease=1;
					}
				}
				break;
			case MESSAGE_ZI:										// zoom-in button pressed in ...
		                if (handleMainMenuDlg) {
					if(isRelease)
						isRelease=0;
					else{
						switch (cameraMode.MenuPos){
#ifdef POSITION_AFPOINTS_2
							case 1:										// ...quality menu
								SendToIntercom(7,1,AfPointSet2);			// set AF point set 2
								SHORTCUTSIGNAL
								break;
#endif
#ifdef POSITION_TEST
/*							case 2:
								eventproc_Shutdown();
								break;*/
#endif
							case 3:										// ...beep menu
								SendToIntercom(0x39,1,cameraMode.CfSafetyShift^1);	//toggle safety shift
								if(cameraMode.CfSafetyShift)				//flash red led for enabled, blue for disabled SS
									LEDBLUE=LEDON;
								else
									LEDRED=LEDON;
								SleepTask(150);
								LEDRED=LEDOFF,LEDBLUE=LEDOFF;
								break;
							case 4:										// ...AF menu
								SendToIntercom(0x16, 1, 5);					// set LCD brightness to max
								break;
							case 5:										// ...metering menu
								eventproc_SetMesMode(&mesmode);					// Spot metering mode
								SHORTCUTSIGNAL
								break;
							case 6:										// ...ISO menu
								eventproc_RiseEvent("SetIsoValue", 0x6f);	// set ISO 3200
								SHORTCUTSIGNAL
								break;
							case 0x13:									// ...WB menu
								SendToIntercom(5, 1, 8);		 			//set WB ColorTemp
								SHORTCUTSIGNAL
						}
					isRelease=1;
					}
				}
				break;
			case MESSAGE_DP:										//direct print button pressed in ...
				if (handleMainMenuDlg) {
					switch (cameraMode.MenuPos){
#ifdef POSITION_AFPOINTS_3
						case 1:											// ...quality menu
							SendToIntercom(7,1,AfPointSet3);				//set AF point set 3
							SHORTCUTSIGNAL
							break;
#endif
#ifdef POSITION_TEST
						case 2:
							EnterFactoryMode();
							Beep(0);
							break;
#endif
						case 3:											// ...beep menu
							SendToIntercom(0x35,1,cameraMode.CfMLU^1);		//toggle mirror lock-up
							if(cameraMode.CfMLU)							//flash red led for enabled, blue for disabled MLU
								LEDBLUE=LEDON;
							else
								LEDRED=LEDON;
							SleepTask(150);
							LEDRED=LEDOFF,LEDBLUE=LEDOFF;
							break;
						case 4:											// ...AF menu
							SendToIntercom(0x16,1,3);						//set LCD brightness to 3
							break;
						case 5:											// ...metering menu
							eventproc_RiseEvent("PressMenuButton", 0);			//go to parameter menu
							eventproc_RiseEvent("PressJumpButton", 0);
							eventproc_RiseEvent("PressUpButton", 0);
							eventproc_RiseEvent("PressSetButton", 0);
							break;
						case 6:											// ...ISO menu
							eventproc_RiseEvent("SetIsoValue", 0x58);		//set ISO 400
							eventproc_RiseEvent("PressMenuButton", 0);		//close menu and
							eventproc_RiseEvent("PressSetButton", 0);		//open menu again to show new setting
							SHORTCUTSIGNAL
							break;
						case 0x13:										// ...WB menu
							SendToIntercom(5,1,6);							// Set WB to manual (using last taken picture as reference)
							eventproc_RiseEvent("PressMenuButton",0);		// go from wb submenu to menu page 2
							eventproc_RiseEvent("PressDownButton",0);
							eventproc_RiseEvent("PressDownButton",0);		//go down to manual wb image selection
							eventproc_RiseEvent("PressSetButton",0);
							eventproc_RiseEvent("PressSetButton",0);		//select last image on card
							eventproc_RiseEvent("PressUpButton",0);
							eventproc_RiseEvent("PressUpButton",0);
							eventproc_RiseEvent("PressSetButton",0);		//go back to WB menu
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
							WriteSettings();
							SendToIntercom(0xd,1,cameraMode.Aeb);
							break;
#endif
#ifdef POSITION_RAWMODE
						case POSITION_RAWMODE-1: // Raw toggle
							SwitchRawMode(pMessage[1]);
							SendToIntercom(0xd,1,cameraMode.Aeb);
							break;
#endif
#ifdef POSITION_AFPOINTS_1
						case POSITION_AFPOINTS_1-1: //AF points set 1 setup
							SetAFPoints(pMessage[1],&AfPointSet1);
							WriteSettings();
							SendToIntercom(0xd,1,cameraMode.Aeb);
							break;
#endif
#ifdef POSITION_AFPOINTS_2
						case POSITION_AFPOINTS_2-1: //AF points set 2 setup
							SetAFPoints(pMessage[1],&AfPointSet2);
							WriteSettings();
							SendToIntercom(0xd,1,cameraMode.Aeb);
							break;
#endif
#ifdef POSITION_AFPOINTS_3
						case POSITION_AFPOINTS_3-1: //AF points set 3 setup
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
							SendToIntercom(3,1,FecHex(fec=CfnVal(pMessage[1],fec,48,0)));
							break;
#endif
#ifdef POSITION_AVCOMP
						case POSITION_AVCOMP-1:
							SendToIntercom(0xa,1,FecHex(avc=CfnVal(pMessage[1],avc,48,0)));
							SendToIntercom(0xd,1,cameraMode.Aeb);
#endif
#ifdef POSITION_FEB
						case POSITION_FEB-1:
							SwitchFEB(pMessage[1]);
							SendToIntercom(0xd,1,cameraMode.Aeb);
							break;
#endif
					}
					if(pMessage[1]==BUTTON_LEFT||pMessage[1]==BUTTON_RIGHT)			//isRelease is only needed for the zoom-in, zoom-out, left, right, up and down buttons
						isRelease=1;												//using it for the other ones too, you need to press them two times
				}
				break;
		}
		MainHeapFree(pMessage);
	}
}
#ifdef POSITION_TEST
void TestTask()
	{
	while(1){
	SleepTask(5000);
	Beep(0);
	}
}
#endif
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
	}

	IntercomHandler(r0, ptr);
}

