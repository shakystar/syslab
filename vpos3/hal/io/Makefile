OBJS = serial.o timer.o

SRCS = serial.c timer.c

all : $(OBJS)
	mv *.o ../../objs
clean:
	rm -f *.o
	
dep:
	gccmakedep $(INCLUDE) $(LIB_SRCS)
