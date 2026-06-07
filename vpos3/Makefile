.EXPORT_ALL_VARIABLES:

TOPDIR	:= $(shell if [ "$$PWD" != "" ]; then echo $$PWD; else pwd; fi)

DIRS = kernel hal/cpu hal/io shell objs
CROSSTOOL = arm-linux-gnueabi-
CC = $(CROSSTOOL)gcc
OC = $(CROSSTOOL)objcopy
LD = $(CROSSTOOL)ld

INCLUDE = -I. -I$(TOPDIR)/include -I$(TOPDIR)/hal/include -I$(TOPDIR)/fs/include -I/home/$(USER)/gcc-linaro-6.3.1_arm-linux-gnueabi/lib/gcc/arm-linux-gnueabi/6.3.1/include

CFLAGS  = -g -O0 -Wall -Wstrict-prototypes -fPIC -msoft-float -nostdinc -nostartfiles -nostdlib -march=armv7-a -marm -fno-builtin $(INCLUDE)

OCFLAGS = -O binary -R .note -R .comment -R .stab -R .stabstr -S 

all:
	for i in $(DIRS) ; do make -C $$i || exit $? ; done
	dd if=images/vpos_kernel_binary of=images/vpos.bin bs=1k conv=sync

	date

imgmerge:
	$(OC) $(OCFLAGS) ./images/vpos_kernel-elf32 ./images/vpos_kernel_binary
	dd if=images/vpos_kernel_binary of=images/vpos.bin bs=1k conv=sync
	dd if=utils4/alpha of=images/vpos.bin bs=1k seek=3040
	

clean:
	for i in $(DIRS) ; do make -C $$i clean; done
	rm -f ./images/vpos_bootloader_binary;rm -f ./images/vpos_bootloader-elf32;rm -f ./images/vpos_kernel_binary;rm -f ./images/vpos_kernel-elf32;rm -f ./images/vpos.bin
	
dep:
	for i in $(DIRS) ; do make -C $$i dep; done
