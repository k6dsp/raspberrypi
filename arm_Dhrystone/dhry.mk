# This makefile is intended for use with GNU make

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


CFLAGS = -W -g -O3 -Otime --no_inline --no_multifile -DMSC_CLOCK


dhry.axf: dhry_1.o dhry_2.o
	$(LD) dhry_1.o dhry_2.o --info totals -o dhry.axf

dhry_1.o: dhry_1.c dhry.h
	$(CC) -c $(CFLAGS) dhry_1.c -o dhry_1.o

dhry_2.o: dhry_2.c dhry.h
	$(CC) -c $(CFLAGS) dhry_2.c -o dhry_2.o

