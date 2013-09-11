armcc -c -g -O0 main.c -o main.o
armcc -c -g -O0 level1.c -o level1.o
armcc -c -g -O0 level2.c -o level2.o
armcc -c -g -O0 checking.c -o checking.o
armcc -c -g -O0 missing_region.c -o missing_region.o
armlink main.o level1.o level2.o checking.o missing_region.o -o mmugen.axf

