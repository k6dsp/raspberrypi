# This makefile is intended for use with GNU make.

TARGET=dhry.axf

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


CFLAGS = -c -g -O2 -Otime --cpu arm926ej-s -W --no_inline -DMSC_CLOCK -D__CLK_TCK=1 -DSTANDALONE -DLIGHT_LED -I../include
LFLAGS = --info totals --info unused --scatter ../scatter/simple_scat.scat
AFLAGS = -g --cpu arm926ej-s

dhry.axf: dhry_1.o dhry_2.o retarget.o clock.o serial.o uish.o ../scatter/simple_scat.scat
	$(LD) $(LFLAGS) dhry_1.o dhry_2.o retarget.o clock.o serial.o uish.o -o dhry.axf

dhry_1.o: ../dhry/dhry_1.c
	$(CC) $(CFLAGS) ../dhry/dhry_1.c -o dhry_1.o

dhry_2.o: ../dhry/dhry_2.c
	$(CC) $(CFLAGS) ../dhry/dhry_2.c -o dhry_2.o

retarget.o: ../source/retarget.c
	$(CC) $(CFLAGS) ../source/retarget.c -o retarget.o

clock.o: ../source/clock.c
	$(CC) $(CFLAGS) ../source/clock.c -o clock.o

serial.o: ../source/serial.c
	$(CC) $(CFLAGS) ../source/serial.c -o serial.o

uish.o: ../source/uish.s
	$(AS) $(AFLAGS) ../source/uish.s

