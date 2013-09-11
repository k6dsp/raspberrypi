/*********************************************************************
 *
 * main.c
 *
 * Contains functions to:
 *  Setup the file pointers for the input rule file and output binary and information files
 *  Parse the input rule file and set the corresponding bits in the page table
 *  Call the appropriate functions to generate and write the entries to the correct output binary file 
 *
 * functions:
 *  int atox( char *buffer);
 *  int stricomp( char *original, char *match );
 *  int GetWordIndex (char *buffer);
 *  int GetWord (char *buffer);
 *  int main(void);
 *  int ParseRule (int* error);
 *  int WriteTableEntries (int* error); 
 *
 * Copyright Peter Maloy, CodeSprite August 2000    www.codesprite.com 
 *
 * Modified January, 2008 Elizabeth Norris, ARM Inc.
 *
 **********************************************************************/


//--HEADER FILES---------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "mmugen.h"

//--GLOBALS---------------------------------------------------------------------
Global global;
tparsedata parsedata[] = {
  {BASE_ADDRESS, "BASE_ADDRESS"},
  {LEVEL, "LEVEL"},
  {VSIX, "VSIX"},
  {VSEVEN, "VSEVEN"},
  {DEFAULT, "DEFAULT"},
  {VIRTUAL, "VIRTUAL"},
  {TO, "TO"},
  {PHYSICAL, "PHYSICAL"},
  {PAGES, "PAGES"},
  {SECTION, "SECTION"},
  {SUPERSECTION, "SUPERSECTION"},
  {READWRITE, "READWRITE"},
  {READONLY, "READONLY"},
  {NO_ACCESS, "NO_ACCESS"},
  {SVC_READWRITE, "SVC_READWRITE"},
  {NO_USR_WRITE, "NO_USR_WRITE"},
  {FULL_ACCESS, "FULL_ACCESS"},
  {FAULT, "FAULT"},
  {SHARED, "SHARED"},        
  {NOT_GLOBAL, "NOT_GLOBAL"},   
  {EXECUTE_NEVER, "EXECUTE_NEVER"},
  {NOT_SECURE, "NOT_SECURE"},
  {EEC, "EEC"},
  {DOMAIN_FIFTEEN,"DOMAIN_FIFTEEN"},
  {DOMAIN_FOURTEEN,"DOMAIN_FOURTEEN"},
  {DOMAIN_THIRTEEN,"DOMAIN_THIRTEEN"},
  {DOMAIN_TWELVE,"DOMAIN_TWELVE"},
  {DOMAIN_ELEVEN,"DOMAIN_ELEVEN"},
  {DOMAIN_TEN,"DOMAIN_TEN"},
  {DOMAIN_NINE,"DOMAIN_NINE"},
  {DOMAIN_EIGHT,"DOMAIN_EIGHT"},
  {DOMAIN_SEVEN,"DOMAIN_SEVEN"},
  {DOMAIN_SIX,"DOMAIN_SIX"},
  {DOMAIN_FIVE,"DOMAIN_FIVE"},
  {DOMAIN_FOUR,"DOMAIN_FOUR"},
  {DOMAIN_THREE,"DOMAIN_THREE"},
  {DOMAIN_TWO,"DOMAIN_TWO"},
  {DOMAIN_ONE,"DOMAIN_ONE"},
  {DOMAIN_ZERO,"DOMAIN_ZERO"},        
  {CACHEABLE, "CACHEABLE"},
  {BUFFERABLE, "BUFFERABLE"},
  {OUTER, "OUTER"},
  {OUTER_B,"OUTER_B"},
  {OUTER_C,"OUTER_C"},
  {OUTER_CB,"OUTER_CB"},  
  {NORMAL, "NORMAL"},
  {DEVICE, "DEVICE"},
  {STRONGLY_ORDERED, "STRONGLY_ORDERED"},
  {LARGEPAGES, "LARGEPAGES"},
  {SMALLPAGES, "SMALLPAGES"},
  {EXTENDED_SMALLPAGES, "EXTENDED_SMALLPAGES"},
  {POSTPROCESS, "POSTPROCESS"},
  {UNRECOGNISED, "UNRECOGNISED"}
};

/*********************************************************************
 * atox
 * 
 * Translates ASCII string to its decimal equivalent
 * 
 * inputs:
 *   char *buffer
 *     contains the word read from the input rule file
 * 
 * outputs:
 *   returns decimal equivalent of input string
 * 
 **********************************************************************/

int atox( char *buffer)
{
  int v=0;
        
  while(*buffer)
  {
    v*=16;
    if(*buffer>='0' && *buffer<='9') v+= *buffer - '0';
    if(*buffer>='a' && *buffer<='f') v+= *buffer - 'a'+10;
    if(*buffer>='A' && *buffer<='F') v+= *buffer - 'A'+10;
    buffer++;
  }
  
  return v ;
}

/*********************************************************************
 * stricomp
 * 
 * Compares string acquired from rule file to string values listed
 * in parsedata
 * 
 * inputs:
 *   char *orginal
 *     contains a word from the parsedata struct
 *   char *match
 *     contains the word read from the input rule file
 * 
 * outputs:
 *   returns 0 if match found
 *   returns 1 if match not found
 * 
 **********************************************************************/

int stricomp( char *original, char *match )
{
  char *ptr = match ;

  while(*ptr)
  {
    *ptr=toupper(*ptr) ;
    ptr++ ;
  }
        
  return strcmp(original,match) ;
}

/*********************************************************************
 * GetWordIndex
 * 
 * Traverses parsedata until a match is found for the acquired string
 * or it reaches the end of parsedata 
 * 
 * inputs:
 *   char *buffer
 *     contains the word read from the input rule file
 * 
 * outputs:
 *   returns current value of index
 * 
 **********************************************************************/

int GetWordIndex (char *buffer)
{
  int index=0 ;
  
  while ( parsedata[index].index != UNRECOGNISED && stricomp(parsedata[index].word,buffer) )
  {
    index++ ;
  }
        
  return index ;
}

/*********************************************************************
 * GetWord
 * 
 * Fetches next word in the rule file by stepping over whitespace(s) 
 * and user comments
 * 
 * inputs:
 *   char *buffer
 *     array to hold the word read from the input rule file
 *  
 * outputs:
 *   returns 1 if word was successfully read
 *   returns 0 if it reached the end of the rule file
 * 
 **********************************************************************/

int GetWord (char *buffer)
{
  int c=fgetc(global.rulefile);

  while(c=='\t' || c==' ' || c=='/' || c==0xd || c==0xa)
  {
    if( c=='/') //start of comment
    {
      do
      {
        c=fgetc(global.rulefile);
      } while( c!= '/' ) ;

      // get next character beyond comment
      c=fgetc(global.rulefile);
    }
    
    while( c=='\t' || c==' ' || c==0xd || c==0xa ) c=fgetc(global.rulefile);
  }

  // Check for end of file
  if( c==EOF ) return 0;

  // Ok, should be at the start of a word
  while ( c!='\t' && c!=' ' && c!=0xd && c!=0xa && c!=EOF)
  {
    *buffer++ = (char)c ;
    c=fgetc(global.rulefile);
  }

  //terminate word
  *buffer = '\0';

  return 1;
}

/*********************************************************************
 * main
 * 
 * Sets the file pointers according to user input.
 * Calls ParseRule to parse the input rule file.
 * Closes files if an error occurs or program completes. 
 * 
 * inputs:
 *   none
 * 
 * outputs:
 *   returns -1 if an error occurred
 *   returns 0 if program completed successfully
 * 
 **********************************************************************/

int main(void)
{
  char rule[20];
  char bin[20];
  int error = 1; //set to 0 if an unrecoverable error occurs
  
  global.state=0;
  global.table_entry = 0;
  global.rule.v_end=0x0;
    
  printf("Name of rule file: ");  
  scanf("%s", rule);
    
  if(NULL==(global.rulefile=fopen(rule,"r")))
  {
    printf("Error 1: Couldn't open Rule file\n");
    return -1;
  }
  
  printf("Name of output file for Level 1 table: ");
  scanf("%s", bin);

  if(NULL==(global.outfile=fopen(bin,"wb")))
  {
    fclose(global.outfile);
    printf("Error 2: Couldn't open Output file\n");
    return -1;
  }
  
  printf("Name of page table entry output file: ");
  scanf("%s", bin);

  if(NULL==(global.entry_outfile=fopen(bin,"wb")))
  {
    fclose(global.entry_outfile);
    printf("Error 2: Couldn't open Output file\n");
    return -1;
  }
  
  printf("Do you have a Level 2 table? (y/n) ");
  scanf("%s", bin);
  
  /* Level 2 table was defined */
  if(bin[0] == 'y')
  {
    printf("Name of output file for Level 2 table: ");
    scanf("%s", bin);

    if(NULL==(global.l2_outfile=fopen(bin,"wb")))
    {
      fclose(global.l2_outfile);
      printf("Error 2: Couldn't open Output file\n");
      return -1;
    }
    
    printf("Base address of Level 2 table: ");
    scanf("%s", bin);
    global.base_address_l2 = atox(bin);
  }
  /* Level 2 table was not defined */
  else
  {
    /* setting Level 2 table base to invalid value to check whether Level 2 was specified when it should not have been */
    global.base_address_l2 = 0xFFFFFFFF;
  }
  
  while( ParseRule(&error) )
  {
    if(WriteTableEntries(&error)==-1)
    {
      fclose( global.outfile );
      fclose( global.rulefile );
      fclose( global.entry_outfile );
      fclose( global.l2_outfile );
      exit(-1);
    }
    
    //check for terminal error
    if(!error)
    {
      fclose( global.outfile );
      fclose( global.rulefile );
      fclose( global.entry_outfile );
      fclose( global.l2_outfile );
      printf("Error 3: MMU table could not be generated\n");
      return 0 ;
    }
    
    global.rule.def = 0; //clear default flag
  }

  printf("MMU table generation complete\n");
        
  fclose( global.outfile );
  fclose( global.rulefile );
  fclose( global.entry_outfile );
  fclose( global.l2_outfile );
  return 0 ;
}

/*********************************************************************
 * ParseRule
 * 
 * Sets the appropiate global rule bits based on the rule file input
 * 
 * inputs:
 *   int* error
 *     contains the current operation status:=
 *      1 if no unrecoverable error has occurred
 *      0 if an unrecoverable error has occurred
 * 
 * outputs:
 *   returns 1 if rule file entry was read completely
 *   returns 0 if an error occurred or the word read was not recognized
 * 
 **********************************************************************/

int ParseRule (int* error)
{
  char buffer[20];
  int i;

  /* fetch next word from rule file */
  while(GetWord(buffer))
  {
    /* set the appropriate entry bit depending on the word fetched from the rule file */
    switch( GetWordIndex(buffer) )
    {
      /* grabs the base address of the mmu table */
      case BASE_ADDRESS:
      
        GetWord(buffer);
        global.base_address = atox(buffer); //converts the address into a decimal value
        printf("Base Address = 0x%x \n", global.base_address);
        fprintf(global.entry_outfile, "Base Address = 0x%x \n", global.base_address);
        break;
       
      /* mmu table using v6 format table entries */                                 
      case VSIX:
      
        global.rule.v6 = 1;
        printf("Format = v6 \n");
        fprintf(global.entry_outfile, "Format = v6 \n");
        break;
      
      /* mmu table using v7 format table entries */  
      case VSEVEN:
        global.rule.v7 = 1;
        printf("Format = v7 \n");
        fprintf(global.entry_outfile, "Format = v7 \n");
        break;
      
      /* grabs the Level number (1 or 2) */                                  
      case LEVEL:
      
        GetWord(buffer);
        global.state = atoi(buffer);  //converts the level number to a decimal value
        /* checks to see if the Level 1 table is 4096 entries */
        if(global.table_entry > 0x1000)
          printf("Warning 1: The Level 1 page table is %d entries, instead of 4096\n",global.table_entry);
        printf("Level = %d \n",global.state);
        fprintf(global.entry_outfile, "Level = %d \n", global.state);
        break;
      
      /* defines default table entry for missing memory regions */  
      case DEFAULT:
              
        global.rule.def = 1;
        
        /* defined default for L1 page table */
        if( global.state == 1 )
          global.rule.def1 = 1;
        /* defined default for L2 page table */
        else
          global.rule.def2 = 1;
        break;
       
      /* grabs the start address of the virtual memory region */  
      case VIRTUAL:
      
        GetWord(buffer);
        global.rule.v_base =  atox(buffer); //converts the address into a decimal value
        printf("Virtual Address = 0x%x ",global.rule.v_base);
        fprintf(global.entry_outfile, "--- Virtual Address = 0x%x ",global.rule.v_base);
        break;

      /* separates the starting virtual address from the ending virtual address */
      case TO:
      
        global.rule.v_end_saved = global.rule.v_end; //save end address of last section      
        GetWord(buffer); //grabs the end address of the virtual memory region        
        global.rule.v_end =  atox(buffer); //converts the address into a decimal value
        printf("to 0x%x ",global.rule.v_end);
        fprintf(global.entry_outfile, "to 0x%x ",global.rule.v_end);
        break;

      /* grabs the start address of the physical memory region */
      case PHYSICAL:
      
        GetWord(buffer);
        global.rule.p_base =  atox(buffer); //converts the address into a decimal value
        printf("Physical Address = 0x%x \n",global.rule.p_base);
        fprintf(global.entry_outfile, "Physical Address = 0x%x ---\n",global.rule.p_base);
        break;
      
      /* the memory region is divided into 1MB fault sections */  
      case FAULT:
      
        /* setting default table entry for missing memory regions */
        if( global.rule.def )
          global.rule.d_entrytype = 'F'; 
        else
          global.rule.entrytype = 'F';
        /* have hit the end of the rule entry */
        return 1;
       
      /* the memory region is divided into pages (4KB or 64KB) */           
      case PAGES:
      
        /* setting default table entry for missing memory regions */
        if( global.rule.def )
          global.rule.d_entrytype = 'P';
        else
          global.rule.entrytype = 'P';
        /* have hit the end of the rule entry */
        return 1;

      /* the memory region is divided into sections (1MB) */
      case SECTION:
      
        /* setting default table entry for missing memory regions */
        if( global.rule.def )
          global.rule.d_entrytype = 'S';
        else      
          global.rule.entrytype = 'S';
        /* have hit the end of the rule entry */
        return 1;
      
      /* the memory region is divided into supersections (16MB) */                                  
      case SUPERSECTION:
      
        /* setting default table entry for missing memory regions */
        if( global.rule.def  )
          global.rule.d_entrytype = 'H';
        else
          global.rule.entrytype = 'H';
        /* have hit the end of the rule entry */
        return 1;
      
      /* sets APX bit to 0 */                                  
      case READWRITE:  
      
        /* setting default table entry for missing memory regions */
        if( global.rule.def )
          global.rule.d_ro = 0;  
        else
          global.rule.ro = 0;  
        break; 
      
      /* sets APX bit to 1 */                  
      case READONLY:   
      
        /* setting default table entry for missing memory regions */
        if( global.rule.def )
          global.rule.d_ro = 1; 
        else 
          global.rule.ro = 1; 
        break;                        
      
      /* sets AP bits to 0 */                  
      case NO_ACCESS:
        
        /* a v7 page table entry cannot have attributes of READONLY and NO_ACCESS */
        if( global.rule.v7 == 1 && global.rule.ro == 1 )
        {
          printf("Error 4: READONLY and NO_ACCESS are not attributes for v7 page tables \n");
          *error = 0;  //generated binary file will be incorrect
          return 0;
        }
          
        /* setting default table entry for missing memory regions */
        if( global.rule.def )
          global.rule.d_access = 0 ;
        else
          global.rule.access = 0 ;
        break;

      /* sets AP bits to 1 */
      case SVC_READWRITE:
      
        /* setting default table entry for missing memory regions */
        if( global.rule.def )
          global.rule.d_access = 1 ; 
        else
          global.rule.access = 1 ; 
        break;

      /* sets AP bits to 2 */
      case NO_USR_WRITE:
      
        /* setting default table entry for missing memory regions */
        if( global.rule.def )
          global.rule.d_access = 2 ; 
        else
          global.rule.access = 2 ; 
        break;

      /* sets AP bits to 3 */
      case FULL_ACCESS:
      
        /* setting default table entry for missing memory regions */
        if( global.rule.def )
          global.rule.d_access = 3 ; 
        else
          global.rule.access = 3 ; 
        break;

      /* sets S bit to 1 */
      case SHARED:
      
        /* setting default table entry for missing memory regions */
        if( global.rule.def )
          global.rule.d_shared = 1; 
        else
          global.rule.shared = 1; 
        break;
      
      /* sets nG bit to 1 */                                  
      case NOT_GLOBAL:
      
        /* setting default table entry for missing memory regions */
        if( global.rule.def )
          global.rule.d_nglobal = 1; 
        else      
          global.rule.nglobal = 1;
        break;
        
      /* sets XN bit to 1 */                
      case EXECUTE_NEVER:
      
        /* setting default table entry for missing memory regions */
        if( global.rule.def )
          global.rule.d_execn = 1; 
        else
          global.rule.execn = 1; 
        break;
      
      /* sets NS bit to 1 */                                
      case NOT_SECURE:
      
        /* setting default table entry for missing memory regions */
        if( global.rule.def )
        {
          /* only set the NS bit if using v6/v7 page table format*/
          if ( global.rule.v6 == 1 || global.rule.v7 == 1)
          {
            global.rule.d_n_s = 1;
          }
        }
        else 
        {         
          /* only set the NS bit if using v6/v7 page table format*/
          if ( global.rule.v6 == 1 || global.rule.v7 == 1)
          {
            global.rule.n_s = 1;
          }
        }
        break;
      
      /* sets P bit to 1 */                                 
      case EEC:
      
        /* setting default table entry for missing memory regions */
        if( global.rule.def )
          global.rule.d_p = 1; 
        else
          global.rule.p = 1; 
        break;
      
      /* sets Domain Selector to 15 */                                  
      case DOMAIN_FIFTEEN:
      
        /* setting default table entry for missing memory regions */
        if( global.rule.def )
          global.rule.d_domain = 15;
        else
          global.rule.domain = 15;
        break;
      
      /* sets Domain Selector to 14 */                                  
      case DOMAIN_FOURTEEN:
      
        /* setting default table entry for missing memory regions */
        if( global.rule.def )
          global.rule.d_domain = 14;
        else
          global.rule.domain = 14;
        break;
      
      /* sets Domain Selector to 13 */                              
      case DOMAIN_THIRTEEN:
      
        /* setting default table entry for missing memory regions */
        if( global.rule.def )
          global.rule.d_domain = 13;
        else
          global.rule.domain = 13; 
        break;
      
      /* sets Domain Selector to 12 */                                  
      case DOMAIN_TWELVE:
      
        /* setting default table entry for missing memory regions */
        if( global.rule.def )
          global.rule.d_domain = 12;
        else
          global.rule.domain = 12;
        break;
      
      /* sets Domain Selector to 11 */                                  
      case DOMAIN_ELEVEN:
      
        /* setting default table entry for missing memory regions */
        if( global.rule.def )
          global.rule.d_domain = 11; 
        else
          global.rule.domain = 11;
        break;
      
      /* sets Domain Selector to 10 */                                  
      case DOMAIN_TEN:
      
        /* setting default table entry for missing memory regions */
        if( global.rule.def )
          global.rule.d_domain = 10;
        else
          global.rule.domain = 10;
        break;
      
      /* sets Domain Selector to 9 */                               
      case DOMAIN_NINE:
      
        /* setting default table entry for missing memory regions */
        if( global.rule.def )
          global.rule.d_domain = 9; 
        else
          global.rule.domain = 9;
        break;
      
      /* sets Domain Selector to 8 */                               
      case DOMAIN_EIGHT:
           
        /* setting default table entry for missing memory regions */ 
        if( global.rule.def )
          global.rule.d_domain = 8; 
        else
          global.rule.domain = 8; 
        break;
      
      /* sets Domain Selector to 7 */                               
      case DOMAIN_SEVEN:
      
        /* setting default table entry for missing memory regions */
        if( global.rule.def )
          global.rule.d_domain = 7; 
        else
          global.rule.domain = 7; 
        break;
      
      /* sets Domain Selector to 6 */                               
      case DOMAIN_SIX:
      
        /* setting default table entry for missing memory regions */
        if( global.rule.def )
          global.rule.d_domain = 6; 
        else
          global.rule.domain = 6;
        break;
      
      /* sets Domain Selector to 5 */                               
      case DOMAIN_FIVE:
      
        /* setting default table entry for missing memory regions */
        if( global.rule.def )
          global.rule.d_domain = 5; 
        else
          global.rule.domain = 5; 
        break;
      
      /* sets Domain Selector to 4 */                               
      case DOMAIN_FOUR:
      
        /* setting default table entry for missing memory regions */
        if( global.rule.def )
          global.rule.d_domain = 4; 
        else
          global.rule.domain = 4; 
        break;
      
      /* sets Domain Selector to 3 */                               
      case DOMAIN_THREE:
      
        /* setting default table entry for missing memory regions */
        if( global.rule.def )
          global.rule.d_domain = 3; 
        else
          global.rule.domain = 3; 
        break;
      
      /* sets Domain Selector to 2 */                               
      case DOMAIN_TWO:
      
        /* setting default table entry for missing memory regions */
        if( global.rule.def )
          global.rule.d_domain = 2; 
        else
          global.rule.domain = 2; 
        break;
      
      /* sets Domain Selector to 1 */                               
      case DOMAIN_ONE:
      
        /* setting default table entry for missing memory regions */
        if( global.rule.def )
          global.rule.d_domain = 1; 
        else
          global.rule.domain = 1; 
        break;
      
      /* sets Domain Selector to 0 */                               
      case DOMAIN_ZERO:
      
        /* setting default table entry for missing memory regions */
        if( global.rule.def )
          global.rule.d_domain = 0; 
        else
          global.rule.domain = 0; 
        break;      
      
      /* sets C bit to 1 */                  
      case CACHEABLE:

        /* setting default table entry for missing memory regions */
        if( global.rule.def )
          global.rule.d_cb |= 2; 
        else
          global.rule.cb |= 2; 
        break;
        
      /* sets B bit to 1 */
      case BUFFERABLE:
      
        /* setting default table entry for missing memory regions */
        if( global.rule.def )
          global.rule.d_cb |= 1; 
        else
          global.rule.cb |= 1; 
        break;
      
      /* sets TEX bits to 4 */  
      case OUTER:
      
        global.rule.outer = 1; //outer cache attributes have been set
        /* setting outer cache as non-cacheable, non-bufferable */
        if( global.rule.def )
            global.rule.d_tex = 4; 
        else
            global.rule.tex = 4; 
        break;
      
      /* sets TEX bits to 5 */
      case OUTER_B:
      
        global.rule.outer = 1; //outer cache attributes have been set
        /* setting outer cache as non-cacheable, bufferable */
        if( global.rule.def )
            global.rule.d_tex = 5; 
        else
            global.rule.tex = 5; 
        break;
      
      /* sets TEX bits to 6 */
      case OUTER_C:
      
        global.rule.outer = 1; //outer cache attributes have been set
        /* setting outer cache as cacheable, non-bufferable */
        if( global.rule.def )
            global.rule.d_tex = 6; 
        else
            global.rule.tex = 6;    
        break;
      
      /* sets TEX bits to 7 */
      case OUTER_CB:
      
        global.rule.outer = 1; //outer cache attributes have been set      
        /* setting outer cache as cacheable, bufferable */
        if( global.rule.def )
            global.rule.d_tex = 7; 
        else
            global.rule.tex = 7; 
        break;

      /* marks memory as Normal by setting the TEX bits accordingly */
      case NORMAL:
      
        /* already specified TEX because outer cacheable attributes have been declared */
        if(global.rule.outer)
        {
          printf("Warning 2: Ignoring Normal memory type, TEX bits have already been set.\n");          
          break;
        }
        
        /* cacheable Normal memory */
        if((global.rule.cb & 2) == 2)
        {
          /* setting default table entry for missing memory regions */
          if( global.rule.def )
            global.rule.d_tex = 0; //TEX bits = 0
          else
            global.rule.tex = 0; //TEX bits = 0
        }
        /* not cacheable Normal memory */
        else
        {
          /* not bufferable, not cacheable Normal memory */
          if((global.rule.cb & 1) == 0) 
          {
            /* setting default table entry for missing memory regions */
            if( global.rule.def )
              global.rule.d_tex = 1; //TEX bits = 1
            else
              global.rule.tex = 1; //TEX bits = 1
          }
          /* bufferable, not cacheable Normal memory - not allowed */
          else 
          {
            printf("Error 5: Bufferable, not cacheable Normal memory is not allowed.\n");
            *error = 0;  //generated binary file will be incorrect
            return 0;
          }
        }
        break;  
      
      /* marks memory as Device by setting the TEX bits accordingly */                                                                          
      case DEVICE:
      
        /* already specified TEX because outer cacheable attributes have been declared */
        if(global.rule.outer)
        {
          printf("Warning 3: Ignoring Device memory type, TEX bits have already been set.\n");
          break;
        }
        
        /* cacheable Device memory - not allowed */
        if ((global.rule.cb & 2) == 2) 
        {   
          printf("Error 6: Device memory cannot be cacheable.\n");
          *error = 0;  //generated binary file will be incorrect
          return 0;
        }
        /* not cacheable, bufferable Device memory */
        else if((global.rule.cb & 1) == 1)  
        {
          /* setting default table entry for missing memory regions */
          if( global.rule.def )
            global.rule.d_tex = 0; //TEX bits = 0
          else
            global.rule.tex = 0; //TEX bits = 0
        }
        /* not cacheable, not bufferable Device memory */
        else 
        {
          /* setting default table entry for missing memory regions */
          if( global.rule.def )
            global.rule.d_tex = 2; //TEX bits = 2
          else
            global.rule.tex = 2; //TEX bits = 2
        }
        break; 
      
      /* marks memory as Strongly Ordered by setting the TEX bits accordingly */                  
      case STRONGLY_ORDERED:
      
        /* already specified TEX because outer cacheable attributes have been declared */
        if(global.rule.outer)
        {
          printf("Warning 4: Ignoring Strongly Ordered memory type, TEX bits have already been set.\n");
          break;
        }
        
        /* not cacheable, not bufferable Strongly Ordered memory*/
        if(global.rule.cb == 0) 
        {           
          /* setting default table entry for missing memory regions */
          if( global.rule.def )
            global.rule.d_tex = 0; //TEX bits = 0
          else
            global.rule.tex = 0; //TEX bits = 0
        }
        /* cacheable or bufferable Strongly Ordered memory - not allowed */
        else
        {
          printf("Error 7: Strongly Ordered memory cannot be cacheable or bufferable.\n");
          *error = 0;  //generated binary file will be incorrect
          return 0;
        }
        break;

      /* the memory region is divided into largepages (64KB) */
      case LARGEPAGES:
      
        /* setting default table entry for missing memory regions */
        if( global.rule.def )
          global.rule.d_entrytype = 'L';
        else
          global.rule.entrytype = 'L'; 
        /* have hit the end of the rule entry */
        return 1;

      /* the memory region is divided into smallpages (4KB) */
      case SMALLPAGES:
      
        /* setting default table entry for missing memory regions */
        if( global.rule.def )
          global.rule.d_entrytype = 'S'; 
        else
          global.rule.entrytype = 'S';
        /* have hit the end of the rule entry */
        return 1;
      
      /* the memory region is divided into extended_smallpages (4KB) */                                  
      case EXTENDED_SMALLPAGES:
      
        /* setting default table entry for missing memory regions */
        if( global.rule.def )
          global.rule.d_entrytype = 'E'; 
        else
          global.rule.entrytype = 'E'; 
        /* have hit the end of the rule entry */
        return 1;

      /* modifies a particular page table entry after it have been generated */
      case POSTPROCESS:
      
        GetWord(buffer); //skip ENTRY
        GetWord(buffer); //get address
        fseek( global.outfile, atox(buffer) ,SEEK_SET );
        GetWord(buffer); //skip EQUALS
        GetWord(buffer); //get value
        i = atox(buffer);
        fwrite( &i, 4, 1, global.outfile );
        break ;

      /* Have hit an unknown entry in the rule file */
      case UNRECOGNISED:
        
        printf("Error 8: Unrecognized word '%s'\n",buffer);
        *error = 0;  //generated binary file will be incorrect
        return 0;
    }
  }

  // must have hit end of rule file without finding the end of a rule
  return 0;
}

/*********************************************************************
 * WriteTableEntries
 * 
 * Call the appropiate page table generation function based on what
 * page table is being generated (L1 or L2)
 * 
 * inputs:
 *   int* error
 *     contains the current operation status:=
 *      1 if no unrecoverable error has occurred
 *      0 if an unrecoverable error has occurred
 * 
 * outputs:
 *   returns 1 if rule file entry was output to the file successfully
 *   returns 0 if an error occurred
 *   returns -1 if the page table level is unknown
 * 
 **********************************************************************/

int WriteTableEntries (int* error)
{
  /* Call the function to write either the Level 1 or Level 2 page table */
  switch( global.state )
  {
    /* currently generating Level 1 page table entries */
    case 1: return WriteLevel1(error);
    /* currently generating Level 2 page table entries */
    case 2:
      /* Level 2 table was specified in the rule file when it should not have been */
      if( global.base_address_l2 != 0xFFFFFFFF) 
        return WriteLevel2(error);
      else
      {
        printf("Error 21: Level 2 table was not declared at runtime.\n");
        *error = 0;
        return 0;
      }        
    /* the current level is unknown */
    default: printf("Error 9: MMU page table level not set\n");
      return -1;
  }
}


