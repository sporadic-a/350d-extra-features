name=AUTOEXEC
entryadr=0x7F0000

# Pass CROSS=arm-elf- to build with the forum releases' original
# toolchain (GCC 3.4.6).
CROSS ?= arm-none-eabi-

# -std=gnu89 keeps calls to undeclared Canon firmware routines legal (GCC
# 14 and later reject them otherwise). Their real argument types aren't
# known, so headers.h doesn't give them prototypes.
CFLAGS=-nostdlib -march=armv5te -fno-builtin -std=gnu89
LDFLAGS=-Wl,-Ttext,$(entryadr)
CC=$(CROSS)gcc
AS=$(CROSS)as
OBJCOPY=$(CROSS)objcopy

# Delete AUTOEXEC.BIN if the size check below fails, so an oversized
# binary is never left around to be copied to a card.
.DELETE_ON_ERROR:

all: $(name).BIN

# The camera crashes (Err 99) on a binary larger than the 8000 words
# (32,000 bytes) that COPY in init.c moves to the run address. Raise both
# limits together.
$(name).BIN: $(name).arm.elf
	$(OBJCOPY) -O binary $(name).arm.elf $(name).BIN
	test $$(wc -c < $@) -le 32000
#	cp $(name).BIN I:\\$(name).BIN

# Copy the card binary into a new releases/ folder named by date and
# commit, with -dirty added when there are uncommitted changes. mkdir fails
# if the folder already exists, so an earlier release is never overwritten.
reldir = ../releases/$(shell date +%Y-%m-%d)-$(shell git describe --always --dirty)

.PHONY: release
release: $(name).BIN
	mkdir $(reldir)
	cp $(name).BIN $(reldir)/

# link.script is a prerequisite so that editing it relinks, but it reaches
# ld only through -T: current ld rejects a script given twice.
$(name).arm.elf:entry.o entry_subs.o gui.o init.o main.o link.script
	$(CC) $(CFLAGS) -Wl,-T,link.script -o$@ $(filter %.o,$^)

entry.o:entry.S

gui.o:gui.S

main.o:main.c

init.o:init.c

entry_subs.o: entry_subs.S


clean:
	rm *.o; rm $(name).arm.elf
