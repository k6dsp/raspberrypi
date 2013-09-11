# This makefile is intended for use with GNU make

TARGET=mmugen.axf

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


CFLAGS = -g -O0

mmugen.axf: main.o level1.o level2.o checking.o missing_region.o
	$(LD) main.o level1.o level2.o checking.o missing_region.o --info totals -o mmugen.axf

main.o: main.c
	$(CC) -c $(CFLAGS) main.c -o main.o

level1.o: level1.c
	$(CC) -c $(CFLAGS) level1.c -o level1.o

level2.o: level2.c
	$(CC) -c $(CFLAGS) level2.c -o level2.o

checking.o: checking.c
	$(CC) -c $(CFLAGS) checking.c -o checking.o

missing_region.o: missing_region.c
	$(CC) -c $(CFLAGS) missing_region.c -o missing_region.o

