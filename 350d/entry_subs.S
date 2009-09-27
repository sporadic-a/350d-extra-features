	.text
	.org 0

#define DEF(n,a) \
	.globl n; n = a

#define NSTUB(name, addr)\
	.globl name ;\
	.weak name ;\
	name: ;\
		ldr  pc, = addr

#define NHSTUB(name, addr)\
	.globl name ;\
	name: ;\
		ldr  pc, = addr

#define STUB(addr)\
	.globl sub_ ## addr ; sub_ ## addr = 0x ## addr

// vars

DEF(aUsrrootMmuConf, 0xFF810A70)

DEF(aStartup, 0xFF81112C)

DEF(aUsbRootDeviceH, 0xFF811134)
DEF(aU2vbus, 0xFF81114C)
DEF(aUsbc20VbusSupp, 0xFF811154)
DEF(aDeviceSpeed, 0xFF811168)

DEF(aGuiInIdlehandl, 0xFF834918)
DEF(aGuiIdlehandler, 0xFF834930)
DEF(aGuiIdlehandl_0, 0xFF834958)
DEF(aGuiRequestMenu, 0xFF834980)
DEF(aGuiRequestInfo, 0xFF834998)
DEF(aGuiRequestPlay, 0xFF8349B0)
DEF(aGuiStartIdleMo, 0xFF8349C8)
DEF(aGuiStartMenuMo, 0xFF8349E0)
DEF(aGuiStartInfoMo, 0xFF8349F8)
DEF(aGuiStartPlayMo, 0xFF834A10)
DEF(aGuiStartQrMode, 0xFF834A28)
DEF(aGuiStartNocfWa, 0xFF834A3C)
DEF(aGuiStartFolder, 0xFF834A5C)
DEF(aGuiStartRtcset, 0xFF834A80)
DEF(aGuiStartMenuIs, 0xFF834A98)
DEF(aGuiStartMenuQu, 0xFF834AB4)
DEF(aGuiStartMenuGe, 0xFF834AD4)

DEF(aGuiswsem, 0xFF835218)
DEF(aForrec, 0xFF835224)
DEF(aForeground, 0xFF83522C)
DEF(aHcanongothic, 0xFF835238)

DEF(aInfocreative, 0xFF84E2A4)
DEF(a__GuiInfoappIn, 0xFF84E2B8)
DEF(a0_8, 0xFF84E2B4)

DEF(aH, 0xFF8453BC)

// procs

#include  "funclist.txt"

.align 2
fin:
