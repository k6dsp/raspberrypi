/* ================================================================== */
/* Copyright ARM Ltd 2002-2011. All rights reserved.                  */
/*                                                                    */
/* ARM1136JF-S Embedded example - Startup Code                         */
/* ================================================================== */

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define N               1000

#if N > 1000
#error "Value of N too big, must be <= 1000"
#endif

#define LOG10_N         6
#define N_FORMAT        "%06d"

extern void PUT32 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );
extern void dummy ( unsigned int );

#define GPFSEL1 0x20200004
#define GPSET0  0x2020001C
#define GPCLR0  0x20200028
#define GPPUD       0x20200094
#define GPPUDCLK0   0x20200098

#define AUX_ENABLES     0x20215004
#define AUX_MU_IO_REG   0x20215040
#define AUX_MU_IER_REG  0x20215044
#define AUX_MU_IIR_REG  0x20215048
#define AUX_MU_LCR_REG  0x2021504C
#define AUX_MU_MCR_REG  0x20215050
#define AUX_MU_LSR_REG  0x20215054
#define AUX_MU_MSR_REG  0x20215058
#define AUX_MU_SCRATCH  0x2021505C
#define AUX_MU_CNTL_REG 0x20215060
#define AUX_MU_STAT_REG 0x20215064
#define AUX_MU_BAUD_REG 0x20215068

#define ARM_TIMER_LOD 0x2000B400
#define ARM_TIMER_VAL 0x2000B404
#define ARM_TIMER_CTL 0x2000B408
#define ARM_TIMER_CLI 0x2000B40C
#define ARM_TIMER_RIS 0x2000B410
#define ARM_TIMER_MIS 0x2000B414
#define ARM_TIMER_RLD 0x2000B418
#define ARM_TIMER_DIV 0x2000B41C
#define ARM_TIMER_CNT 0x2000B420

#define SYSTIMERCLO 0x20003004

volatile unsigned int icount;

int getTimerCount()
{
	return GET32(ARM_TIMER_CNT);
}


extern void core_init(void);

#if N <= 10000
static void insert_sort(char *strings[], int n)
{
    char *v, *t;
    char **strp, **endp;
    int i;

    endp = &strings[N-1];
    i = N-2;
    do {
        strp = &strings[i];
        v = strp[0];
        do {
            t = strp[1];
            if (strcmp(v, t) <= 0) break;
            *strp++ = t;
        } while (strp < endp);
        strp[0] = v;
    } while (--i >= 0);
}
#endif

static void shell_sort(char *strings[], int n)
{
    int h, i, j;
    char *v;

    strings--;        /* Make array 1 origin */
    h = 1;
    do {h = h * 3 + 1;} while (h <= n);
    do {
        h = h / 3;
        for (i = h + 1; i <= n; i++) {
            v = strings[i];
            j = i;
            while (j > h && strcmp(strings[j-h], v) > 0) {
                strings[j] = strings[j-h];
                j = j-h;
            }
            strings[j] = v;
        }
    }
    while (h > 1);
}

static void randomise(char *strings[], int n)
{
    int i;
    int v;
    char *t;

    srand(1);  /* for repeatable results.  Or use  srand(clock());  for 'more random' results */
    for (i = 0; i < N; i++) {
        v = rand() % N;
        t = strings[v];
        strings[v] = strings[i];
        strings[i] = t;
    }
}

static void check_order(char *sort_type, char *strings[], int n)
{
    int i;

    for (i = 0; i < n; i++) {
        if (atoi(strings[i]) != i) {
            fprintf(stderr, "%s sort failed - exiting\n", sort_type);
            exit(1);
        }
    }
}

int qs_string_compare(const void *a, const void *b)
{
    return strcmp(*(char **)a, *(char **)b);
}

int main(void)
{
    char *strings[N], *strings_copy[N];
    char buffer[N*(LOG10_N+1)];
    char *p;
    clock_t starttime, endtime;
    int i;

	unsigned int ra;


	PUT32(AUX_ENABLES,1);
	PUT32(AUX_MU_IER_REG,0);
	PUT32(AUX_MU_CNTL_REG,0);
	PUT32(AUX_MU_LCR_REG,3);
	PUT32(AUX_MU_MCR_REG,0);
	PUT32(AUX_MU_IER_REG,0);
	PUT32(AUX_MU_IIR_REG,0xC6);
	PUT32(AUX_MU_BAUD_REG,270);

	ra=GET32(GPFSEL1);
	ra&=~(7<<12); //gpio14
	ra|=2<<12;    //alt5
	ra&=~(7<<15); //gpio15
	ra|=2<<15;    //alt5
	PUT32(GPFSEL1,ra);

	PUT32(GPPUD,0);
	for(ra=0;ra<150;ra++) dummy(ra);
	PUT32(GPPUDCLK0,(1<<14)|(1<<15));
	for(ra=0;ra<150;ra++) dummy(ra);
	PUT32(GPPUDCLK0,0);

	PUT32(AUX_MU_CNTL_REG,3);

	PUT32(ARM_TIMER_CTL,0x00F90000);
	PUT32(ARM_TIMER_CTL,0x00F90200);


    core_init();                    // does some extra setup work


    p = buffer;
    for (i = 0; i < N; i++) {
        sprintf(p, N_FORMAT, i);
        strings[i] = p;
        p += LOG10_N+1;
    }
    randomise(strings, N);

#if N <= 10000
    /* Do insertion sort */
    memcpy(strings_copy, strings, sizeof(strings));
    starttime = clock();
    insert_sort(strings_copy, N);
    endtime = clock();
    check_order("Insertion", strings_copy, N);
    printf("Insertion sort took %d clock ticks, end time %d and start time %d\n", endtime - starttime, endtime, starttime);
#else
    printf("Value of N too big to use insertion sort, must be <= 10000\n");
#endif

    /* Do shell sort */
    memcpy(strings_copy, strings, sizeof(strings));
    starttime = clock();
    shell_sort(strings_copy, N);
    endtime = clock();
    check_order("Shell", strings_copy, N);
    printf("Shell sort took %d clock ticks, end time %d and start time %d\n", endtime - starttime, endtime, starttime);

    /* Do quick sort - use built-in C library sort */
    memcpy(strings_copy, strings, sizeof(strings));
    starttime = clock();
    qsort(strings_copy, N, sizeof(char *), qs_string_compare);
    endtime = clock();
    check_order("Quick", strings_copy, N);
    printf("Quick sort took %d clock ticks, end time %d and start time %d\n", endtime - starttime, endtime, starttime);

    return 0;
}
