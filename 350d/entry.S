// This code only works at the address it is linked for (ADDON_BASE in the
// Makefile), because it gives absolute addresses of add-on code to the CPU
// and to Canon's firmware: LDR PC, =my_romStart, LDR R0, =my_usrRoot and
// LDR R3, =my_task_Startup.

	.text
	.org 0
	.globl _start, start

start:
_start:
		BL      COPY
		LDR     R0, =0xC0220000
		LDR     R1, =0x46
                STR     R1, [R0]
		MOV	R0, #2
		TEQ	R0, #2
		MOV	R1, #0x78
		MCR	p15, 0,	R1,c1,c0
		MOV	R1, #0
		MCR	p15, 0,	R1,c7,c10, 4
		MCR	p15, 0,	R1,c7,c7
		MRS	R1, CPSR
		BIC	R1, R1,	#0x3F
		ORR	R1, R1,	#0xD3
		MSR	CPSR_cf, R1
		LDR	R2, =0xC0200000
		MOV	R1, #0xFFFFFFFF			//MOVL	R1, 0xFFFFFFFF
		STR	R1, [R2,#0x10C]
		STR	R1, [R2,#0xC]
		STR	R1, [R2,#0x1C]
		STR	R1, [R2,#0x2C]
		STR	R1, [R2,#0x3C]
		STR	R1, [R2,#0x4C]
		STR	R1, [R2,#0x5C]
		STR	R1, [R2,#0x6C]
		STR	R1, [R2,#0x7C]
		STR	R1, [R2,#0x8C]
		STR	R1, [R2,#0x9C]
		STR	R1, [R2,#0xAC]
		STR	R1, [R2,#0xBC]
		STR	R1, [R2,#0xCC]
		STR	R1, [R2,#0xDC]
		STR	R1, [R2,#0xEC]
		STR	R1, [R2,#0xFC]
		LDR	R1, =0xC0400000
		LDR	R2, =0x80003
		STR	R2, [R1,#4]
		MOV	R3, #0xF

loc_FF8100D8:
		NOP
		SUB	R3, R3,	#1
		CMP	R3, #0
		BNE	loc_FF8100D8
		MOV	R1, #0xC0000000
		LDR	R2, =0xD9C50000
		STR	R2, [R1,#0x10]
		LDR	R1, =0xC0400000
		LDR	R2, =0x410004
		ORR	R2, R2,	#1
		STR	R2, [R1,#8]
		MOV	R3, #0xF

loc_FF810108:
		NOP
		SUB	R3, R3,	#1
		CMP	R3, #0
		BNE	loc_FF810108
		LDR	R1, =0xC0100000
		LDR	R2, =0x4D3
		STR	R2, [R1]
		LDR	R2, =0x56600
		STR	R2, [R1,#0x1C]
		MOV	R2, #0x16
		STR	R2, [R1,#4]
		LDR	R1, =0xC0400000
		LDR	R2, =0x410004
		ORR	R2, R2,	#1
		STR	R2, [R1,#8]
		MOV	R3, #0x100

loc_FF810148:
		NOP
		SUB	R3, R3,	#1
		CMP	R3, #0
		BNE	loc_FF810148
		LDR	R1, =0xC0100000
		MOV	R2, #0x33
		STR	R2, [R1,#8]
		LDR	R1, =0xC0210600
		LDR	R2, =0x201
		STR	R2, [R1]
		LDR	R1, =0xC0100000
		MOV	R2, #1
		STR	R2, [R1,#0xC]
		LDR	SP, =0x1900
		MOV	R11, #0
		LDR	PC, =my_romStart		// romStart

// ==========================================
//
//my_LedRedOn:
//		LDR	R0, =0xC02200A0
//		LDR	R1, =0x46
//		STR	R1, [R0]
//		MOV	PC, LR
//
//my_LedRedOff:
//		LDR	R0, =0xC02200A0
//		LDR	R1, =0x44
//		STR	R1, [R0]
//		MOV	PC, LR
//
//my_LedBlueOn:
//		LDR	R0, =0xC0220000
//		LDR	R1, =0x46
//		STR	R1, [R0]
//		MOV	PC, LR
//
//my_LedBlueOff:
//		LDR	R0, =0xC0220000
//		LDR	R1, =0x44
//		STR	R1, [R0]
//		MOV	PC, LR

// ==========================================

my_romStart:
		LDR	R2, =0x1D6F0
		STMFD	SP!, {R4,LR}
		MOV	R2, R2,LSR#2
		MOV	R4, R0
		MOV	R1, #0x1900
		LDR	R0, =0xFFA5EBF0
		BL	sub_FF8101EC
		MOV	R0, R4
		LDMFD	SP!, {R4,LR}
		B	my_usrInit			// usrInit

my_usrInit:
		STMFD	SP!, {R4,R5,LR}
		MOV	R4, R0
		SUB	SP, SP,	#8
		LDR	R5, =0x1EFF0
		BL	sysHwInit0
		LDR	R0, =0xD820
		RSB	R1, R0,	R5
		BL	sub_FFA5234C
		LDR	R3, =0x1E080
		MOV	R0, #0
		STR	R4, [R3]
		BL	intVecBaseSet
		MOV	R1, #2
		MOV	R0, #1
		BL	cacheLibInit
		BL	excVecInit
		BL	sysHwInit
		MOV	R0, #0
		BL	cacheEnable
		MOV	R0, #1
		BL	cacheEnable
		BL	classLibInit
		BL	taskLibInit
		LDR	R3, =0xCCD4
		LDR	R2, =0x1E580
		LDR	R1, [R3]
		LDR	R0, =0x1DFC4
		MOV	R3, #0x100
		BL	qInit
		LDR	R3, =0xCC94
		LDR	R0, =0xD034
		LDR	R1, [R3]
		BL	qInit
		LDR	R3, =0xCD50
		LDR	R0, =0x1BC48
		LDR	R1, [R3]
		BL	qInit
		BL	workQInit
		BL	sub_FF810538
		MOV	R4, #0
		MOV	R3, R0
		MOV	R12, #0xC00
		MOV	R2, R5
		LDR	R0, =my_usrRoot			// usrRoot
		MOV	R1, #0x4000
		STR	R12, [SP,#0x14-0x14]
		STR	R4, [SP,#0x14-0x10]
		BL	kernelInit
		ADD	SP, SP,	#8
		LDMFD	SP!, {R4,R5,PC}
		MOV	PC, LR				// RET

my_usrRoot:
		STMFD	SP!, {R4,R5,LR}
		MOV	R4, R1
		MOV	R5, R0
		SUB	SP, SP,	#8
		BL	eventLibInit
		BL	semBLibInit
		BL	semMLibInit
		BL	semCLibInit
		BL	msgQLibInit
		BL	wdLibInit
		BL	taskHookInit
		MOV	R1, R4
		MOV	R0, R5
		BL	memInit
		MOV	R1, R4
		MOV	R0, R5
		BL	memPartLibInit
		LDR	R3, =0xB2F8
		LDR	R3, [R3]
		CMP	R3, #0
		MOV	R0, #0x1000
		BEQ	loc_FF810C90
		MOV	LR, PC
		MOV	PC, R3
		CMP	R0, #0
		MOV	R0, #0x1000
		BNE	loc_FF810C90
		BL	vmMpuLibInit
		CMP	R0, #0
		MOV	R2, #1
		LDR	R0, =0x191C
		BNE	loc_FF810C90
		LDR	R3, =0x1980
		LDR	R1, [R3]
		BL	vmMpuGlobalMapInit
		CMP	R0, #0
		BNE	loc_FF810CB8

loc_FF810C90:
		BL	sub_FFA474AC
		MOV	R2, #0
		LDR	R1, [R0]
		MOV	R3, R2
		LDR	R0, =aUsrrootMmuConf		// "usrRoot: MMU configuration failed, errno = %#x"
		STR	R2, [SP,#0x14-0x14]
		STR	R2, [SP,#0x14-0x10]
		BL	printExc
		MOV	R0, #1
		BL	reboot

loc_FF810CB8:
		MOV	R1, #0
		LDR	R0, =usrClock
		BL	sub_FFA31FB8
		MOV	R0, #0x3C
		BL	sub_FFA31DF8
		BL	sub_FFA32054
		MOV	R0, #0x32
		BL	selectInit
		MOV	R0, #0x1000
		BL	sub_FF810794
		LDR	R2, =0xFF810BF4
		MOV	R1, #0x32
		MOV	R0, #0x14
		BL	iosInit
		BL	ttyDrv
		BL	sub_FF810AAC
		BL	hashLibInit
		MOV	R0, #1
		BL	envLibInit
		BL	sigInit
		BL	excInit
		LDR	R3, =0x19A4
		MOV	R1, #0x32
		LDR	R0, [R3]
		BL	logInit
		BL	stdioInit
		BL	fioLibInit
		BL	selTaskDeleteHookAdd
		BL	cplusCtorsLink
		BL	my_taskcreate_Startup		// taskcreate_Startup
		ADD	SP, SP,	#8
		LDMFD	SP!, {R4,R5,PC}
		MOV	PC, LR				// RET

my_taskcreate_Startup:
//		STR	LR, [SP,#-0x4]!
//		LDR	LR, [SP],#0x4
//		B	loc_FF811508
		STR	LR, [SP,#-0x4]!
		// End Canon's main heap where the add-on starts, so the heap
		// never hands out the add-on's memory. Canon's own end is
		// 0x800000.
		LDR	R1, =ADDON_BASE
		MOV	R0, #0x200000
		SUB	SP, SP, #4
		BL	CreateMainHeap
		BL	InitializeKernel
		MOV	R0, #0x4000			// MOVL	R0, 0x386D4000
		ADD	R0, R0, #0x6D0000
		ADD	R0, R0, #0x38000000		// R0 = 0x4000 + 0x386D0000 = 0x386D4000
		ADD	R0, R0, #0x380
		BL	sub_FFA2F4B8
		BL	sub_FFA1E164
		BL	CreateMyTask
		MOV	R12, #0
		MOV	R1, #0x19
		MOV	R2, #0x2000
		LDR	R3, =my_task_Startup		// task_Startup
		LDR	R0, =aStartup			// "Startup"
		STR	R12, [SP,#4-0x4]
		BL	CreateTask
		ADD	SP, SP, #4
		LDR	PC, [SP],#0x4
		MOV	PC, LR				// RET

my_task_Startup:
		STR	LR, [SP,#-4]!
		SUB	SP, SP,	#4
		BL	sub_FFA15E0C
		BL	FlushWriteCache
		BL	sub_FFA1898C
		BL	ShutdownInit
		MOV	R0, #0xF8000000
		BL	sub_FFA2FA9C
		BL	sub_FFA2C58C
		BL	sub_FFA295A8
		BL	sub_FFA295F4
		BL	sub_FFA187E8
		BL	sub_FFA181E8
		BL	InitializeIC_OC
		MOV	R0, #0x32
		BL	InitializeTimerSystem
		LDR	R2, =0x1BC4
		MOV	R1, #8
		MOV	R0, #0x10
		BL	InitializeHPTimerSystem
		BL	sub_FFA23BA8
		MOV	R0, #0
		BL	sub_FFA20D4C
		MOV	R0, #3
		BL	sub_FFA2E25C
		BL	sub_FFA2DE80
		BL	sub_FFA2DD20
		MOV	R0, #0x4000			// MOVL	R0, 0x386D4000
		ADD	R0, R0, #0x6D0000
		ADD	R0, R0, #0x38000000		// R0 = 0x4000 + 0x386D0000 = 0x386D4000
		ADD	R0, R0,	#0x380
		BL	sub_FFA0FD14
		BL	AdjustDefect
		BL	sub_FF925D58
		MOV	R1, #0
		LDR	R0, =sub_FFA2FEB8
		BL	sub_FFA2F78C
		BL	my_InitializeIntercom		// InitializeIntercom
		BL	sub_FFA119B0
		BL	sub_FF959B68
		BL	sub_FF94FD98
		BL	sub_FF952878
		MOV	R0, #0
		BL	sub_FFA2FFE8
		MOV	R0, #1
		BL	sub_FFA2E008
		BL	sub_FFA131C8
		BL	sub_FFA14844
		BL	FlyingInit
		BL	sub_FF9221F0
		BL	DigitalGainInit
		BL	sub_FF95CB04
		BL	sub_FF95AEF4
		BL	sub_FF9232C8
		BL	LoadSystemInfo
		MOV	R1, #0x14000000
		MOV	R2, #0x2F0000
		MOV	R3, #0xBC0000
		MOV	R0, #0x10800000
		BL	sub_FF89FEDC
		BL	sub_FF8A15B8
		BL	sub_FF8A448C
		BL	sub_FF89C784
		BL	taskcreate_DigProp
		BL	taskcreate_ShootMain
		BL	taskcreate_CardDoor
		BL	taskcreate_MainCtrl
		BL	DigitalGainForDarkInit
		BL	sub_FF927268
		BL	sub_FF96637C
		BL	sub_FF9672F8
		BL	sub_FF9273D0
		BL	sub_FF969018
		BL	sub_FF968948
		BL	sub_FF925350
		BL	sub_FF969870
		BL	DefsInit
		BL	WBDetectionInit
		BL	sub_FF926AE0
		BL	DevelopParamInit
		BL	sub_FF940488
		BL	HuffmanInit
		BL	sub_FF93E5D0
		BL	sub_FF961638
		BL	sub_FF960978
		BL	sub_FF95FC24
		BL	sub_FF963DB0
		BL	sub_FF967D38
		BL	sub_FF9628F4
		BL	sub_FF96208C
		BL	sub_FF9631F0
		BL	sub_FF95EE8C
		BL	sub_FF964E8C
		BL	sub_FF968520
		BL	sub_FF953064
		BL	sub_FF954E0C
		BL	sub_FF956098
		BL	sub_FF956784
		BL	sub_FF956FEC
		BL	sub_FF95907C
		BL	sub_FF955854
		BL	sub_FF95B600
		BL	sub_FF942974
		MOV	R12, #0x1C0000
		MOV	R3, #0x40000
		MOV	R1, #0x1F
		MOV	R2, #0x180000
		MOV	R0, #1
		STR	R12, [SP]
		BL	sub_FFA26324
		BL	sub_FFA264DC
		BL	sub_FFA26A0C
		BL	sub_FFA3051C
		BL	EdLedInit
		BL	sub_FFA13A10
		BL	sub_FFA134E8
		BL	sub_FF930E70
		BL	sub_FF930A84
		BL	loc_FF9287C8
		BL	sub_FF929DAC
		BL	sub_FF9374C4
		BL	sub_FFA2C4B0
		BL	sub_FFA11370
		BL	sub_FF89D8E4
		BL	BootDiskInit
		BL	FactoryModeInit
		BL	sub_FF87C4D0
		BL	sub_FF8763BC
		BL	sub_FF8769CC
		BL	sub_FF87A6D4
		BL	sub_FF893074
		BL	sub_FF879C64
		BL	sub_FF879B6C
		BL	sub_FF88438C
		BL	sub_FF886AF8
		BL	sub_FF8811C8
		BL	sub_FF896468
		BL	sub_FF889E94
		BL	sub_FF89A454
		BL	sub_FF895418
		BL	taskcreate_AEBDeletetingTask
		BL	taskcreate_PopupMenuDeleteting
		BL	taskcreate_ColorTempDeleteting
		MOV	R0, #0x23
		BL	sub_FF87FAC0
		BL	sub_FF88ABBC
		BL	sub_FF882858
		BL	sub_FF8806E4
		BL	sub_FF8943D8
		BL	sub_FF88C008
		BL	sub_FF8834A8
		BL	sub_FF8834A8
		BL	my_GUIInit
		BL	sub_FF82D80C
		BL	sub_FF839394
		BL	ColorBarInit
		BL	LCDAdjustInit
		BL	sub_FF82D15C
		LDR	R3, =0xAE88
		MOV	R2, #0x7B
		LDR	R0, [R3]
		LDR	R1, =aUsbRootDeviceH		// "USB ROOT DEVICE HANDLE"
		BL	RegisterName
		MOV	R0, #0xB8			// MOVL	R0, 0xC00000B8
		ADD	R0, R0, #0xC0000000		// R0 = 0xB8 + 0xC0000000 = 0xC00000B8
		LDR	R1, =aU2vbus			// "U2VBUS"
		ADD	R0, R0,	#0x220000
		BL	sub_FF99861C
		LDR	R1, =aUsbc20VbusSupp		// "USBC20 VBUS SUPPORT"
		MOV	R0, #1
		BL	sub_FF99861C
		LDR	R1, =aDeviceSpeed		// "DEVICE SPEED"
		MOV	R0, #0x14
		BL	sub_FF99861C
		BL	sub_FF99F6E0
		BL	sub_FF99DEE8
		BL	sub_FF99DF60
		BL	sub_FF99F8C4
		BL	sub_FF9A78C8
		BL	sub_FF98104C
		BL	sub_FF998CD4
		BL	sub_FF98980C
		BL	sub_FF981138
		BL	sub_FF991604
		BL	sub_FF98B2C8
		BL	sub_FF984A94
		BL	sub_FF8B8DD4
		BL	sub_FF9936D0
		MOV	R2, #0
		MOV	R1, #0x1B
		MOV	R3, R2
		MOV	R0, #1
		BL	sub_FF8C5054
		BL	sub_FF8BE060
		BL	sub_FF986064
		BL	sub_FF9907A4
		BL	sub_FF90FF84
		BL	sub_FF90FB9C
		BL	sub_FF981138
		BL	sub_FF8B4D80
		BL	sub_FF9807F4
		BL	sub_FF981388
		BL	sub_FF8B51E0
		BL	sub_FF8B4F4C
		BL	USBModeInit
		BL	sub_FF901D28
		BL	sub_FF993708
		BL	sub_FF8B74B8
		BL	sub_FF8B6EE0
		BL	sub_FF8FF46C
		BL	sub_FF90E414
		BL	sub_FF9987FC
		BL	TransMemoryInit
		BL	sub_FF8BAFC8
		BL	sub_FF816A88
		BL	sub_FF811754
		BL	sub_FF8A7388
		BL	sub_FF8A3F18
		BL	sub_FFA28038
		ADD	SP, SP,	#4
		LDR	PC, [SP],#4
		MOV     PC, LR				// RET

.align 2
fin:
