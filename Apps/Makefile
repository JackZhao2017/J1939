# CROSS_COMPILE=arm-linux-gnueabihf-

OBJDIR = .
SRCDIR = .
VERSION = 0.0.3
AS		= $(CROSS_COMPILE)as
LD		= $(CROSS_COMPILE)ld
CC		= $(CROSS_COMPILE)gcc
CPLUS	= $(CROSS_COMPILE)g++
CPP		= $(CC) -E
AR		= $(CROSS_COMPILE)ar
NM		= $(CROSS_COMPILE)nm
STRIP	= $(CROSS_COMPILE)strip
OBJCOPY	= $(CROSS_COMPILE)objcopy
OBJDUMP	= $(CROSS_COMPILE)objdump
RM		= rm -f
MAKEDIR	= mkdir -p




JzApp.out:JzAppMain.c src/*.c
	${CC}   -I./inc/  $^ -DLINUX -o $@ -lpthread

clean:
	rm *.out