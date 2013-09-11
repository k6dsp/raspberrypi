# This makefile is intended for use with GNU make

TARGET=armtothumb.axf

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


armtothumb.axf: armmain.o thumbsub.o
	$(LD) armmain.o thumbsub.o -o armtothumb.axf --info veneers

armmain.o: armmain.c
	$(CC) -c $(CFLAGS) armmain.c -o armmain.o

thumbsub.o: thumbsub.c
	$(CC) -c --thumb $(CFLAGS) thumbsub.c -o thumbsub.o


