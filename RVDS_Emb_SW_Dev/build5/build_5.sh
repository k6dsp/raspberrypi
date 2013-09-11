armasm -g --cpu arm926ej-s --predefine "ROM_RAM_REMAP SETL {TRUE}" --predefine "USE_SCATTER_SYMS SETL {TRUE}" ../source/init.s
armasm -g --cpu arm926ej-s --predefine "TCM SETL {TRUE}" --predefine "USE_SCATTER_SYMS SETL {TRUE}" ../source/926ej-s/inittcm.s
armasm -g --cpu arm926ej-s ../source/926ej-s/initmmu.s
armasm -g --cpu arm926ej-s ../source/926ej-s/initcache.s
armasm -g --cpu arm926ej-s ../source/vectors.s
armasm -g --cpu arm926ej-s ../source/scattersyms/dtcm.s
armasm -g --cpu arm926ej-s ../source/scattersyms/ttb.s
armcc -c -g --cpu arm926ej-s -O2 -Otime -W --no_inline -DMSC_CLOCK -D__CLK_TCK=100 ../dhry/dhry_1.c ../dhry/dhry_2.c -I../include
armcc -c -g --cpu arm926ej-s -O2 -DSTANDALONE -DLIGHT_LED ../source/retarget.c -I../include
armcc -c -g --cpu arm926ej-s -O2 -DSCATTER_PERIP ../source/clock_irq.c -I../include
armcc -c -g --cpu arm926ej-s -O2 -DSCATTER_PERIP ../source/serial.c -I../include
armcc -c -g --cpu arm926ej-s -O2 ../source/scattersyms/uart.c -I../include
armcc -c -g --cpu arm926ej-s -O2 ../source/scattersyms/irq.c -I../include
armcc -c -g --cpu arm926ej-s -O2 ../source/scattersyms/timer.c -I../include
armcc -c -g --cpu arm926ej-s -O2 -DUSE_SERIAL_PORT ../source/submain.c
armlink --scatter ../scatter/final_scat.scat init.o inittcm.o initmmu.o initcache.o vectors.o dtcm.o ttb.o dhry_1.o dhry_2.o retarget.o clock_irq.o serial.o uart.o irq.o timer.o submain.o -o dhry.axf --keep inittcm.o --keep initmmu.o --keep initcache.o --entry Reset_Handler --info totals --info unused

