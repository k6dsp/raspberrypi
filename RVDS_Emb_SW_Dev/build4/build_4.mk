# This makefile is intended for use with GNU make

TARGET=dhry.axf

CC=armcc
AS=armasm
LD=armlink
AR=armar

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
	  $(call RM,*.axf)
	  $(call RM,$(TARGET))


CFLAGS = -c -g --cpu arm926ej-s -O2 -Otime -W --no_inline -DMSC_CLOCK -D__CLK_TCK=100 -DSTANDALONE -DLIGHT_LED -I../include
LFLAGS = --info totals --info unused --scatter ../scatter/embedded_scat.scat --entry Reset_Handler --keep initmmu.o --keep inittcm.o --keep initcache.o
AFLAGS = -g --cpu arm926ej-s --predefine "TCM SETL {TRUE}" --predefine "ROM_RAM_REMAP SETL {TRUE}" --predefine "LOCATIONS_IN_CODE SETL {TRUE}"

dhry.axf: dhry_1.o dhry_2.o retarget.o clock_irq.o serial.o embed_uish.o init.o vectors.o submain.o inittcm.o initmmu.o initcache.o ../scatter/embedded_scat.scat
	$(LD) $(LFLAGS) dhry_1.o dhry_2.o retarget.o clock_irq.o serial.o embed_uish.o init.o vectors.o submain.o inittcm.o initmmu.o initcache.o -o dhry.axf

dhry_1.o: ../dhry/dhry_1.c
	$(CC) $(CFLAGS) ../dhry/dhry_1.c -o dhry_1.o

dhry_2.o: ../dhry/dhry_2.c
	$(CC) $(CFLAGS) ../dhry/dhry_2.c -o dhry_2.o

retarget.o: ../source/retarget.c
	$(CC) $(CFLAGS) ../source/retarget.c -o retarget.o

clock_irq.o: ../source/clock_irq.c
	$(CC) $(CFLAGS) ../source/clock_irq.c -o clock_irq.o

serial.o: ../source/serial.c
	$(CC) $(CFLAGS) ../source/serial.c -o serial.o

embed_uish.o: ../source/embed_uish.s
	$(AS) $(AFLAGS) ../source/embed_uish.s

init.o: ../source/init.s
	$(AS) $(AFLAGS) ../source/init.s

vectors.o: ../source/vectors.s
	$(AS) $(AFLAGS) ../source/vectors.s

submain.o: ../source/submain.c
	$(CC) $(CFLAGS) -DUSE_SERIAL_PORT ../source/submain.c -o submain.o

inittcm.o: ../source/926ej-s/inittcm.s
	$(AS) $(AFLAGS) ../source/926ej-s/inittcm.s

initmmu.o: ../source/926ej-s/initmmu.s
	$(AS) $(AFLAGS) ../source/926ej-s/initmmu.s

initcache.o: ../source/926ej-s/initcache.s
	$(AS) $(AFLAGS) ../source/926ej-s/initcache.s
