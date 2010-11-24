
bootflg2.o:     file format elf32-littlearm

Disassembly of section .text:

00000000 <Delay>:
   0:	e2500001 	subs	r0, r0, #1	; 0x1
   4:	01a0f00e 	moveq	pc, lr
   8:	e3a030ff 	mov	r3, #255	; 0xff
   c:	e1a00000 	nop			(mov r0,r0)
  10:	e1a00000 	nop			(mov r0,r0)
  14:	e1a00000 	nop			(mov r0,r0)
  18:	e1a00000 	nop			(mov r0,r0)
  1c:	e2533001 	subs	r3, r3, #1	; 0x1
  20:	1afffff9 	bne	c <Delay+0xc>
  24:	e2500001 	subs	r0, r0, #1	; 0x1
  28:	1afffff6 	bne	8 <Delay+0x8>
  2c:	e1a0f00e 	mov	pc, lr

00000030 <Copy>:
  30:	e59f0020 	ldr	r0, [pc, #32]	; 58 <.text+0x58>
  34:	e3a01601 	mov	r1, #1048576	; 0x100000
  38:	e3a02000 	mov	r2, #0	; 0x0
  3c:	e59fc018 	ldr	ip, [pc, #24]	; 5c <.text+0x5c>
  40:	e4903004 	ldr	r3, [r0], #4
  44:	e4813004 	str	r3, [r1], #4
  48:	e2822001 	add	r2, r2, #1	; 0x1
  4c:	e152000c 	cmp	r2, ip
  50:	dafffffa 	ble	40 <Copy+0x10>
  54:	e1a0f00e 	mov	pc, lr
  58:	ffff25f4
  5c:	000032ff

00000060 <Zero>:
  60:	e3a03000 	mov	r3, #0	; 0x0
  64:	e1530001 	cmp	r3, r1
  68:	a1a0f00e 	movge	pc, lr
  6c:	e1a02003 	mov	r2, r3
  70:	e7802103 	str	r2, [r0, r3, lsl #2]
  74:	e2833001 	add	r3, r3, #1	; 0x1
  78:	e1530001 	cmp	r3, r1
  7c:	bafffffb 	blt	70 <Zero+0x10>
  80:	e1a0f00e 	mov	pc, lr

00000084 <main>:
  84:	e1a0c00d 	mov	ip, sp
  88:	e92dd8f0 	stmdb	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
  8c:	e24cb004 	sub	fp, ip, #4	; 0x4
  90:	e24ddc01 	sub	sp, sp, #256	; 0x100
  94:	e3a02044 	mov	r2, #68	; 0x44
  98:	e59f3230 	ldr	r3, [pc, #560]	; 2d0 <.text+0x2d0>
  9c:	e48320a0 	str	r2, [r3], #160
  a0:	e5832000 	str	r2, [r3]
  a4:	e3a00902 	mov	r0, #32768	; 0x8000
  a8:	ebfffffe 	bl	0 <Delay>
  ac:	e59f1220 	ldr	r1, [pc, #544]	; 2d4 <.text+0x2d4>
  b0:	e59f0220 	ldr	r0, [pc, #544]	; 2d8 <.text+0x2d8>
  b4:	e3a0733e 	mov	r7, #-134217728	; 0xf8000000
  b8:	e3a06000 	mov	r6, #0	; 0x0
  bc:	e5912000 	ldr	r2, [r1]
  c0:	e59f3214 	ldr	r3, [pc, #532]	; 2dc <.text+0x2dc>
  c4:	e1520003 	cmp	r2, r3
  c8:	1a000002 	bne	d8 <main+0x54>
  cc:	e5913004 	ldr	r3, [r1, #4]
  d0:	e3530035 	cmp	r3, #53	; 0x35
  d4:	0a000000 	beq	dc <main+0x58>
  d8:	e3a06001 	mov	r6, #1	; 0x1
  dc:	e5902000 	ldr	r2, [r0]
  e0:	e59f31f8 	ldr	r3, [pc, #504]	; 2e0 <.text+0x2e0>
  e4:	e1520003 	cmp	r2, r3
  e8:	1a000003 	bne	fc <main+0x78>
  ec:	e5902004 	ldr	r2, [r0, #4]
  f0:	e283300e 	add	r3, r3, #14	; 0xe
  f4:	e1520003 	cmp	r2, r3
  f8:	0a000000 	beq	100 <main+0x7c>
  fc:	e3a06001 	mov	r6, #1	; 0x1
 100:	e5973000 	ldr	r3, [r7]
 104:	e3530000 	cmp	r3, #0	; 0x0
 108:	1a000002 	bne	118 <main+0x94>
 10c:	e5973008 	ldr	r3, [r7, #8]
 110:	e3730001 	cmn	r3, #1	; 0x1
 114:	0a000000 	beq	11c <main+0x98>
 118:	e3a06001 	mov	r6, #1	; 0x1
 11c:	e5973004 	ldr	r3, [r7, #4]
 120:	e2833001 	add	r3, r3, #1	; 0x1
 124:	e3530001 	cmp	r3, #1	; 0x1
 128:	83a06001 	movhi	r6, #1	; 0x1
 12c:	e3560000 	cmp	r6, #0	; 0x0
 130:	1a000030 	bne	1f8 <main+0x174>
 134:	e59f4194 	ldr	r4, [pc, #404]	; 2d0 <.text+0x2d0>
 138:	e3a03046 	mov	r3, #70	; 0x46
 13c:	e5843000 	str	r3, [r4]
 140:	e3a00902 	mov	r0, #32768	; 0x8000
 144:	ebfffffe 	bl	0 <Delay>
 148:	e3a03044 	mov	r3, #68	; 0x44
 14c:	e5843000 	str	r3, [r4]
 150:	e3a00902 	mov	r0, #32768	; 0x8000
 154:	ebfffffe 	bl	0 <Delay>
 158:	ebfffffe 	bl	30 <Copy>
 15c:	e24b509c 	sub	r5, fp, #156	; 0x9c
 160:	e1a00005 	mov	r0, r5
 164:	e3a01020 	mov	r1, #32	; 0x20
 168:	ebfffffe 	bl	60 <Zero>
 16c:	e24b4f47 	sub	r4, fp, #284	; 0x11c
 170:	e1a00004 	mov	r0, r4
 174:	e3a01020 	mov	r1, #32	; 0x20
 178:	ebfffffe 	bl	60 <Zero>
 17c:	e1a00005 	mov	r0, r5
 180:	e1a01004 	mov	r1, r4
 184:	e59f3158 	ldr	r3, [pc, #344]	; 2e4 <.text+0x2e4>
 188:	e1a0e00f 	mov	lr, pc
 18c:	e1a0f003 	mov	pc, r3
 190:	e5973000 	ldr	r3, [r7]
 194:	e3530000 	cmp	r3, #0	; 0x0
 198:	1a000002 	bne	1a8 <main+0x124>
 19c:	e5973008 	ldr	r3, [r7, #8]
 1a0:	e3730001 	cmn	r3, #1	; 0x1
 1a4:	0a000000 	beq	1ac <main+0x128>
 1a8:	e3a06001 	mov	r6, #1	; 0x1
 1ac:	e5973004 	ldr	r3, [r7, #4]
 1b0:	e2833001 	add	r3, r3, #1	; 0x1
 1b4:	e3530001 	cmp	r3, #1	; 0x1
 1b8:	83a06001 	movhi	r6, #1	; 0x1
 1bc:	e5972000 	ldr	r2, [r7]
 1c0:	e51b309c 	ldr	r3, [fp, #-156]
 1c4:	e1530002 	cmp	r3, r2
 1c8:	1a000007 	bne	1ec <main+0x168>
 1cc:	e5972004 	ldr	r2, [r7, #4]
 1d0:	e51b3098 	ldr	r3, [fp, #-152]
 1d4:	e1530002 	cmp	r3, r2
 1d8:	1a000003 	bne	1ec <main+0x168>
 1dc:	e5972008 	ldr	r2, [r7, #8]
 1e0:	e51b3094 	ldr	r3, [fp, #-148]
 1e4:	e1530002 	cmp	r3, r2
 1e8:	0a000000 	beq	1f0 <main+0x16c>
 1ec:	e3a06001 	mov	r6, #1	; 0x1
 1f0:	e3560000 	cmp	r6, #0	; 0x0
 1f4:	0a000003 	beq	208 <main+0x184>
 1f8:	e3a02046 	mov	r2, #70	; 0x46
 1fc:	e59f30e4 	ldr	r3, [pc, #228]	; 2e8 <.text+0x2e8>
 200:	e5832000 	str	r2, [r3]
 204:	ea000030 	b	2cc <main+0x248>
 208:	e59f40c0 	ldr	r4, [pc, #192]	; 2d0 <.text+0x2d0>
 20c:	e3a05046 	mov	r5, #70	; 0x46
 210:	e5845000 	str	r5, [r4]
 214:	e3a00902 	mov	r0, #32768	; 0x8000
 218:	ebfffffe 	bl	0 <Delay>
 21c:	e3a03044 	mov	r3, #68	; 0x44
 220:	e5843000 	str	r3, [r4]
 224:	e3a00902 	mov	r0, #32768	; 0x8000
 228:	ebfffffe 	bl	0 <Delay>
 22c:	e5845000 	str	r5, [r4]
 230:	e59f30b0 	ldr	r3, [pc, #176]	; 2e8 <.text+0x2e8>
 234:	e5835000 	str	r5, [r3]
 238:	e51b3098 	ldr	r3, [fp, #-152]
 23c:	e3530000 	cmp	r3, #0	; 0x0
 240:	02433001 	subeq	r3, r3, #1	; 0x1
 244:	050b3098 	streq	r3, [fp, #-152]
 248:	13a03000 	movne	r3, #0	; 0x0
 24c:	150b3098 	strne	r3, [fp, #-152]
 250:	e24b009c 	sub	r0, fp, #156	; 0x9c
 254:	e24b1f47 	sub	r1, fp, #284	; 0x11c
 258:	e59f308c 	ldr	r3, [pc, #140]	; 2ec <.text+0x2ec>
 25c:	e1a0e00f 	mov	lr, pc
 260:	e1a0f003 	mov	pc, r3
 264:	e3a00902 	mov	r0, #32768	; 0x8000
 268:	ebfffffe 	bl	0 <Delay>
 26c:	e59f405c 	ldr	r4, [pc, #92]	; 2d0 <.text+0x2d0>
 270:	e3a02044 	mov	r2, #68	; 0x44
 274:	e5842000 	str	r2, [r4]
 278:	e59f3068 	ldr	r3, [pc, #104]	; 2e8 <.text+0x2e8>
 27c:	e5832000 	str	r2, [r3]
 280:	e3a00902 	mov	r0, #32768	; 0x8000
 284:	ebfffffe 	bl	0 <Delay>
 288:	e5972000 	ldr	r2, [r7]
 28c:	e51b309c 	ldr	r3, [fp, #-156]
 290:	e1530002 	cmp	r3, r2
 294:	1a000009 	bne	2c0 <main+0x23c>
 298:	e5972004 	ldr	r2, [r7, #4]
 29c:	e51b3098 	ldr	r3, [fp, #-152]
 2a0:	e1530002 	cmp	r3, r2
 2a4:	1a000005 	bne	2c0 <main+0x23c>
 2a8:	e5972008 	ldr	r2, [r7, #8]
 2ac:	e51b3094 	ldr	r3, [fp, #-148]
 2b0:	e1530002 	cmp	r3, r2
 2b4:	03a03046 	moveq	r3, #70	; 0x46
 2b8:	05843000 	streq	r3, [r4]
 2bc:	0a000002 	beq	2cc <main+0x248>
 2c0:	e3a02046 	mov	r2, #70	; 0x46
 2c4:	e59f301c 	ldr	r3, [pc, #28]	; 2e8 <.text+0x2e8>
 2c8:	e5832000 	str	r2, [r3]
 2cc:	eafffffe 	b	2cc <main+0x248>
 2d0:	c0220000
 2d4:	ff81044c
 2d8:	ff9822ec
 2dc:	2f322e31
 2e0:	006e0061
 2e4:	001059b0
 2e8:	c02200a0
 2ec:	0010586c
