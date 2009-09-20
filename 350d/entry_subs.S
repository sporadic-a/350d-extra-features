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

// procs

#include  "funclist.txt"

.align 2
fin:
