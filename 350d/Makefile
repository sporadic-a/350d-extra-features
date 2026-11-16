name=AUTOEXEC

# Memory reserved for the add-on, in bytes; a multiple of 4. The add-on
# takes the top ADDON_SIZE bytes of Canon's main heap: it runs at
# ADDON_BASE, directly below 0x800000 where the camera loads
# AUTOEXEC.BIN, and entry.S ends the heap at ADDON_BASE so the firmware
# never hands that memory out. This one value sets the link address, the
# amount COPY() in init.c copies, the heap's end and the size check below.
#
# The add-on needs a fixed address rather than a block allocated from the
# heap, for two reasons. It runs before the heap exists: entry.S replaces
# Canon's startup code and is what creates the heap. And it only works at
# the address it was linked for. The hand-written assembly in entry.S and
# gui.S gives absolute addresses of add-on code to the CPU and to Canon's
# firmware, such as LDR PC, =my_romStart and LDR R0, =my_IDLEHandler.
ADDON_SIZE ?= 32000
ADDON_BASE := $(shell printf '0x%X' $$((0x800000 - $(ADDON_SIZE))))
CPPFLAGS = -DADDON_SIZE=$(ADDON_SIZE) -DADDON_BASE=$(ADDON_BASE)

# Pass CROSS=arm-elf- to build with the forum releases' original
# toolchain (GCC 3.4.6).
CROSS ?= arm-none-eabi-

# -std=gnu89 keeps calls to undeclared Canon firmware routines legal (GCC
# 14 and later reject them otherwise). Their real argument types aren't
# known, so headers.h doesn't give them prototypes.
CFLAGS=-nostdlib -march=armv5te -fno-builtin -std=gnu89
CC=$(CROSS)gcc
AS=$(CROSS)as
OBJCOPY=$(CROSS)objcopy

# Delete AUTOEXEC.BIN if the size check below fails, so an oversized
# binary is never left around to be copied to a card.
.DELETE_ON_ERROR:

all: $(name).BIN

# The camera crashes (Err 99) on a binary larger than ADDON_SIZE, because
# COPY() in init.c moves only that much to the run address.
$(name).BIN: $(name).arm.elf
	$(OBJCOPY) -O binary $(name).arm.elf $(name).BIN
	test $$(wc -c < $@) -le $(ADDON_SIZE)
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
	$(CC) $(CFLAGS) -Wl,--defsym=ADDON_BASE=$(ADDON_BASE) \
		-Wl,-T,link.script -o$@ $(filter %.o,$^)

# Rebuild every object when ADDON_SIZE changes, including when it's only
# overridden on the command line. A binary that mixed objects built for
# different sizes would copy the add-on to one address and end the heap at
# another.
addon_size.stamp: FORCE
	@echo $(ADDON_SIZE) | cmp -s - $@ || echo $(ADDON_SIZE) > $@
FORCE:
entry.o entry_subs.o gui.o init.o main.o: addon_size.stamp

# Check that the link address, COPY()'s destination and the heap's end
# all agree on ADDON_BASE.
.PHONY: check
check: $(name).arm.elf
	CROSS=$(CROSS) ./check_layout.sh $< $(ADDON_BASE)

entry.o:entry.S

gui.o:gui.S

main.o:main.c

init.o:init.c

entry_subs.o: entry_subs.S


clean:
	rm *.o; rm $(name).arm.elf
