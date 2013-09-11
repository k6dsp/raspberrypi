armcc -c -g -O2 -Otime --cpu arm926ej-s -W --no_inline -DMSC_CLOCK -D__CLK_TCK=1 ../dhry/dhry_1.c ../dhry/dhry_2.c -I../include
armcc -c -g -O2 --cpu arm926ej-s -DUSE_TARGET_CLOCK -DUSE_SERIAL_PORT -DLIGHT_LED ../source/retarget.c
armcc -c -g -O2 --cpu arm926ej-s ../source/clock.c -I../include
armcc -c -g -O2 --cpu arm926ej-s ../source/serial.c -I../include
armlink dhry_1.o dhry_2.o retarget.o clock.o serial.o -o dhry.axf --info totals --info unused

