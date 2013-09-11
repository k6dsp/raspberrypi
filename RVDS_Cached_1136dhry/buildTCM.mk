# This makefile is intended for use with GNU make.

TARGET=1136dhry.axf

CC=armcc
AS=armasm
LD=armlink
AR=armar
FE=fromelf

# Select build rules based on Windows or Unix
ifdef WINDIR
DONE=@if exist $(1) echo Build completed.
RM=if exist $(1) del /q $(1)
SHELL=$(WINDIR)\system32\cmd.exe
else
ifdef windir
DONE=@if exist $(1) echo Build completed.
RM=if exist $(1) del /q $(1)
SHELL=$(windir)\system32\cmd.exe
else
DONE=@if [ -f $(1) ]; then echo Build completed.; fi
RM=rm -f $(1)
endif
endif

all: $(TARGET)
	 $(call DONE,$(TARGET))

rebuild: clean all

clean:
	  $(call RM,*.o)
	  $(call RM,$(TARGET))


CORE=6
DEBUG=-g
OPT2=-O3 -W --no_inline -Otime
OPT1=-O1

CPD=-DMSC_CLOCK
APD=--predefine "TCM SETL {TRUE}"

1136dhry.axf : dhry_1.o dhry_2.o retarget.o Init.o InitCache.o scatter.scat
	$(LD) --info totals --info unused --scatter scatter.scat --entry Start \
	-o 1136dhry.axf dhry_1.o dhry_2.o retarget.o Init.o InitCache.o
	
dhry_1.o : dhry_1.c
	$(CC) -c $(DEBUG) $(OPT2) --cpu $(CORE) $(CPD) dhry_1.c
	
dhry_2.o : dhry_2.c
	$(CC) -c $(DEBUG) $(OPT2) --cpu $(CORE) $(CPD) dhry_2.c
	
retarget.o : retarget.c
	$(CC) -c $(DEBUG) $(OPT1) --cpu $(CORE) $(CPD) retarget.c
	
Init.o :  Init.s
	$(AS) $(DEBUG) --cpu $(CORE) $(APD) Init.s

InitCache.o :  InitCache.s
	$(AS) $(DEBUG) --cpu $(CORE) InitCache.s
