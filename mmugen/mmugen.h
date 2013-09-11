/*********************************************************************
 *
 * mmugen.h
 *
 * Contains structures to store page table information, file pointers,
 * and valid rule file strings. It also contains function prototypes for
 * all the functions contained in this application.
 *
 * functions:
 *  none
 *
 * Copyright Peter Maloy, CodeSprite August 2000    www.codesprite.com 
 *
 * Modified January, 2008 Elizabeth Norris, ARM Inc.
 *
 **********************************************************************/


#ifndef MMUGEN_H_
#define MMUGEN_H_

/* Structure containing the attributes and addresses set for a particular memory region */
typedef struct
{ 
  unsigned int v_base;       /* first address in virtual section */
  unsigned int v_end;        /* last address in virtual section */
  unsigned int v_end_saved;  /* save end address of last section */
  unsigned int p_base;       /* corresponding physical base address */
  unsigned int entrytype;    /* 'P' for pages, 'S' for section, 'H' for supersection, 'L' for largepages, 'S' for smallpages, 'E' for extended smallpages */
  unsigned int v6;           /* using v6 page table format */
  unsigned int v7;           /* using v7 page table format */
  unsigned int ro;           /* readonly access permission */
  unsigned int access;       /* access types defined in the enum above */
  unsigned int p;            /* EEC enabled for memory region */
  unsigned int domain;       /* set domain of region */
  unsigned int n_s;          /* non secure tells whether memory region corresponding to page is secure or nonsecure */
  unsigned int nglobal;      /* global translations */
  unsigned int shared;       /* shared or not shared memory */
  unsigned int execn;        /* region executable */
  unsigned int outer;        /* set outer cache attributes */
  unsigned int tex;          /* region type */
  unsigned int cb;           /* cacheable & bufferable status */
  unsigned int def;          /* defined a default table entry for missing sections */
  unsigned int def1;         /* defined a default table entry for L1 missing sections */
  unsigned int def2;         /* defined a default table entry for L2 missing sections */
  unsigned int d_entrytype;  /* default - 'P' for pages, 'S' for section, 'H' for supersection, 'L' for largepages, 'S' for smallpages, 'E' for extended smallpages */
  unsigned int d_ro;         /* default - readonly access permission */
  unsigned int d_access;     /* default - access types defined in the enum above */
  unsigned int d_p;          /* default - EEC enabled for memory region */
  unsigned int d_domain;     /* default - set domain of region */
  unsigned int d_n_s;        /* default - non secure tells whether memory region corresponding to page is secure or nonsecure */
  unsigned int d_nglobal;    /* default - global translations */
  unsigned int d_shared;     /* default - shared or not shared memory */
  unsigned int d_execn;      /* default - region executable */
  unsigned int d_tex;        /* default - region type */
  unsigned int d_cb;         /* default - cacheable & bufferable status */
}mmudata;

typedef struct
{
  long index;
  char *word;
}tparsedata;

typedef struct {
  FILE *rulefile ;              /* pointer to the input rule file */
  FILE *outfile ;               /* pointer to the output binary file */
  FILE *entry_outfile;          /* pointer to the page table entry output file */
  FILE *l2_outfile;             /* pointer to the Level 2 binary file */
  unsigned int base_address ;   /* the base address of the Level 1 page table */
  unsigned int base_address_l2; /* the base address of the Level 2 page table */
  unsigned int table_entry ;    /* how many entries are currently in the Level 1 table */
  unsigned int table_entry_2;   /* how many entries are currently in the Level 2 table */
  long state ;                  /* tells whether you are generating entries for the Level 1 or Level 2 page table */
  long pagescount ;             /* number of Level 2 page tables */ 
  mmudata rule ;
} Global ;

enum validwords {
  BASE_ADDRESS,                 /* address of the start of the page table entries */     
  LEVEL,                        /* which page table entries you are writing */
  VSIX,                         /* using v6 formatted page tables */
  VSEVEN,                       /* using v7 formatted page tables */
  DEFAULT,                      /* defines the default page table entry for missing memory regions in the Level 1 and Level 2 tables */
  VIRTUAL,                      /* defines the virtual addresses */
  TO,                           /* used to specify the range of virtual addresses */
  PHYSICAL,                     /* defines the phyical address */
  PAGES,                        /* divide memory region into pages (4 or 64 KB) */
  SECTION,                      /* divide memory region into sections (1 MB) */
  SUPERSECTION,                 /* divide memory region into supersections (16 MB) */
  READWRITE,                    /* readwrite memory */
  READONLY,                     /* readonly memory */
  NO_ACCESS,
  SVC_READWRITE,
  NO_USR_WRITE,
  FULL_ACCESS,
  FAULT,                        /* divide memory region into faults (1 MB) */
  SHARED,                       /* memory shared with other components (processors/peripherals) */
  NOT_GLOBAL,                   /* the translation is marked as processor-specific in the TLB */
  EXECUTE_NEVER,                /* memory region is not-executable */
  NOT_SECURE,                   /* the memory region is accessed with Non-Secure rights (ARM1176 & Cortex-A8 only) */
  EEC,                          /* memory supports error checking */
  DOMAIN_FIFTEEN,               /* sets domain number associated with memory region */
  DOMAIN_FOURTEEN,
  DOMAIN_THIRTEEN,
  DOMAIN_TWELVE,
  DOMAIN_ELEVEN,
  DOMAIN_TEN,
  DOMAIN_NINE,
  DOMAIN_EIGHT,
  DOMAIN_SEVEN,
  DOMAIN_SIX,
  DOMAIN_FIVE,
  DOMAIN_FOUR,
  DOMAIN_THREE,
  DOMAIN_TWO,
  DOMAIN_ONE,
  DOMAIN_ZERO,        
  CACHEABLE,                    /* cacheable memory */
  BUFFERABLE,                   /* bufferable memory */
  OUTER,                        /* setting outer cache attributes to non-cacheable/non-bufferable */
  OUTER_B,                      /* setting outer cache attributes to non-cacheable/bufferable */
  OUTER_C,                      /* setting outer cache attributes to cacheable/non-bufferable */
  OUTER_CB,                     /* setting outer cache attributes to cacheable/bufferable */
  NORMAL,                       /* defining memory region as normal */
  DEVICE,                       /* defining memory region as device */
  STRONGLY_ORDERED,             /* defining memory region as strongly-ordered */
  LARGEPAGES,                   /* divide memory region into largepages (64 KB) */
  SMALLPAGES,                   /* divide memory region into smallpages (4 KB - v4/v5 page tables only) */
  EXTENDED_SMALLPAGES,          /* divide memory region into extended smallpages (4 KB) */
  POSTPROCESS,                  /* defines postprocessing information */
  UNRECOGNISED                  /* unknown text in rule file */
};

//--function prototypes------------------------------------------------------
int  ParseRule (int* error);
int  WriteTableEntries (int* error);
int  WriteLevel1(int* error);
int  WriteLevel2(int* error);
int  GetWord (char *buffer);
int  GetWordIndex (char *buffer);
int  atox( char *buffer);
int  stricomp( char *original, char *match );
void printEntry_L1(unsigned int writevalue);
void printEntry_L2v4(unsigned int writevalue);
void printEntry_L2v6(unsigned int writevalue);
void printMissingEntry_L1(unsigned int writevalue);
void printMissingEntry_L2v4(unsigned int writevalue);
void printMissingEntry_L2v6(unsigned int writevalue);
int WriteMissingRegionL1(int* error);
int WriteMissingRegionL2(int* error);

#endif /*MMUGEN_H_*/


