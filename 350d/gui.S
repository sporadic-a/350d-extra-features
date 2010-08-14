	.text
	.org 0
	.globl my_GUIInit, my_ShowInfoCreativeDlg

my_GUIInit:
		STMFD	SP!, {R4-R6,LR}
		LDR	R12, =0x20C8
		LDR	R2, =0x20C4
		LDR	R3, =0x20C0
		MOV	R6, #0
		SUB	SP, SP,	#0x18
		MOV	R1, #1
		LDR	R0, =aGuiswsem
		STR	R6, [R12]
		STR	R6, [R2]
		STR	R6, [R3]
		BL	sub_FFA1D6C8
		LDR	R3, =0xE828
		LDR	R5, =aForeground
		STR	R0, [R3]
		MOV	R0, #1
		BL	sub_FF92E008
		LDR	R0, =aForrec
		BL	sub_FF87CDFC
		MOV	R0, R5
		BL	sub_FF87CD70
		MOV	R4, R0
		MOV	R0, R5
		BL	sub_FF87CCEC
		MOV	R5, R0
		MOV	R0, R4
		BL	sub_FF92AB74
		BL	sub_FF92E89C
		LDR	R0, =aHcanongothic
		ADD	R1, SP,	#0x28-0x14
		ADD	R2, SP,	#0x28-0x18
		BL	sub_FFA295CC
		LDR	R12, =sub_FF9309AC
		LDR	R0, [SP,#0x28-0x14]
		LDR	R1, =sub_FF930864
		LDR	R2, =sub_FF9309E0
		LDR	R3, =sub_FF930A2C
		STR	R12, [SP,#0x28-0x28]
		BL	sub_FF92B69C
		LDR	R3, =0x20D0
		STR	R0, [R3]
		BL	sub_FF92D6CC
		ADD	R1, SP,	#0x28-0x1C
		ADD	R2, SP,	#0x28-0x20
		BL	sub_FF92D678
		LDR	R12, =sub_FF836408
		LDR	R3, [SP,#0x28-0x20]
		LDR	R2, [SP,#0x28-0x1C]
		MOV	R1, R6
		MOV	R0, R6
		STR	R12, [SP,#0x28-0x28]
		STR	R5, [SP,#0x28-0x24]
		BL	sub_FF92F818
		LDR	R3, =0x20CC
		STR	R0, [R3]
		BL	sub_FF92FC10
		BL	taskcreate_GuiMainTask
		BL	sub_FF83A444
		BL	taskcreate_tGraphics
		BL	createMessageQueue_QRevImgQueue
		BL	SetFocusLensDefaultCondition
		MOV	R1, R6
		LDR	R0, =my_IDLEHandler
		BL	sub_FF875D90
		BL	sub_FF83DB58
		BL	sub_FF836330
		ADD	SP, SP,	#0x18
		LDMFD	SP!, {R4-R6,PC}

my_IDLEHandler:
		STMFD	SP!, {R4,LR}
		MOV	R4, R1
		SUB	R4, R4,	#0x800
		MOV	R0, #0x18
		MOV	R1, #5
		LDR	R2, =aGuiInIdlehandl
		SUB	R4, R4,	#1
		BL	sub_FFA265AC
		CMP	R4, #0x86	// switch 135 cases
		LDRLS	PC, [PC,R4,LSL#2] // switch jump
		B	loc_FF8351C4	// default
					// jumptable FF834B20 cases 1,3-10,12-29,31-102,104-109

		.word loc_FF834D5C, loc_FF8351C4,	loc_FF834D44, loc_FF8351C4 // jump table	for switch statement
		.word loc_FF8351C4, loc_FF8351C4,	loc_FF8351C4, loc_FF8351C4
		.word loc_FF8351C4, loc_FF8351C4,	loc_FF8351C4, label_requestMenu
		.word loc_FF8351C4, loc_FF8351C4,	loc_FF8351C4, loc_FF8351C4
		.word loc_FF8351C4, loc_FF8351C4,	loc_FF8351C4, loc_FF8351C4
		.word loc_FF8351C4, loc_FF8351C4,	loc_FF8351C4, loc_FF8351C4
		.word loc_FF8351C4, loc_FF8351C4,	loc_FF8351C4, loc_FF8351C4
		.word loc_FF8351C4, loc_FF8351C4,	label_requestPlay, loc_FF8351C4
		.word loc_FF8351C4, loc_FF8351C4,	loc_FF8351C4, loc_FF8351C4
		.word loc_FF8351C4, loc_FF8351C4,	loc_FF8351C4, loc_FF8351C4
		.word loc_FF8351C4, loc_FF8351C4,	loc_FF8351C4, loc_FF8351C4
		.word loc_FF8351C4, loc_FF8351C4,	loc_FF8351C4, loc_FF8351C4
		.word loc_FF8351C4, loc_FF8351C4,	loc_FF8351C4, loc_FF8351C4
		.word loc_FF8351C4, loc_FF8351C4,	loc_FF8351C4, loc_FF8351C4
		.word loc_FF8351C4, loc_FF8351C4,	loc_FF8351C4, loc_FF8351C4
		.word loc_FF8351C4, loc_FF8351C4,	loc_FF8351C4, loc_FF8351C4
		.word loc_FF8351C4, loc_FF8351C4,	loc_FF8351C4, loc_FF8351C4
		.word loc_FF8351C4, loc_FF8351C4,	loc_FF8351C4, loc_FF8351C4
		.word loc_FF8351C4, loc_FF8351C4,	loc_FF8351C4, loc_FF8351C4
		.word loc_FF8351C4, loc_FF8351C4,	loc_FF8351C4, loc_FF8351C4
		.word loc_FF8351C4, loc_FF8351C4,	loc_FF8351C4, loc_FF8351C4
		.word loc_FF8351C4, loc_FF8351C4,	loc_FF8351C4, loc_FF8351C4
		.word loc_FF8351C4, loc_FF8351C4,	loc_FF8351C4, loc_FF8351C4
		.word loc_FF8351C4, loc_FF8351C4,	loc_FF8351C4, loc_FF8351C4
		.word loc_FF8351C4, loc_FF8351C4,	loc_FF8351C4, loc_FF8351C4
		.word loc_FF8351C4, loc_FF8351C4,	loc_FF8351C4, label_requestInfo
		.word loc_FF8351C4, loc_FF8351C4,	loc_FF8351C4, loc_FF8351C4
		.word loc_FF8351C4, loc_FF8351C4,	label_startMenuMode, label_startInfoMode
		.word label_startPlayMode, loc_FF834FA8, loc_FF834FFC, loc_FF83502C
		.word loc_FF83505C, loc_FF83508C,	loc_FF8350C0, loc_FF8350F4
		.word loc_FF835128, loc_FF83515C,	loc_FF835190, label_startIdleMode
		.word loc_FF834FEC, loc_FF834FF4,	loc_FF834E30, loc_FF834E4C
		.word loc_FF834E68, loc_FF834E84,	loc_FF834E98, loc_FF834EA0
		.word loc_FF834EA8, loc_FF834EB0,	loc_FF834EB8

loc_FF834D44:
		MOV	R0, #0x18	// jumptable FF834B20 case 2
		MOV	R1, #5
		LDR	R2, =aGuiIdlehandler
		BL	sub_FFA265AC

loc_FF834D54:
		MOV	R0, #1
		LDMFD	SP!, {R4,PC}

loc_FF834D5C:
		MOV	R1, #5		// jumptable FF834B20 case 0
		LDR	R2, =aGuiIdlehandl_0
		MOV	R0, #0x18
		BL	sub_FFA265AC
		BL	sub_FF83D018
		BL	sub_FF83D2E0
		BL	sub_FF83ED50
		BL	sub_FF841180
		BL	sub_FF840E8C
		BL	sub_FF842274
		BL	CancelExecuteEventHook
		BL	sub_FF93023C
		BL	sub_FF9301A0
		B	loc_FF834D54

label_requestMenu:
		LDR	R2, =aGuiRequestMenu // jumptable FF834B20 case 11
		MOV	R0, #0x18
		MOV	R1, #5
		BL	sub_FFA265AC
		LDR	R3, =0x20C0
		LDR	R2, [R3]
		CMP	R2, #2
		MOV	R0, #0
		BEQ	loc_FF834E88
		MOV	R0, #2
		MOV	R1, #0
		BL	sub_FF836098
		B	loc_FF834E90

label_requestInfo:
		LDR	R2, =aGuiRequestInfo // jumptable FF834B20 case 103
		MOV	R0, #0x18
		MOV	R1, #5
		BL	sub_FFA265AC
		LDR	R3, =0x20C0
		LDR	R2, [R3]
		CMP	R2, #4
		MOV	R0, #0
		BEQ	loc_FF834E88
		MOV	R0, #4
		MOV	R1, #0
		BL	sub_FF836098
		B	loc_FF834E90

label_requestPlay:
		LDR	R2, =aGuiRequestPlay // jumptable FF834B20 case 30
		MOV	R0, #0x18
		MOV	R1, #5
		BL	sub_FFA265AC
		LDR	R3, =0x20C0
		LDR	R2, [R3]
		CMP	R2, #1
		MOV	R0, #0
		BEQ	loc_FF834E88
		MOV	R0, #1
		MOV	R1, #0
		BL	sub_FF836098
		B	loc_FF834E90

loc_FF834E30:
		LDR	R3, =0x20C0	// jumptable FF834B20 case 126
		LDR	R1, [R3]
		CMP	R1, #0
		BNE	loc_FF834E90
		MOV	R0, #5
		BL	sub_FF836098
		B	loc_FF834E90

loc_FF834E4C:
		LDR	R3, =0x20C0	// jumptable FF834B20 case 127
		LDR	R1, [R3]
		CMP	R1, #0
		BNE	loc_FF834E90
		MOV	R0, #7
		BL	sub_FF836098
		B	loc_FF834E90

loc_FF834E68:
		LDR	R3, =0x20C0	// jumptable FF834B20 case 128
		LDR	R1, [R3]
		CMP	R1, #0
		BNE	loc_FF834E90
		MOV	R0, #3
		BL	sub_FF836098
		B	loc_FF834E90

loc_FF834E84:
		MOV	R0, #8		// jumptable FF834B20 case 129

loc_FF834E88:
		MOV	R1, #0
		BL	sub_FF836098

loc_FF834E90:
		MOV	R0, #0
		LDMFD	SP!, {R4,PC}

loc_FF834E98:
		MOV	R0, #0xA	// jumptable FF834B20 case 130
		B	loc_FF834E88

loc_FF834EA0:
		MOV	R0, #9		// jumptable FF834B20 case 131
		B	loc_FF834E88

loc_FF834EA8:
		MOV	R0, #0xB	// jumptable FF834B20 case 132
		B	loc_FF834E88

loc_FF834EB0:
		MOV	R0, #0xC	// jumptable FF834B20 case 133
		B	loc_FF834E88

loc_FF834EB8:
		MOV	R0, #0xD	// jumptable FF834B20 case 134
		B	loc_FF834E88

label_startIdleMode:
		MOV	R1, #5		// jumptable FF834B20 case 123
		LDR	R2, =aGuiStartIdleMo
		MOV	R0, #0x18
		BL	sub_FFA265AC
		BL	sub_FF85DCA0
		BL	sub_FF83D018
		BL	sub_FF83ED50
		BL	sub_FF841180
		BL	sub_FF840E8C
		BL	sub_FF842274
		MOV	R0, #0
		BL	sub_FF836174
		BL	sub_FF84051C
		LDR	R3, =0x20C0
		MOV	R0, #0
		STR	R0, [R3]
		LDMFD	SP!, {R4,PC}

label_startMenuMode:
		MOV	R1, #5		// jumptable FF834B20 case 110
		LDR	R2, =aGuiStartMenuMo
		MOV	R0, #0x18
		BL	sub_FFA265AC
		BL	sub_FF930250
		BL	sub_FF930164
		MOV	R0, #2
		BL	sub_FF836174
		LDR	R3, =0x20C0
		LDR	R0, [R3]

loc_FF834F2C:
		BL	sub_FF840F24

loc_FF834F30:
		BL	sub_FF93023C
		BL	sub_FF9301A0
		BL	sub_FF83D2E0

loc_FF834F3C:
		BL	sub_FF85DC78
		B	loc_FF834E90

label_startInfoMode:
		MOV	R1, #5		// jumptable FF834B20 case 111
		LDR	R2, =aGuiStartInfoMo
		MOV	R0, #0x18
		BL	sub_FFA265AC
		BL	sub_FF930250
		BL	sub_FF930164
		MOV	R0, #4
		BL	sub_FF836174
		LDR	R3, =0x20C0
		LDR	R0, [R3]
		BL	my_StartInfoMode
		B	loc_FF834F30

label_startPlayMode:
		MOV	R1, #5		// jumptable FF834B20 case 112
		LDR	R2, =aGuiStartPlayMo
		MOV	R0, #0x18
		BL	sub_FFA265AC
		BL	sub_FF930250
		BL	sub_FF930164
		MOV	R0, #1
		BL	sub_FF836174
		LDR	R0, =VirtualHeadError
		BL	sub_FF840618
		BL	sub_FF93023C
		BL	sub_FF9301A0
		B	loc_FF834F3C

loc_FF834FA8:
		MOV	R1, #5		// jumptable FF834B20 case 113
		LDR	R2, =aGuiStartQrMode
		MOV	R0, #0x18
		BL	sub_FFA265AC
		BL	sub_FF930250
		BL	sub_FF930164
		MOV	R0, #6
		BL	sub_FF836174
		LDR	R3, =0x20C0
		LDR	R0, [R3]
		BL	sub_FF840A34
		MOV	R4, R0
		BL	sub_FF93023C
		BL	sub_FF9301A0
		CMP	R4, #0
		BNE	loc_FF834D54
		B	loc_FF834F3C

loc_FF834FEC:
		BL	sub_FF840CB8	// jumptable FF834B20 case 124
		B	loc_FF834E90

loc_FF834FF4:
		BL	sub_FF840BF0	// jumptable FF834B20 case 125
		B	loc_FF834E90

loc_FF834FFC:
		MOV	R1, #5		// jumptable FF834B20 case 114
		LDR	R2, =aGuiStartNocfWa
		MOV	R0, #0x18
		BL	sub_FFA265AC
		BL	sub_FF930250
		BL	sub_FF930164
		MOV	R0, #5
		BL	sub_FF836174
		BL	sub_FF8405D8
		BL	sub_FF93023C
		BL	sub_FF9301A0
		B	loc_FF834F3C

loc_FF83502C:
		MOV	R1, #5		// jumptable FF834B20 case 115
		LDR	R2, =aGuiStartFolder
		MOV	R0, #0x18
		BL	sub_FFA265AC
		BL	sub_FF930250
		BL	sub_FF930164
		MOV	R0, #7
		BL	sub_FF836174
		BL	sub_FF8405F8
		BL	sub_FF93023C
		BL	sub_FF9301A0
		B	loc_FF834F3C

loc_FF83505C:
		MOV	R1, #5		// jumptable FF834B20 case 116
		LDR	R2, =aGuiStartRtcset
		MOV	R0, #0x18
		BL	sub_FFA265AC
		BL	sub_FF930250
		BL	sub_FF930164
		MOV	R0, #3
		BL	sub_FF836174
		BL	sub_FF841258
		BL	sub_FF93023C
		BL	sub_FF9301A0
		B	loc_FF834F3C

loc_FF83508C:
		MOV	R1, #5		// jumptable FF834B20 case 117
		LDR	R2, =aGuiStartMenuIs
		MOV	R0, #0x18
		BL	sub_FFA265AC
		BL	sub_FF930250
		BL	sub_FF930164
		LDR	R2, =0x20C0
		MOV	R3, #2
		MOV	R0, R3
		STR	R3, [R2]
		BL	sub_FF836174
		MOV	R0, #8
		B	loc_FF834F2C

loc_FF8350C0:
		MOV	R1, #5		// jumptable FF834B20 case 118
		LDR	R2, =aGuiStartMenuIs
		MOV	R0, #0x18
		BL	sub_FFA265AC
		BL	sub_FF930250
		BL	sub_FF930164
		LDR	R2, =0x20C0
		MOV	R3, #2
		MOV	R0, R3
		STR	R3, [R2]
		BL	sub_FF836174
		MOV	R0, #0xA
		B	loc_FF834F2C

loc_FF8350F4:
		MOV	R1, #5		// jumptable FF834B20 case 119
		LDR	R2, =aGuiStartMenuIs
		MOV	R0, #0x18
		BL	sub_FFA265AC
		BL	sub_FF930250
		BL	sub_FF930164
		LDR	R2, =0x20C0
		MOV	R3, #2
		MOV	R0, R3
		STR	R3, [R2]
		BL	sub_FF836174
		MOV	R0, #9
		B	loc_FF834F2C

loc_FF835128:
		MOV	R1, #5		// jumptable FF834B20 case 120
		LDR	R2, =aGuiStartMenuIs
		MOV	R0, #0x18
		BL	sub_FFA265AC
		BL	sub_FF930250
		BL	sub_FF930164
		LDR	R2, =0x20C0
		MOV	R3, #2
		MOV	R0, R3
		STR	R3, [R2]
		BL	sub_FF836174
		MOV	R0, #0xB
		B	loc_FF834F2C

loc_FF83515C:
		MOV	R1, #5		// jumptable FF834B20 case 121
		LDR	R2, =aGuiStartMenuQu
		MOV	R0, #0x18
		BL	sub_FFA265AC
		BL	sub_FF930250
		BL	sub_FF930164
		LDR	R2, =0x20C0
		MOV	R3, #2
		MOV	R0, R3
		STR	R3, [R2]
		BL	sub_FF836174
		MOV	R0, #0xC
		B	loc_FF834F2C

loc_FF835190:
		MOV	R1, #5		// jumptable FF834B20 case 122
		LDR	R2, =aGuiStartMenuGe
		MOV	R0, #0x18
		BL	sub_FFA265AC
		BL	sub_FF930250
		BL	sub_FF930164
		LDR	R2, =0x20C0
		MOV	R3, #2
		MOV	R0, R3
		STR	R3, [R2]
		BL	sub_FF836174
		MOV	R0, #0xD
		B	loc_FF834F2C

loc_FF8351C4:
		MOV	R0, #1		// default
					// jumptable FF834B20 cases 1,3-10,12-29,31-102,104-109
		LDMFD	SP!, {R4,PC}

my_StartInfoMode:
		STMFD	SP!, {R4,LR}
		SUB	SP, SP,	#0x2D0
		ADD	R4, SP,	#0x2D8-0x178
		MOV	R0, R4
		BL	sub_FF83A3A8
		MOV	R2, #0x160
		MOV	R0, SP
		ADD	R1, SP,	#0x2D8-0x168
		BL	memcpy
		LDMIA	R4, {R0-R3}
		BL	sub_FF843DFC
		LDR	R3, =0xFDF4
		MOVS	R2, R0,LSL#16
		STRH	R0, [R3]
		BNE	loc_FF840E34
		BL	my_ShowInfoCreativeDlg
		B	loc_FF840E38

loc_FF840E34:
		BL	ShowInfoAutoDlg

loc_FF840E38:
		MOV	R1, #0
		MOV	R0, #7
		BL	sub_FF847510
		LDR	R1, =sub_FF840EB0
		LDR	R2, [R0]
		LDR	R3, =0xFDFC
		MOV	R0, #0x3E8
		STR	R2, [R3]
		MOV	R3, #0
		MOV	R2, R1
		BL	SetTimerAfter
		LDR	R3, =0xFDF8
		STR	R0, [R3]
		MOV	R0, #0
		ADD	SP, SP,	#0x2D0
		LDMFD	SP!, {R4,PC}

my_ShowInfoCreativeDlg:
		STMFD	SP!, {R4,R5,LR}
		LDR	R4, =0x2304
		LDR	R3, [R4]
		SUB	SP, SP, #4
		CMP	R3, #0
		BEQ	loc_FF84E84C
		BL	sub_FF84E8DC

loc_FF84E84C:
		MOV	R12, #0
		MOV	R0, R12
		MOV	R1, R0
		LDR	R2, =sub_FF84E910
		LDR	R3, =aInfocreative
		STR	R12, [SP,#0x10-0x10]
		BL	sub_FF876FE8
		MOV	R3, R0
		ANDS	R5, R3, #1
		MOV	R0, #9
		LDR	R1, =a__GuiInfoappIn
		MOV	R2, #0x47
		STR	R3, [R4]
		BEQ	loc_FF84E894
		LDR	R0, =a0_8
		BL	sub_FFA2F734
		LDR	R0, [R4]
		B	loc_FF84E8B4

loc_FF84E894:
		BL	sub_FF8421E8
		LDR	R1, =my_UpdateInfoDlgProc2
		LDR	R2, [R4]
		LDR	R0, =my_UpdateInfoDlgProc
		BL	UpdateInfoDlg
		LDR	R0, [R4]
		BL	sub_FF878A38
		MOV	R0, R5

loc_FF84E8B4:
		ADD	SP, SP, #4
		LDMFD	SP!, {R4,R5,PC}

my_UpdateInfoDlgProc:
		SUB	SP, SP, #0x10
		STMFD	SP!, {R4-R8,R10,LR}
		SUB	SP, SP, #0x164
		ADD	R12, SP, #0x190-0x10
		STMIA	R12, {R0-R3}
		LDR	R7, =0xFF84E278
		LDRH	R3, [R7]
		CMP	R3, #0
		BEQ	loc_FF84E368
		ADD	R10, SP, #0x190+0x170
		ADD	R8, SP, #0x190+0x164

loc_FF84E308:
		LDRH	R3, [R7,#2]
		CMP	R3, #0
		LDR	R6, =0x2304
		MOV	R0, SP
		MOV	R1, R10
		MOV	R2, #0x164
		BEQ	loc_FF84E354
		MOV	R4, R3
		LDR	R6, [R6]
		LDRH	R5, [R7]
		BL	memcpy
		LDMIA	R8, {R1-R3}
		MOV	R0, R4
		BL	sub_FF843D00
		MOV	R2, R0,LSL#16
		MOV	R1, R5
		MOV	R0, R6
		MOV	R2, R2,LSR#16
		BL	sub_FF841880

loc_FF84E354:
		ADD	R7, R7,	#4
		LDRH	R3, [R7]
		CMP	R3, #0
		BNE	loc_FF84E308
		B	loc_FF84E370

loc_FF84E368:
		ADD	R10, SP, #0x190+0x170
		ADD	R8, SP,	#0x190+0x164

loc_FF84E370:
		LDR	R5, =0x2304
		MOV	R0, SP
		MOV	R1, R10
		MOV	R2, #0x164
		LDR	R4, [R5]
		BL	memcpy
		LDMIA	R8, {R1-R3}
		// MOV	R0, #0x3A
		// BL	GetInfoString
		LDR	R0, [SP, #0x20-0x04]
		BL	my_GetISOString
		// LDR	R0, =0xFE18
		// LDR	R1, =aH
		// BL	strcpy
		// LDR	R0, =0xFE18
		// LDR	R0, =aH
		MOV	R1, #0x19
		MOV	R2, R0
		MOV	R0, R4
		BL	sub_FF8417D0

		ADD	R6, SP,	#0x190+0x174
		MOV	R0, SP
		MOV	R1, R6
		MOV	R2, #0x160
		LDR	R4, [R5]
		BL	memcpy
		LDMIA	R8, {R0-R3}
		BL	GetWBString
		MOV	R1, #0x16
		MOV	R2, R0
		MOV	R0, R4
		BL	sub_FF8417D0  // related to WB String

		MOV	R1, R10
		MOV	R0, SP
		MOV	R2, #0x164
		LDR	R4, [R5]
		BL	memcpy
		LDMIA	R8, {R1-R3}
		MOV	R0, #0x18
		BL	sub_FF842D2C //related to Date Time String
		MOV	R0, R0,LSL#16
		MOV	R1, #0
		MOV	R0, R0,LSR#16
		BL	sub_FF847510	//related to Date Time String
		MOV	R1, #0x15
		MOV	R2, R0
		MOV	R0, R4
		BL	sub_FF8417D0 	//related to Date Time String

		MOV	R1, R6
		MOV	R0, SP
		MOV	R2, #0x160
		LDR	R4, [R5]
		BL	memcpy
		LDMIA	R8, {R0-R3}
		BL	sub_FF843DC8 	// related to CF card Storage string
		MOV	R1, #0x18
		MOV	R2, R0
		MOV	R0, R4
		BL	sub_FF8417D0 	// related to CF card storage string

		MOV	R1, R10
		MOV	R0, SP
		MOV	R2, #0x164
		LDR	R4, [R5]
		BL	memcpy
		LDMIA	R8, {R1-R3}
		MOV	R0, #0x31
		BL	GetInfoString
		MOV	R1, #6
		MOV	R2, R0
		MOV	R0, R4
		BL	sub_FF8419F8

		MOV	R1, R6
		MOV	R0, SP
		MOV	R2, #0x160
		LDR	R4, [R5]
		BL	memcpy
		LDMIA	R8, {R0-R3}
		BL	sub_FF842AE0 	// related to parameter 
		MOV	R1, #0x17 
		MOV	R2, R0
		MOV	R0, R4
		BL	sub_FF8417D0

		MOV	R1, R6 // START - add by cyrustam for Current Setting string
		MOV	R0, SP
		MOV	R2, #0x160
		LDR	R4, [R5]
		BL	memcpy
		LDMIA	R8, {R0-R3}
		BL	my_GetCurSetString 	// 
		MOV	R1, #0x10
		MOV	R2, R0
		MOV	R0, R4
		BL	sub_FF8417D0 	 // END - add by cyrustam

		MOV	R1, R6 // START - add by cyrustam for Color Temp String
		MOV	R0, SP
		MOV	R2, #0x160
		LDR	R4, [R5]
		BL	memcpy
		LDMIA	R8, {R0-R3}
		BL	my_GetColorTempString 	// 
		MOV	R1, #0x14
		MOV	R2, R0
		MOV	R0, R4
		BL	sub_FF8417D0 	 // END - add by cyrustam

		MOV	R1, R6 // START - add by cyrustam for dynamic String
		MOV	R0, SP
		MOV	R2, #0x160
		LDR	R4, [R5]
		BL	memcpy
		LDMIA	R8, {R0-R3}
		BL	my_GetDynamicString 	// 
		MOV	R1, #0x12
		MOV	R2, R0
		MOV	R0, R4
		BL	sub_FF8417D0 	 // END - add by cyrustam

		MOV	R1, R6 // start
		MOV	R0, SP
		MOV	R2, #0x160
		LDR	R4, [R5]
		BL	memcpy
		LDMIA	R8, {R0-R3}
		//BL	sub_FF843FE0 	// related to AEB
		BL	my_GetAEBString 	// related to AEB
		MOV	R1, #0xC
		MOV	R2, R0
		MOV	R0, R4
		//BL	sub_FF841ED0 	// related to AEB
		BL	sub_FF8417D0 	// related to AEB // END

		ADD	SP, SP,	#0x164
		LDMFD	SP!, {R4-R8,R10,LR}
		ADD	SP, SP,	#0x10
		MOV	PC, LR				// RET

my_UpdateInfoDlgProc2:
		SUB	SP, SP,	#0x10
		STMFD	SP!, {R4-R10,LR}
		LDR	R7, =0xFF84E278
		SUB	SP, SP,	#0x168
		ADD	R8, SP,	#0x198-0x10
		STMIA	R8, {R0-R3}
		LDRH	R3, [R7]
		CMP	R3, #0
		MOV	R10, #0
		BEQ	loc_FF84E59C
		ADD	R9, SP,	#0x198-0x4

loc_FF84E50C:
		LDRH	R3, [R7,#2]
		CMP	R3, #0
		MOV	R0, SP
		MOV	R1, R9
		MOV	R2, #0x168
		BEQ	loc_FF84E588
		MOV	R4, R3
		BL	memcpy
		LDMIA	R8, {R1-R3}
		MOV	R0, R4
		BL	sub_FF8432AC
		MOVS	R0, R0,LSL#16
		LDR	R6, =0x2304
		ADD	R1, SP,	#0x198+0x170
		MOV	R2, #0x164
		MOV	R0, SP
		BEQ	loc_FF84E588
		LDR	R6, [R6]
		LDRH	R5, [R7]
		LDRH	R4, [R7,#2]
		BL	memcpy
		ADD	R12, SP, #0x198+0x164
		LDMIA	R12, {R1-R3}
		MOV	R0, R4
		BL	sub_FF843D00
		MOV	R2, R0,LSL#16
		MOV	R1, R5
		MOV	R0, R6
		MOV	R2, R2,LSR#16
		MOV	R10, #1
		BL	sub_FF841880

loc_FF84E588:
		ADD	R7, R7,	#4
		LDRH	R3, [R7]
		CMP	R3, #0
		BNE	loc_FF84E50C
		B	loc_FF84E5A0

loc_FF84E59C:
		ADD	R9, SP,	#0x198-0x04

loc_FF84E5A0:
		MOV	R0, SP
		MOV	R1, R9
		MOV	R2, #0x168
		BL	memcpy
		MOV	R0, #0x3A
		LDMIA	R8, {R1-R3}
		BL	sub_FF8432AC
		MOVS	R0, R0,LSL#16
		BEQ	loc_FF84E600
		LDR	R3, =0x2304
		MOV	R0, SP
		ADD	R1, SP,	#0x198+0x170
		MOV	R2, #0x164
		LDR	R4, [R3]
		BL	memcpy
		ADD	R12, SP, #0x198+0x164
		LDMIA	R12, {R1-R3}
		// MOV	R0, #0x3A
		// BL	GetInfoString
		LDR	R0, [SP, #0x20-0x04]
		BL	my_GetISOString
		MOV	R1, #0x19
		MOV	R2, R0
		MOV	R0, R4
		BL	sub_FF8417D0
		MOV	R10, #1

loc_FF84E600:
		MOV	R0, SP
		MOV	R1, R9
		MOV	R2, #0x168
		BL	memcpy
		MOV	R0, #0x3D
		LDMIA	R8, {R1-R3}
		BL	sub_FF8432AC
		MOVS	R0, R0,LSL#16
		BEQ	loc_FF84E65C
		LDR	R3, =0x2304
		MOV	R0, SP
		ADD	R1, SP,	#0x198+0x174
		MOV	R2, #0x160
		LDR	R4, [R3]
		BL	memcpy
		ADD	R12, SP, #0x198+0x164
		LDMIA	R12, {R0-R3}
		BL	GetWBString
		MOV	R1, #0x16
		MOV	R2, R0
		MOV	R0, R4
		BL	sub_FF8417D0
		MOV	R10, #1

loc_FF84E65C:
		MOV	R0, SP
		MOV	R1, R9
		MOV	R2, #0x168
		BL	memcpy
		MOV	R0, #0x4B
		LDMIA	R8, {R1-R3}
		BL	sub_FF8432AC
		MOVS	R0, R0,LSL#16
		BEQ	loc_FF84E6B8
		LDR	R3, =0x2304
		MOV	R0, SP
		ADD	R1, SP,	#0x198+0x174
		MOV	R2, #0x160
		LDR	R4, [R3]
		BL	memcpy
		ADD	R12, SP, #0x198+0x164
		LDMIA	R12, {R0-R3}
		BL	sub_FF843DC8
		MOV	R1, #0x18
		MOV	R2, R0
		MOV	R0, R4
		BL	sub_FF8417D0
		MOV	R10, #1

loc_FF84E6B8:
		MOV	R0, SP
		MOV	R1, R9
		MOV	R2, #0x168
		BL	memcpy
		MOV	R0, #0x31
		LDMIA	R8, {R1-R3}
		BL	sub_FF8432AC
		MOVS	R0, R0,LSL#16
		BEQ	loc_FF84E718
		LDR	R3, =0x2304
		MOV	R0, SP
		ADD	R1, SP,	#0x198+0x170
		MOV	R2, #0x164
		LDR	R4, [R3]
		BL	memcpy
		ADD	R12, SP, #0x198+0x164
		LDMIA	R12, {R1-R3}
		MOV	R0, #0x31
		BL	GetInfoString
		MOV	R1, #6
		MOV	R2, R0
		MOV	R0, R4
		BL	sub_FF8419F8
		MOV	R10, #1

loc_FF84E718:
		MOV	R0, SP
		MOV	R1, R9
		MOV	R2, #0x168
		BL	memcpy
		MOV	R0, #0x51
		LDMIA	R8, {R1-R3}
		BL	sub_FF8432AC
		MOVS	R0, R0,LSL#16
		BNE	loc_FF84E760
		MOV	R0, SP
		MOV	R1, R9
		MOV	R2, #0x168
		BL	memcpy
		MOV	R0, #0xD
		LDMIA	R8, {R1-R3}
		BL	sub_FF8432AC
		MOVS	R0, R0,LSL#16
		BEQ	loc_FF84E798

loc_FF84E760:
		LDR	R3, =0x2304
		MOV	R0, SP
		ADD	R1, SP,	#0x198+0x174
		MOV	R2, #0x160
		LDR	R4, [R3]
		BL	memcpy
		ADD	R12, SP, #0x198+0x164
		LDMIA	R12, {R0-R3}
		BL	sub_FF842AE0
		MOV	R1, #0x17
		MOV	R2, R0
		MOV	R0, R4
		BL	sub_FF8417D0
		MOV	R10, #1

loc_FF84E798:
		MOV	R0, SP
		MOV	R1, R9
		MOV	R2, #0x168
		BL	memcpy
		MOV	R0, #0x3C
		LDMIA	R8, {R1-R3}
		BL	sub_FF8432AC
		MOVS	R0, R0,LSL#16
		BEQ	loc_FF84E7F4

		LDR	R3, =0x2304
		MOV	R0, SP
		ADD	R1, SP,	#0x198+0x174
		MOV	R2, #0x160
		LDR	R4, [R3]
		BL	memcpy
		ADD	R12, SP, #0x198+0x164
		LDMIA	R12, {R0-R3}
		//BL	sub_FF843FE0
		BL	my_GetAEBString
		MOV	R1, #0xC
		MOV	R2, R0
		MOV	R0, R4
		//BL	sub_FF841ED0
		BL	sub_FF8417D0

		BL	my_GetCurSetString // start- add by cyrustam
		MOV	R1, #0x10
		MOV	R2, R0
		MOV	R0, R4
		BL	sub_FF8417D0 //end-add by cyrustam

		BL	my_GetColorTempString // start- add by cyrustam
		MOV	R1, #0x14
		MOV	R2, R0
		MOV	R0, R4
		BL	sub_FF8417D0 //end-add by cyrustam

		BL	my_GetDynamicString // start- add by cyrustam
		MOV	R1, #0x12
		MOV	R2, R0
		MOV	R0, R4
		BL	sub_FF8417D0 //end-add by cyrustam

		MOV	R10, #1

loc_FF84E7F4:
		CMP	R10, #0
		BEQ	loc_FF84E818
		LDR	R4, =0x2304
		LDR	R0, [R4]
		BL	sub_FF8796E0
		LDR	R0, [R4]
		BL	sub_FF878C60
		LDR	R0, [R4]
		BL	sub_FF878488

loc_FF84E818:
		ADD	SP, SP,	#0x168
		LDMFD	SP!, {R4-R10,LR}
		ADD	SP, SP,	#0x10
		MOV	PC, LR				// RET

.align 2
fin:
