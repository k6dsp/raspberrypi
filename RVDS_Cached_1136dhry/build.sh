armcc -c --debug -O3 -W --no_inline -Otime --no_multifile --cpu 6 -DMSC_CLOCK dhry_1.c dhry_2.c
armcc -c --debug -O1 --cpu 6 retarget.c
armasm --debug --keep --cpu 6 Init.s
armasm --debug --keep --cpu 6 InitCache.s
armlink --info totals --info unused --scatter scatter.scat --entry Start -o 1136dhry.axf dhry_1.o dhry_2.o retarget.o Init.o InitCache.o
