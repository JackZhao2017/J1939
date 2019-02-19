# CROSS_COMPILE=arm-linux-gnueabihf-

OBJDIR = .
SRCDIR = .

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

MODULE_NAME="adas"

MAJOR_N  = 0
MINOR_N  = 1
REVISION_N = 2
SVN =0
TIME =$(shell date +"%Y-%m-%d %H:%M:%S")
COMPILETIME ="$(TIME)"

VERSION_FLAG :=-DMODULE=\"$(MODULE_NAME)\" -DMAJOR=$(MAJOR_N) -DMINOR=$(MINOR_N) -DREVISION=$(REVISION_N) -DCOMPILETIME=\"$(COMPILETIME)\" 
# VERSION_FLAG +=-DCOMPILETIME =\"$(COMPILETIME)\" 

JzApp.out:JzAppMain.c src/*.c
	${CC}   -I./inc/ -I../ $^ -D LINUX ${VERSION_FLAG}  -o $@ -lpthread

clean:
	rm *.out