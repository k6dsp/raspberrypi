/*********************************************************************
 *
 * level2.c
 *
 * Contains a function that generates and writes the Level 2 page table
 * entries to the specified binary file
 *
 * functions:
 *  int WriteLevel2(int* error);
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
 * WriteLevel2
 * 
 * create and write the Level 2 table entries to the specified
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

int WriteLevel2(int* error)
{
  unsigned int fields;
  unsigned int writevalue;
  unsigned int i, j;
  
  /* don't process rule entry if it is a DEFAULT entry */
  if(global.rule.def)
    return 0;
        
  /* Missing a definition for a memory region */
  if((global.rule.v_end_saved + 1) != global.rule.v_base && (global.table_entry_2 % 256) != 0)
  {
    printf("Error 10: Section not defined from 0x%x to 0x%x\n", (global.rule.v_end_saved + 1), (global.rule.v_base - 1));
        
    /* user has not defined a default L2 page table entry */
    if( !global.rule.def2 )
    {
      global.rule.d_entrytype = 'F'; //create fault page table entries for  missing memory regions
      printf("Filling missing page table entries with Fault entry\n");
    }
    else
    {
      printf("Filling missing page table entries with Level 2 Default entry\n");
    }
    
    /* errored occured while filling missing region */  
    if(!WriteMissingRegionL2(error))
      return 0;
  }
  
  /* checking to see if virtual base and end addresses are divisible by the entry type */
  if (global.rule.entrytype == 'L') //large page check
  {
      if((global.rule.v_base % 0x10000) != 0 || ((global.rule.v_end + 1) % 0x10000) != 0) //defined base or end address is not divisible by 64KB
      {
        printf("Error 14: Defined address range cannot be divided into 64KB sections.\n");
        *error = 0;  //generated binary file will be incorrect
        return 0;
      }
  } 
  else //small and fault page check
  {
    if((global.rule.v_base % 0x1000) != 0 || ((global.rule.v_end + 1) % 0x1000) != 0) //defined base or end address is not divisible by 4KB
    {
      printf("Error 15: Defined address range cannot be divided into 4KB sections.\n");
      *error = 0;  //generated binary file will be incorrect
      return 0;
    }
  }
   
  /* Using v6/v7 page table format */     
  if(global.rule.v6 || global.rule.v7)
  {
    /* set and write page table entry based on rule file entry type (Largepage, Extended smallpage, or Fault) */
    switch (global.rule.entrytype)
    {
      /* Largepage entry type */
      case 'L':
        fields = (global.rule.execn<<15) | (global.rule.tex<<12) | (global.rule.nglobal<<11) | (global.rule.shared<<10) | (global.rule.ro<<9) | (global.rule.access<<4) | (global.rule.cb<<2) | 1;
        break;
    
      /* Extended smallpage entry type */
      case 'E':
        fields = (global.rule.nglobal<<11) | (global.rule.shared<<10) | (global.rule.ro<<9) | (global.rule.tex<<6) | (global.rule.access<<4) | (global.rule.cb<<2) | 2 | global.rule.execn;
        break;
    
      /* Fault entry type */
      case 'F': 
        fields = 0x0;
        break;
      
      /* Invalid page entry type used */  
      default:
        printf("Error 16: An invalid PAGE type was entered.\n");
        *error = 0;  //generated binary file will be incorrect
        return 0;
    }
  }
  
  /* Using v4/v5 page table format */
  else
  {
    /* set and write page table entry based on rule file entry type (Largepage, Smalllpage, Extended smallpage, or Fault) */
    switch (global.rule.entrytype)
    {
      /* Largepage entry type */
      case  'L':
        fields = (global.rule.tex<<12) | (global.rule.access<<10) | (global.rule.access<<8) | (global.rule.access<<6) | (global.rule.access<<4) | (global.rule.cb<<2) | 1;
        break;  
    
      /* Smallpage entry type */
      case 'S':
        fields = (global.rule.access<<10) | (global.rule.access<<8) | (global.rule.access<<6) | (global.rule.access<<4) | (global.rule.cb<<2) | 2 ;
        break;
    
      /* Extended smallpage entry type */
      case 'E':
        fields = (global.rule.tex<<6) | (global.rule.access<<4) | (global.rule.cb<<2) | 3;
        break;
      
      /* Fault entry type */
      case 'F':
        fields = 0x0;
        break;
      
      /* Invalid page entry type used */
      default:
        printf("Error 16: An invalid PAGE type was entered.\n");
        *error = 0;  //generated binary file will be incorrect
        return 0;
    }
  }
        
  /* create entries for this descriptor */
  i = global.rule.v_base ;
       
  while(i<global.rule.v_end)
  {
    /* Create large page table entries */
    if( global.rule.entrytype == 'L' )
    {
      writevalue = (global.rule.p_base & 0xFFFF0000) | fields ;
      
      /* Copy a single large page table entry 16 times */                   
      for( j=16; j > 0; j-- )
      {
        fwrite( &writevalue, 4, 1, global.l2_outfile );
        
        /* Print page table entry bit settings */
        if (global.rule.v6 || global.rule.v7)
          printEntry_L2v6(writevalue);
        else
          printEntry_L2v4(writevalue);
           
        global.table_entry_2++;
      }
      
      global.rule.p_base += 0x10000;
      i += 0x10000;
    }
    
    /* Create small, extended_small, and fault page table entries */
    else
    {
      writevalue = (global.rule.p_base & 0xFFFFF000) | fields ;
      fwrite( &writevalue, 4, 1, global.l2_outfile );
      
      /* Print page table entry bit settings */
      if (global.rule.v6 || global.rule.v7)
        printEntry_L2v6(writevalue);
      else
        printEntry_L2v4(writevalue); 
        
      global.rule.p_base += 0x1000;
      i += 0x1000;
                        
      global.table_entry_2++;
    }

  }  
  
  //clear set values
  global.rule.ro = 0;      
  global.rule.access = 0;   
  global.rule.p = 0; 
  global.rule.domain = 0; 
  global.rule.nglobal = 0;  
  global.rule.shared = 0;  
  global.rule.execn = 0; 
  global.rule.tex = 0;
  global.rule.cb = 0;
  global.rule.outer = 0;  
   
  return 0 ;
}


