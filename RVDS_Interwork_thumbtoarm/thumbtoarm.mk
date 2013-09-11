# This makefile is intended for use with GNU make

TARGET=thumbtoarm.axf

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


CFLAGS = -g -O1 --apcs /interwork


thumbtoarm.axf: thumbmain.o armsub.o
	$(LD) thumbmain.o armsub.o -o thumbtoarm.axf --info veneers

thumbmain.o: thumbmain.c
	$(CC) -c --thumb $(CFLAGS) thumbmain.c -o thumbmain.o

armsub.o: armsub.c
	$(CC) -c $(CFLAGS) armsub.c -o armsub.o

