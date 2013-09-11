armasm -g --cpu arm926ej-s --predefine "ROM_RAM_REMAP SETL {TRUE}" --predefine "LOCATIONS_IN_CODE SETL {TRUE}" ../source/init.s
armasm -g --cpu arm926ej-s ../source/vectors.s
armasm -g --cpu arm926ej-s --predefine "LOCATIONS_IN_CODE SETL {TRUE}" ../source/embed_uish.s
armasm -g --cpu arm926ej-s --predefine "LOCATIONS_IN_CODE SETL {TRUE}" ../source/926ej-s/inittcm.s
armasm -g --cpu arm926ej-s --predefine "LOCATIONS_IN_CODE SETL {TRUE}" ../source/926ej-s/initmmu.s
armasm -g --cpu arm926ej-s ../source/926ej-s/initcache.s
armcc -c -g --cpu arm926ej-s -O2 -Otime -W --no_inline -DMSC_CLOCK -D__CLK_TCK=100 ../dhry/dhry_1.c ../dhry/dhry_2.c -I../include
armcc -c -g --cpu arm926ej-s -O2 -DSTANDALONE -DLIGHT_LED ../source/retarget.c
armcc -c -g --cpu arm926ej-s -O2 ../source/clock_irq.c -I../include
armcc -c -g --cpu arm926ej-s -O2 ../source/serial.c -I../include
armcc -c -g --cpu arm926ej-s -O2 -DUSE_SERIAL_PORT ../source/submain.c
armlink --scatter ../scatter/embedded_scat.scat init.o inittcm.o initmmu.o initcache.o vectors.o embed_uish.o dhry_1.o dhry_2.o retarget.o clock_irq.o serial.o submain.o -o dhry.axf --entry Reset_Handler --keep initmmu.o --keep inittcm.o --keep initcache.o --info totals --info unused

