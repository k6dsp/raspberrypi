/*********************************************************************
 *
 * checking.h
 *
 * Contains functions that print the contents of every page table 
 * entry to the specified output file
 *
 * functions:
 *  void printEntry_L1(unsigned int writevalue);
 *  void printEntry_L2v4(unsigned int writevalue);
 *  void printEntry_L2v6(unsigned int writevalue);
 *  void printMissingEntry_L1(unsigned int writevalue);
 *  void printMissingEntry_L2v4(unsigned int writevalue);
 *  void printMissingEntry_L2v6(unsigned int writevalue);
 *
 * Copyright Peter Maloy, CodeSprite August 2000    www.codesprite.com 
 *
 * Modified January, 2008 Elizabeth Norris, ARM Inc.
 *
 **********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "mmugen.h"

extern Global global;
extern tparsedata parsedata;

/*********************************************************************
 * printEntry_L1
 * 
 * Print the values of the different bits in the Level 1 page table
 * entries to the specified output file
 * 
 * inputs:
 *   unsigned int writevalue
 *     contains the page table entry
 * 
 * outputs:
 *   none
 * 
 **********************************************************************/
 
void printEntry_L1(unsigned int writevalue)
{
  /* print Section table entry */
  if(global.rule.entrytype == 'S')
  {
    fprintf(global.entry_outfile, "  entry: 0x%x \n", writevalue);
    fprintf(global.entry_outfile, " PA: 0x%x ",(writevalue & 0xFFF00000));
    /* only print out NS bit if using v6/v7 */
    if( global.rule.v6 == 1 || global.rule.v7 == 1)
    {
      fprintf(global.entry_outfile, "NS: %d ", ((writevalue & 0x80000) >> 19));
    }
    fprintf(global.entry_outfile, "[18]: %d ", ((writevalue & 0x40000) >> 18));
    fprintf(global.entry_outfile, "nG: %d ", ((writevalue & 0x20000) >> 17));
    fprintf(global.entry_outfile, "S: %d ",((writevalue & 0x10000) >> 16));
    fprintf(global.entry_outfile, "APX: %d ",((writevalue & 0x8000) >> 15));
    fprintf(global.entry_outfile, "TEX: %d ", ((writevalue & 0x7000) >> 12));
    fprintf(global.entry_outfile, "AP: %d ", ((writevalue & 0xC00) >> 10));
    fprintf(global.entry_outfile, "P: %d ", ((writevalue & 0x200) >> 9));
    fprintf(global.entry_outfile, "Domain: %d ", ((writevalue & 0x1E0) >> 5));
    fprintf(global.entry_outfile, "XN: %d ",((writevalue & 0x10) >> 4));
    fprintf(global.entry_outfile, "CB: 0x%x ",((writevalue & 0xC) >> 2));
    fprintf(global.entry_outfile, "ID: 0x%x \n", (writevalue & 0x3));
  }
        
  /* print Supersection table entry */
  else if(global.rule.entrytype == 'H')
  {  
    fprintf(global.entry_outfile, "  entry: 0x%x \n", writevalue);
    fprintf(global.entry_outfile, " PA: 0x%x ",(writevalue & 0xFF000000));
    /* only print out NS bit if using v6/v7 */
    if( global.rule.v6 == 1 || global.rule.v7 == 1)
    {
      fprintf(global.entry_outfile, "NS: %d ", ((writevalue & 0x80000) >> 19));
    }
    fprintf(global.entry_outfile, "[18]: %d ", ((writevalue & 0x40000) >> 18));
    fprintf(global.entry_outfile, "nG: %d ", ((writevalue & 0x20000) >> 17));
    fprintf(global.entry_outfile, "S: %d ",((writevalue & 0x10000) >> 16));
    fprintf(global.entry_outfile, "APX: %d ",((writevalue & 0x8000) >> 15));
    fprintf(global.entry_outfile, "TEX: %d ", ((writevalue & 0x7000) >> 12));
    fprintf(global.entry_outfile, "AP: %d ", ((writevalue & 0xC00) >> 10));
    fprintf(global.entry_outfile, "P: %d ", ((writevalue & 0x200) >> 9));
    fprintf(global.entry_outfile, "XN: %d ",((writevalue & 0x10) >> 4));
    fprintf(global.entry_outfile, "CB: 0x%x ",((writevalue & 0xC) >> 2));
    fprintf(global.entry_outfile, "ID: 0x%x \n", (writevalue & 0x3));
  }
    
  /* print Fault table entry */
  else if(global.rule.entrytype == 'F')
  { 
    fprintf(global.entry_outfile, "  entry: 0x%x \n", writevalue);
  }
      
  /* print Page table entry */    
  else 
  {
    fprintf(global.entry_outfile, "  entry: 0x%x \n", writevalue);
    fprintf(global.entry_outfile, " PA: 0x%x ", (writevalue & 0xFFFFFC00));
    fprintf(global.entry_outfile, "P: %d ", ((writevalue & 0x200) >> 9));
    fprintf(global.entry_outfile, "Domain: %d ", ((writevalue & 0x1E0) >> 5));
    /* only print out NS bit if using v6/v7 */
    if( global.rule.v6 == 1 || global.rule.v7 == 1)
    {
      fprintf(global.entry_outfile, "NS: %d ", ((writevalue & 0x80000) >> 19));
    }
    fprintf(global.entry_outfile, "ID: 0x%x \n", (writevalue & 0x3));
  }
}

/*********************************************************************
 * printEntry_L2v4
 * 
 * Print the values of the different bits in the v4/v5 Level 2 page table
 * entries to the specified output file
 * 
 * inputs:
 *   unsigned int writevalue
 *     contains the page table entry
 * 
 * outputs:
 *   none
 * 
 **********************************************************************/

void printEntry_L2v4(unsigned int writevalue)
{
  /* print v4/v5 Largepage table entry */
  if(global.rule.entrytype == 'L')
  {      
    fprintf(global.entry_outfile, "  entry: 0x%x \n", writevalue);
    fprintf(global.entry_outfile, " PA: 0x%x ", (writevalue & 0xFFFF0000));
    fprintf(global.entry_outfile, "TEX: %d ",(writevalue & 0x7000) >> 12);
    fprintf(global.entry_outfile, "AP3: %d ",(writevalue & 0xC00) >> 10);
    fprintf(global.entry_outfile, "AP2: %d ",(writevalue & 0x300) >> 8);
    fprintf(global.entry_outfile, "AP1: %d ",(writevalue & 0xC0) >> 6);
    fprintf(global.entry_outfile, "AP0: %d ",(writevalue & 0x30) >> 4);
    fprintf(global.entry_outfile, "CB: %x ",(writevalue & 0xC) >> 2);
    fprintf(global.entry_outfile, "ID: %d \n",writevalue & 0x3);
  }
      
  /* print v4/v5 Smallpage table entry */
  else if(global.rule.entrytype == 'S')
  { 
    fprintf(global.entry_outfile, "  entry: 0x%x \n", writevalue);
    fprintf(global.entry_outfile, " PA: 0x%x ", (writevalue & 0xFFFFF000));
    fprintf(global.entry_outfile, "AP3: %d ",(writevalue & 0xC00) >> 10);
    fprintf(global.entry_outfile, "AP2: %d ",(writevalue & 0x300) >> 8);
    fprintf(global.entry_outfile, "AP1: %d ",(writevalue & 0xC0) >> 6);
    fprintf(global.entry_outfile, "AP0: %d ",(writevalue & 0x30) >> 4);
    fprintf(global.entry_outfile, "CB: %x ",(writevalue & 0xC) >> 2);
    fprintf(global.entry_outfile, "ID: %d \n",writevalue & 0x3);
  }
    
  /* print v4/v5 Extended_Smallpage table entry */
  else if(global.rule.entrytype == 'E')
  {
    fprintf(global.entry_outfile, "  entry: 0x%x \n", writevalue);
    fprintf(global.entry_outfile, " PA: 0x%x ", (writevalue & 0xFFFFF000));
    fprintf(global.entry_outfile, "TEX: %d ",(writevalue & 0x1C0) >> 6);
    fprintf(global.entry_outfile, "AP: %d ",(writevalue & 0x30) >> 4);
    fprintf(global.entry_outfile, "CB: %x ",(writevalue & 0xC) >> 2);
    fprintf(global.entry_outfile, "ID: %d \n",writevalue & 0x3);
  }
    
  /* print v4/v5 Fault table entry */  
  else 
  {
    fprintf(global.entry_outfile, "  entry: 0x%x \n", writevalue);
  }
}

/*********************************************************************
 * printEntry_L2v6
 * 
 * Print the values of the different bits in the v6/v7 Level 2 page table
 * entries to the specified output file
 * 
 * inputs:
 *   unsigned int writevalue
 *     contains the page table entry
 * 
 * outputs:
 *   none
 * 
 **********************************************************************/

void printEntry_L2v6(unsigned int writevalue)
{
  /* print v6/v7 Largepage table entry */
  if(global.rule.entrytype == 'L')
  {
    fprintf(global.entry_outfile, "  entry: 0x%x \n", writevalue);
    fprintf(global.entry_outfile, " PA: 0x%x ",(writevalue & 0xFFFF0000));
    fprintf(global.entry_outfile, "XN: %d ", (writevalue & 0x8000) >> 15);
    fprintf(global.entry_outfile, "TEX: %d ",(writevalue & 0x7000) >> 12);
    fprintf(global.entry_outfile, "nG: %d ",(writevalue & 0x800) >> 11);
    fprintf(global.entry_outfile, "S: %d ", (writevalue & 0x400) >> 10);
    fprintf(global.entry_outfile, "APX: %d ",(writevalue & 0x200) >> 9);
    fprintf(global.entry_outfile, "AP: %d ",(writevalue & 0x30) >> 4);
    fprintf(global.entry_outfile, "CB: %x ",(writevalue & 0xC) >> 2);
    fprintf(global.entry_outfile, "ID: %d \n",writevalue & 0x3);
  }
    
  /* print v6/v7 Extended_Smallpage table entry */
  else if(global.rule.entrytype == 'E')
  {
    fprintf(global.entry_outfile, "  entry: 0x%x \n", writevalue);
    fprintf(global.entry_outfile, " PA: 0x%x ",(writevalue & 0xFFFFF000));
    fprintf(global.entry_outfile, "nG: %d ",(writevalue & 0x800) >> 11);
    fprintf(global.entry_outfile, "S: %d ", (writevalue & 0x400) >> 10);
    fprintf(global.entry_outfile, "APX: %d ",(writevalue & 0x200) >> 9);
    fprintf(global.entry_outfile, "TEX: %d ",(writevalue & 0x1C0) >> 6);
    fprintf(global.entry_outfile, "AP: %d ",(writevalue & 0x30) >> 4);
    fprintf(global.entry_outfile, "CB: %x ",(writevalue & 0xC) >> 2);
    fprintf(global.entry_outfile, "ID: %d ",writevalue & 0x2);
    fprintf(global.entry_outfile, "XN: %d \n", writevalue & 0x1);
  }
    
  /* print v6/v7 Fault table entry */
  else 
  {
    fprintf(global.entry_outfile, "  entry: 0x%x \n", writevalue);
  } 
}

/*********************************************************************
 * printMissingEntry_L1
 * 
 * print the values of the different bits of the missing Level 1 page 
 * table entries to the specified output file
 * 
 * inputs:
 *   unsigned int writevalue
 *     contains the page table entry
 * 
 * outputs:
 *   none
 * 
 **********************************************************************/

void printMissingEntry_L1(unsigned int writevalue)
{  
  /* print Section table entry */
  if(global.rule.d_entrytype == 'S')
  {
    fprintf(global.entry_outfile, "  entry: 0x%x \n", writevalue);
    fprintf(global.entry_outfile, " PA: 0x%x ",(writevalue & 0xFFF00000));
    /* only print out NS bit if using v6/v7 */
    if( global.rule.v6 == 1 || global.rule.v7 == 1 )
    {
      fprintf(global.entry_outfile, "NS: %d ", ((writevalue & 0x80000) >> 19));
    }
    fprintf(global.entry_outfile, "[18]: %d ", ((writevalue & 0x40000) >> 18));
    fprintf(global.entry_outfile, "nG: %d ", ((writevalue & 0x20000) >> 17));
    fprintf(global.entry_outfile, "S: %d ",((writevalue & 0x10000) >> 16));
    fprintf(global.entry_outfile, "APX: %d ",((writevalue & 0x8000) >> 15));
    fprintf(global.entry_outfile, "TEX: %d ", ((writevalue & 0x7000) >> 12));
    fprintf(global.entry_outfile, "AP: %d ", ((writevalue & 0xC00) >> 10));
    fprintf(global.entry_outfile, "P: %d ", ((writevalue & 0x200) >> 9));
    fprintf(global.entry_outfile, "Domain: %d ", ((writevalue & 0x1E0) >> 5));
    fprintf(global.entry_outfile, "XN: %d ",((writevalue & 0x10) >> 4));
    fprintf(global.entry_outfile, "CB: 0x%x ",((writevalue & 0xC) >> 2));
    fprintf(global.entry_outfile, "ID: 0x%x \n", (writevalue & 0x3));
  }
        
  /* print Supersection table entry */
  else if(global.rule.d_entrytype == 'H')
  {  
    fprintf(global.entry_outfile, "  entry: 0x%x \n", writevalue);
    fprintf(global.entry_outfile, " PA: 0x%x ",(writevalue & 0xFF000000));
    /* only print out NS bit if using v6/v7 */
    if( global.rule.v6 == 1 || global.rule.v7 == 1)
    {
      fprintf(global.entry_outfile, "NS: %d ", ((writevalue & 0x80000) >> 19));
    }
    fprintf(global.entry_outfile, "[18]: %d ", ((writevalue & 0x40000) >> 18));
    fprintf(global.entry_outfile, "nG: %d ", ((writevalue & 0x20000) >> 17));
    fprintf(global.entry_outfile, "S: %d ",((writevalue & 0x10000) >> 16));
    fprintf(global.entry_outfile, "APX: %d ",((writevalue & 0x8000) >> 15));
    fprintf(global.entry_outfile, "TEX: %d ", ((writevalue & 0x7000) >> 12));
    fprintf(global.entry_outfile, "AP: %d ", ((writevalue & 0xC00) >> 10));
    fprintf(global.entry_outfile, "P: %d ", ((writevalue & 0x200) >> 9));
    fprintf(global.entry_outfile, "XN: %d ",((writevalue & 0x10) >> 4));
    fprintf(global.entry_outfile, "CB: 0x%x ",((writevalue & 0xC) >> 2));
    fprintf(global.entry_outfile, "ID: 0x%x \n", (writevalue & 0x3));
  }
    
  /* print Fault table entry */
  else if(global.rule.d_entrytype == 'F')
  { 
    fprintf(global.entry_outfile, "  entry: 0x%x \n", writevalue);
  }
      
  /* print Page table entry */    
  else 
  {
    fprintf(global.entry_outfile, "  entry: 0x%x \n", writevalue);
    fprintf(global.entry_outfile, " PA: 0x%x ", (writevalue & 0xFFFFFC00));
    fprintf(global.entry_outfile, "P: %d ", ((writevalue & 0x200) >> 9));
    fprintf(global.entry_outfile, "Domain: %d ", ((writevalue & 0x1E0) >> 5));
    /* only print out NS bit if using v6/v7 */
    if( global.rule.v6 == 1 || global.rule.v7 == 1)
    {
      fprintf(global.entry_outfile, "NS: %d ", ((writevalue & 0x80000) >> 19));
    }
    fprintf(global.entry_outfile, "ID: 0x%x \n", (writevalue & 0x3));
  }
}

/*********************************************************************
 * printMissingEntry_L2v4
 * 
 * Print the values of the different bits of the missing v4/v5 Level 2
 * page table entries to the specified output file
 * 
 * inputs:
 *   unsigned int writevalue
 *     contains the page table entry
 * 
 * outputs:
 *   none
 * 
 **********************************************************************/

void printMissingEntry_L2v4(unsigned int writevalue)
{
  /* print v4/v5 Largepage table entry */
  if(global.rule.d_entrytype == 'L')
  {      
    fprintf(global.entry_outfile, "  entry: 0x%x \n", writevalue);
    fprintf(global.entry_outfile, " PA: 0x%x ", (writevalue & 0xFFFF0000));
    fprintf(global.entry_outfile, "TEX: %d ",(writevalue & 0x7000) >> 12);
    fprintf(global.entry_outfile, "AP3: %d ",(writevalue & 0xC00) >> 10);
    fprintf(global.entry_outfile, "AP2: %d ",(writevalue & 0x300) >> 8);
    fprintf(global.entry_outfile, "AP1: %d ",(writevalue & 0xC0) >> 6);
    fprintf(global.entry_outfile, "AP0: %d ",(writevalue & 0x30) >> 4);
    fprintf(global.entry_outfile, "CB: %x ",(writevalue & 0xC) >> 2);
    fprintf(global.entry_outfile, "ID: %d \n",writevalue & 0x3);
  }
      
  /* print v4/v5 Smallpage table entry */
  else if(global.rule.d_entrytype == 'S')
  { 
    fprintf(global.entry_outfile, "  entry: 0x%x \n", writevalue);
    fprintf(global.entry_outfile, " PA: 0x%x ", (writevalue & 0xFFFFF000));
    fprintf(global.entry_outfile, "AP3: %d ",(writevalue & 0xC00) >> 10);
    fprintf(global.entry_outfile, "AP2: %d ",(writevalue & 0x300) >> 8);
    fprintf(global.entry_outfile, "AP1: %d ",(writevalue & 0xC0) >> 6);
    fprintf(global.entry_outfile, "AP0: %d ",(writevalue & 0x30) >> 4);
    fprintf(global.entry_outfile, "CB: %x ",(writevalue & 0xC) >> 2);
    fprintf(global.entry_outfile, "ID: %d \n",writevalue & 0x3);
  }
    
  /* print v4/v5 Extended_Smallpage table entry */
  else if(global.rule.d_entrytype == 'E')
  {
    fprintf(global.entry_outfile, "  entry: 0x%x \n", writevalue);
    fprintf(global.entry_outfile, " PA: 0x%x ", (writevalue & 0xFFFFF000));
    fprintf(global.entry_outfile, "TEX: %d ",(writevalue & 0x1C0) >> 6);
    fprintf(global.entry_outfile, "AP: %d ",(writevalue & 0x30) >> 4);
    fprintf(global.entry_outfile, "CB: %x ",(writevalue & 0xC) >> 2);
    fprintf(global.entry_outfile, "ID: %d \n",writevalue & 0x3);
  }
    
  /* print v4/v5 Fault table entry */  
  else 
  {
    fprintf(global.entry_outfile, "  entry: 0x%x \n", writevalue);
  }
}

/*********************************************************************
 * printMissingEntry_L2v6
 * 
 * Print the values of the different bits of the missing v6/v7 Level 2
 * page table entries to the specified output file
 * 
 * inputs:
 *   unsigned int writevalue
 *     contains the page table entry
 * 
 * outputs:
 *   none
 * 
 **********************************************************************/

void printMissingEntry_L2v6(unsigned int writevalue)
{
  /* print v6/v7 Largepage table entry */
  if(global.rule.d_entrytype == 'L')
  {
    fprintf(global.entry_outfile, "  entry: 0x%x \n", writevalue);
    fprintf(global.entry_outfile, " PA: 0x%x ",(writevalue & 0xFFFF0000));
    fprintf(global.entry_outfile, "XN: %d ", (writevalue & 0x8000) >> 15);
    fprintf(global.entry_outfile, "TEX: %d ",(writevalue & 0x7000) >> 12);
    fprintf(global.entry_outfile, "nG: %d ",(writevalue & 0x800) >> 11);
    fprintf(global.entry_outfile, "S: %d ", (writevalue & 0x400) >> 10);
    fprintf(global.entry_outfile, "APX: %d ",(writevalue & 0x200) >> 9);
    fprintf(global.entry_outfile, "AP: %d ",(writevalue & 0x30) >> 4);
    fprintf(global.entry_outfile, "CB: %x ",(writevalue & 0xC) >> 2);
    fprintf(global.entry_outfile, "ID: %d \n",writevalue & 0x3);
  }
    
  /* print v6/v7 Extended_Smallpage Table entry */
  else if(global.rule.d_entrytype == 'E')
  {
    fprintf(global.entry_outfile, "  entry: 0x%x \n", writevalue);
    fprintf(global.entry_outfile, " PA: 0x%x ",(writevalue & 0xFFFFF000));
    fprintf(global.entry_outfile, "nG: %d ",(writevalue & 0x800) >> 11);
    fprintf(global.entry_outfile, "S: %d ", (writevalue & 0x400) >> 10);
    fprintf(global.entry_outfile, "APX: %d ",(writevalue & 0x200) >> 9);
    fprintf(global.entry_outfile, "TEX: %d ",(writevalue & 0x1C0) >> 6);
    fprintf(global.entry_outfile, "AP: %d ",(writevalue & 0x30) >> 4);
    fprintf(global.entry_outfile, "CB: %x ",(writevalue & 0xC) >> 2);
    fprintf(global.entry_outfile, "ID: %d ",writevalue & 0x2);
    fprintf(global.entry_outfile, "XN: %d \n", writevalue & 0x1);
  }
    
  /* print v6/v7 Fault Table entry */
  else 
  {
    fprintf(global.entry_outfile, "  entry: 0x%x \n", writevalue);
  } 
}


