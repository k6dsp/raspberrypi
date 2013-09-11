/*********************************************************************
 *
 * level1.c
 *
 * Contains a function that generates and writes the Level 1 page table
 * entries to the specified binary file
 *
 * functions:
 *  int WriteLevel1(int* error);
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
extern Global global;
extern tparsedata parsedata;

/*********************************************************************
 * WriteLevel1
 * 
 * create and write the Level 1 table entries to the specified
 * output file 
 * 
 * inputs:
 *   int* error
 *     contains the current operation status:=
 *      1 if no unrecoverable error has occurred
 *      0 if an unrecoverable error has occurred
 * 
 * outputs:
 *   returns 0 if an error occurred
 *   returns 1 if the table entry was successfully written to the output
 *     file
 * 
 **********************************************************************/

int WriteLevel1(int* error)
{
  unsigned int fields;
  unsigned int writevalue;
  unsigned int i, j;
  
  /* don't process rule entry if it is a DEFAULT entry */
  if(global.rule.def)
    return 0;

  /* Missing a definition for a memory region */
  if((global.rule.v_end_saved + 1) != global.rule.v_base && global.table_entry != 0)
  {
    printf("Error 10: Section not defined from 0x%x to 0x%x\n", (global.rule.v_end_saved + 1), (global.rule.v_base - 1));
            
    /* user has not defined a default L1 page table entry */
    if( !global.rule.def1 )
    {
      global.rule.d_entrytype = 'F'; //create fault page table entries for  missing memory regions
      printf("Filling missing page table entries with Fault entry\n");
    }
    else
    {
      printf("Filling missing page table entries with Level 1 Default entry\n");
    }
     
    /* errored occured while filling missing region */ 
    if(!WriteMissingRegionL1(error))
      return 0;
  }
  
  /* checking to see if virtual base and end addresses are divisible by the entry type */
  if (global.rule.entrytype == 'H') //supersection check
  {
    if((global.rule.v_base % 0x1000000) != 0 || ((global.rule.v_end + 1) % 0x1000000) != 0) //defined base or end address is not divisible by 16MB
    {
      printf("Error 11: Defined address range cannot be divided into 16MB sections.\n");
      *error = 0;  //generated binary file will be incorrect
      return 0;
    }
  }
  else //section and fault checking
  {
    if((global.rule.v_base % 0x100000) != 0 || ((global.rule.v_end + 1) % 0x100000) != 0) //defined base or end address is not divisible by 1MB
    {
      printf("Error 12: Defined address range cannot be divided into 1MB sections.\n");
      *error = 0;  //generated binary file will be incorrect
      return 0;
    }
  }
  
  /* set and write page table entry based on rule file entry type (Section, Supersection, Fault, or Pages) */
  switch (global.rule.entrytype)
  {      
    /* Section entry type */
    case 'S': 
      fields =  (global.rule.n_s<<19) | (global.rule.tex<<12) | (global.rule.access<<10) | (global.rule.p<<9) | (global.rule.domain<<5) | (global.rule.cb<<2) | 0x2;
                        
      if(global.rule.v6 == 1 || global.rule.v7 == 1) //using v6 or v7 page tables
      {
        fields |= (global.rule.nglobal<<17) | (global.rule.shared<<16) | (global.rule.ro<<15) | (global.rule.execn<<4);
      }
                        
      /* generate all the SECTION entries for this descriptor */
      for (i = global.rule.v_base; i<global.rule.v_end; )
      {
        writevalue = (global.rule.p_base & 0xFFF00000) | fields;
        fwrite( &writevalue, 4, 1, global.outfile );
        
        /* Print page table entry bit settings */
        printEntry_L1(writevalue);
        
        global.table_entry++ ;
        global.rule.p_base += 0x100000 ; //1MB

        /* check for memory wrap-round */
        i+=0x100000;
        if (i==0x00000000) break;
      }
      break; 
    
    /* Supersection entry type */
    case 'H': 
                                
      fields = (global.rule.n_s<<19) | 0x40000 | (global.rule.tex<<12) | (global.rule.access<<10) | (global.rule.p<<9) | (global.rule.cb<<2) | 0x2;
                        
      if(global.rule.v6 == 1 || global.rule.v7 == 1) //using v6 or v7 page tables
      {
        fields |= (global.rule.nglobal<<17) | (global.rule.shared<<16) | (global.rule.ro<<15) | (global.rule.execn<<4);
      }
                        
      /* generate all the SUPERSECTION entries for this descriptor */
      for (i = global.rule.v_base; i<global.rule.v_end; )
      {
        for (j=16; j > 0; j--) //create 16 duplicate entries (1 entry = 1 MB)
        {
          writevalue = (global.rule.p_base & 0xFF000000) | fields ;
          fwrite( &writevalue, 4, 1, global.outfile );
          
          /* Print page table entry bit settings */
          printEntry_L1(writevalue);
          
          global.table_entry++ ;
        }
                            
        global.rule.p_base += 0x1000000 ; //16MB

        /* check for memory wrap-round */
        i+=0x1000000;
        if (i==0x00000000) break;
      }
      break;
    
    /* Fault entry type */    
    case 'F':
                
      //write fault entry for every 1MB of fault region specified
      for (i = global.rule.v_base; i<global.rule.v_end; )
      {
        writevalue = 0;
        fwrite( &writevalue, 4, 1, global.outfile );
        
        /* Print page table entry bit settings */
        //printEntry_L1(writevalue);
        
        global.table_entry++ ;
            
        /* check for memory wrap-round */
        i+=0x100000;
        if (i==0x00000000) break;
      }
      break;
    
    /* Page entry type */    
    case 'P':
      
      //Physical Addr = base address of Level 2 table + (number of Level 2 page tables<<10) 
      writevalue = ((global.base_address_l2 + (global.pagescount++<<10)) & 0xFFFFFC00) | (global.rule.p<<9) | (global.rule.domain<<5) | (global.rule.n_s<<3) | 0x1;
      fwrite( &writevalue, 4, 1, global.outfile );
      
      /* Print page table entry bit settings */
      printEntry_L1(writevalue);
            
      global.table_entry++;
      break;
  
    /* Invalid entry type used */
    default: 
      printf("Error 13: An invalid entry type was entered.\n");
      *error = 0;  //generated binary file will be incorrect
      return 0;
 }     
        
  /* clear set values */
  global.rule.ro = 0;      
  global.rule.access = 0;   
  global.rule.p = 0; 
  global.rule.domain = 0; 
  global.rule.n_s = 0; 
  global.rule.nglobal = 0;  
  global.rule.shared = 0;  
  global.rule.execn = 0; 
  global.rule.tex = 0;  
  global.rule.cb = 0;
  global.rule.outer = 0;               
    
  return 0 ;
}


