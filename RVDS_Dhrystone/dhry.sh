armcc -c -W -g -O3 -Otime --no_inline --no_multifile -DMSC_CLOCK dhry_1.c dhry_2.c
armlink dhry_1.o dhry_2.o -o dhry.axf --info totals
