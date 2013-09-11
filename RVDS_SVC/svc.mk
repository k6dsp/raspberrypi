# This makefile is intended for use with GNU make

TARGET=svc.axf

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


CFLAGS = -g -O1


svc.axf: ahandle.o main.o chandle.o
	$(LD) ahandle.o main.o chandle.o -o svc.axf

ahandle.o: ahandle.s
	$(AS) -g ahandle.s -o ahandle.o

main.o: main.c
	$(CC) -c $(CFLAGS) main.c -o main.o

chandle.o: chandle.c
	$(CC) -c $(CFLAGS) chandle.c -o chandle.o

