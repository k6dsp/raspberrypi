armcc -c -g -O2 -Otime --cpu arm926ej-s -W --no_inline -DMSC_CLOCK ../dhry/dhry_1.c ../dhry/dhry_2.c -I../include
armlink dhry_1.o dhry_2.o -o dhry.axf --info totals --info unused

