OBJS = kernel_start.o machine_init.o exception_handler.o HAL_arch_startup.o hal_swi_handler.o led.o serial.o timer.o irq.o vpsh.o

IMAGE_TARGET = vpos_kernel_binary
PRE_TARGET = vpos_kernel-elf32

all : $(PRE_TARGET)
	$(OC) $(OCFLAGS) $(PRE_TARGET) $(IMAGE_TARGET)
	mv vpos_kernel_binary vpos_kernel-elf32 ../images

$(PRE_TARGET) : 
	$(LD) -v -T../hal/cpu/vpos_kernel-ld-script -Bstatic *.o -o $@ -L /home/$(USER)/gcc-linaro-6.3.1_arm-linux-gnueabi/lib/gcc/arm-linux-gnueabi/6.3.1 -L /home/$(USER)/gcc-linaro-6.3.1_arm-linux-gnueabi/arm-linux-gnueabi/lib -lgcc -lc
#	$(LD) -r -Bstatic *.o -o libvpos.a -L/usr/local/arm_huins/lib/gcc-lib/arm-linux/3.2 -lgcc -lc
	
clean:
	rm -f $(OBJS)
	rm -f $(PRE_TARGET)
	rm -f $(IMAGE_TARGET)
	
dep:
	gccmakedep $(INCLUDE) $(LIB_SRCS)
