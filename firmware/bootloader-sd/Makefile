OBJ = main.o spike_hw.o mmc.o ff.o

LM32_CC=lm32-elf-gcc
LM32_LD=lm32-elf-ld
LM32_OBJCOPY=lm32-elf-objcopy
LM32_OBJDUMP=lm32-elf-objdump

SREC2VRAM ?= ../../tools/srec2vram/srec2vram

VRAMFILE=image.ram
CPU_FLAGS=-mbarrel-shift-enabled -mmultiply-enabled -msign-extend-enabled \
          -mdivide-enabled

CFLAGS=-MMD -O2 -Wall -g -s -fomit-frame-pointer $(CPU_FLAGS)
LDFLAGS=-nostdlib -nodefaultlibs -Tlinker.ld 
SEGMENTS = -j .text -j .rodata -j .data -j .bss

all: $(VRAMFILE) 

crt0ram.o: crt0ram.S
	$(LM32_CC) $(CFLAGS) -c crt0ram.S

%.o: %.c
	$(LM32_CC) $(CFLAGS) -c $<

image: crt0ram.o $(OBJ)
	$(LM32_LD) $(LDFLAGS) -Map image.map -N -o image crt0ram.o $(OBJ)

image.lst: image
	$(LM32_OBJDUMP) -h -S $< > $@

image.bin: image
	$(LM32_OBJCOPY) $(SEGMENTS) -O srec image image.bin

image.srec: image image.lst
	$(LM32_OBJCOPY) $(SEGMENTS) -O srec image image.srec

$(VHDLFILE): image.srec
	$(SREC2VHDL)  --srec-forceS3 image.srec > $(VHDLFILE)

$(VRAMFILE): image.srec
	$(SREC2VRAM) 4 image.srec 0x00000000 0x4000 > $(VRAMFILE)
	ruby convert.rb

clean:
	rm -f image image.lst image.bin image.srec image.map  *.o *.d

DEPS := $(wildcard *.d)
ifneq ($(DEPS),)
include $(DEPS)
endif

