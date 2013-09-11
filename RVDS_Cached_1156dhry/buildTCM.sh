armcc -c -g -O3 -W --no_inline -Otime --no_multifile --cpu ARM1156T2-S -DMSC_CLOCK dhry_1.c dhry_2.c
armcc -c -g -O1 --cpu ARM1156T2-S retarget.c
armasm -g --cpu ARM1156T2-S --predefine "TCM SETL {TRUE}" InitTCM.s
armasm -g --cpu ARM1156T2-S InitMPU.s
armasm -g --cpu ARM1156T2-S InitCache.s
armlink --info totals --info unused --scatter scatter.scat --entry Start -o 1156dhry.axf dhry_1.o dhry_2.o retarget.o InitTCM.o InitMPU.o InitCache.o
