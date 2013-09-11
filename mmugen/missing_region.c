/*********************************************************************
 *
 * missing_region.c
 *
 * Contains functions that generate and write the missing entries in the
 * Level 1 and Level 2 page tables to the specified binary file
 * 
 * functions:
 *  int WriteMissingRegionL1(int* error);
 *  int WriteMissingRegionL2(int* error);
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
 * WriteMissingRegionL1
 * 
 * create and write the missing Level 1 table entries to the specified
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
 
int WriteMissingRegionL1(int* error)
{
  unsigned int fields;
  unsigned int writevalue;
  unsigned int i, j;
  
  /* checking to see if virtual base and end addresses of the missing region is divisible by the entry type */
  if (global.rule.d_entrytype == 'H') //supersection check
  {
    if(((global.rule.v_end_saved + 1) % 0x1000000) != 0 || (global.rule.v_base % 0x1000000) != 0) //defined base or end address is not divisible by 16MB
    {
      printf("Error 17: Missing address range cannot be divided into 16MB sections.\n");
      *error = 0;  //generated binary file will be incorrect
      return 0;
    }
  }
  else //section and fault checking
  {
    if(((global.rule.v_end_saved + 1) % 0x100000) != 0 || (global.rule.v_base % 0x100000) != 0) //defined base or end address is not divisible by 1MB
    {
      printf("Error 18: Missing address range cannot be divided into 1MB sections.\n");
      *error = 0;  //generated binary file will be incorrect
      return 0;
    }
  }
  
  /* set and write page table entry based on default rule file entry type (Section, Supersection, or Fault) */
  switch (global.rule.d_entrytype)
  {      
    /* Section entry type */
    case 'S': 
      fields =  (global.rule.d_n_s<<19) | (global.rule.d_tex<<12) | (global.rule.d_access<<10) | (global.rule.d_p<<9) | (global.rule.d_domain<<5) | (global.rule.d_cb<<2) | 0x2;
                        
      if(global.rule.v6 == 1 || global.rule.v7 == 1) //using v6 or v7 page tables
      {
        fields |= (global.rule.d_nglobal<<17) | (global.rule.d_shared<<16) | (global.rule.d_ro<<15) | (global.rule.d_execn<<4);
      }
                        
      /* generate all the SECTION entries for this descriptor */
      for (i = (global.rule.v_end_saved + 1); i<global.rule.v_base;)
      {
        writevalue = (global.rule.p_base & 0xFFF00000) | fields;
        fwrite( &writevalue, 4, 1, global.outfile );
        
        /* Print page table entry bit settings */
        printMissingEntry_L1(writevalue);
        
        global.table_entry++ ;
        global.rule.p_base += 0x100000 ; //1MB

        /* check for memory wrap-round */
        i+=0x100000;
        if (i==0x00000000) break;
      }
      break; 
    
    /* Supersection entry type */
    case 'H': 
                                
      fields = (global.rule.d_n_s<<19) | 0x40000 | (global.rule.d_tex<<12) | (global.rule.d_access<<10) | (global.rule.d_p<<9) | (global.rule.d_cb<<2) | 0x2;
                        
      if(global.rule.v6 == 1 || global.rule.v7 == 1) //using v6 or v7 page tables
      {
        fields |= (global.rule.d_nglobal<<17) | (global.rule.d_shared<<16) | (global.rule.d_ro<<15) | (global.rule.d_execn<<4);
      }
                        
      /* generate all the SUPERSECTION entries for this descriptor */
      for (i = (global.rule.v_end_saved + 1); i<global.rule.v_base;)
      {
        for (j=16; j > 0; j--) //create 16 duplicate entries (1 entry = 1 MB)
        {
          writevalue = (global.rule.p_base & 0xFF000000) | fields ;
          fwrite( &writevalue, 4, 1, global.outfile );
          
          /* Print page table entry bit settings */
          printMissingEntry_L1(writevalue);
          
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
      for (i = (global.rule.v_end_saved + 1); i<global.rule.v_base;)
      {
        writevalue = 0;
        fwrite( &writevalue, 4, 1, global.outfile );
        
        /* Print page table entry bit settings */
        //printMissingEntry_L1(writevalue);
        
        global.table_entry++ ;
            
        /* check for memory wrap-round */
        i+=0x100000;
        if (i==0x00000000) break;
      }
      break;
    
    /* Invalid entry type used */    
    default: 
      printf("Error 13: An invalid entry type was entered.\n");
      *error = 0;  //generated binary file will be incorrect
      return 0;
  }
  
  return 1;
}

/*********************************************************************
 * WriteMissingRegionL2
 * 
 * create and write the missing Level 2 table entries to the specified
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

int WriteMissingRegionL2(int* error)
{
  unsigned int fields ;
  unsigned int writevalue ;
  unsigned int i, j;
  
  /* checking to see if virtual base and end addresses are divisible by the entry type */
  if (global.rule.d_entrytype == 'L') //large page check
  {
    if(((global.rule.v_end_saved + 1) % 0x10000) != 0 || (global.rule.v_base % 0x10000) != 0) //defined base or end address is not divisible by 64KB
    {
      printf("Error 19: Missing address range cannot be divided into 64KB sections.\n");
      *error = 0;  //generated binary file will be incorrect
      return 0;
    }
  } 
  else //small and fault page check
  {
    if(((global.rule.v_end_saved + 1) % 0x1000) != 0 || (global.rule.v_base % 0x1000) != 0) //defined base or end address is not divisible by 4KB
    {
      printf("Error 20: Missing address range cannot be divided into 4KB sections.\n");
      *error = 0;  //generated binary file will be incorrect
      return 0;
    }
  }
  
  /* Using v6/v7 page table format */      
  if(global.rule.v6 || global.rule.v7) 
  {
    /* set and write page table entry based on default rule file entry type (Largepage, Extended smallpage, or Fault) */
    switch (global.rule.d_entrytype)
    {
      /* Largepage entry type */
      case 'L':
        fields = (global.rule.d_execn<<15) | (global.rule.d_tex<<12) | (global.rule.d_nglobal<<11) | (global.rule.d_shared<<10) | (global.rule.d_ro<<9) | (global.rule.d_access<<4) | (global.rule.d_cb<<2) | 1;
        break;
        
      /* Extended smallpage entry type */
      case 'E':
        fields = (global.rule.d_nglobal<<11) | (global.rule.d_shared<<10) | (global.rule.d_ro<<9) | (global.rule.d_tex<<6) | (global.rule.d_access<<4) | (global.rule.d_cb<<2) | 2 | global.rule.d_execn;
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
    /* set and write page table entry based on default rule file entry type (Largepage, Smalllpage, Extended smallpage, or Fault) */
    switch (global.rule.d_entrytype)
    {
      /* Largepage entry type */
      case  'L':
        fields = (global.rule.d_tex<<12) | (global.rule.d_access<<10) | (global.rule.d_access<<8) | (global.rule.d_access<<6) | (global.rule.d_access<<4) | (global.rule.d_cb<<2) | 1;
        break;  
      
      /* Smallpage entry type */
      case 'S':
        fields = (global.rule.d_access<<10) | (global.rule.d_access<<8) | (global.rule.d_access<<6) | (global.rule.d_access<<4) | (global.rule.d_cb<<2) | 2 ;
        break;
      
      /* Extended smallpage entry type */
      case 'E':
        fields = (global.rule.d_tex<<6) | (global.rule.d_access<<4) | (global.rule.d_cb<<2) | 3;
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
  i = global.rule.v_end_saved + 1;
       
  while(i<global.rule.v_base)
  {
    /* Create large page table entries */
    if( global.rule.d_entrytype == 'L' )
    {
      writevalue = (global.rule.p_base & 0xFFFF0000) | fields ;
      
      /* Copy a single large page table entry 16 times */                   
      for( j=16; j > 0; j-- )
      {
        fwrite( &writevalue, 4, 1, global.l2_outfile );
        
        /* Print page table entry bit settings */
        if (global.rule.v6 || global.rule.v7)
          printMissingEntry_L2v6(writevalue);
        else
          printMissingEntry_L2v4(writevalue);
        
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
        printMissingEntry_L2v6(writevalue);
      else
        printMissingEntry_L2v4(writevalue);
      
      global.rule.p_base += 0x1000;
      i += 0x1000;
                        
      global.table_entry_2++;
    }
  }
  
  return 1;
}


