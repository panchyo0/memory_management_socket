#include "memchunk.h"
/* Global variable for sigjmp buffer */
static sigjmp_buf env; 

int readPrivs(uint1 * address)
{
  uint1 charTester;
  int signalVariable;
  
  /* Override default signal behavior for segmentation faults corresponding
   * to SIGSEGV signals to behavior assigned by subroutine signalHandler */
  (void) signal(SIGSEGV, signalHandler);
  
  signalVariable = sigsetjmp(env, 1);
  if (signalVariable == 0)
  {
    /* Test address for read access by assigning charTester to address */
    charTester = *address;
    /* Address was readable, so return 1 to indicate read-access memory */
    return 1;
  }
  /* Address is not readable, so return -1 to indicate inaccessible memory */
  else
  {
    return -1;
  }
}


int writePrivs(uint1 * address)
{
  uint1 charTester;
  int signalVariable; 
  
  /* Backup the old value of address- works fine since it has read privileges */
  charTester = *address;
  
  /* Override default signal behavior for segmentation faults corresponding
   * to SIGSEGV signals to behavior assigned by subroutine signalHandler */
  (void) signal(SIGSEGV, signalHandler);
  
  signalVariable = sigsetjmp(env, 1);
  /* Test address for write access by assigning adress back to charTester */
  if (signalVariable == 0)
  {
    *address = charTester;
    /* Address was both readable and writable, return 1 to indicate RW memory */
    return 1;
  }
  /* Address is not writable, so return 0 to indicate read-only memory */
  else
  {
    return 0;
  }
}


int getReadability(uint1 * address) 
{
  int isReadable;
  /* Override default signal behavior for segmentation faults corresponding
   * to SIGSEGV signals to behavior assigned by subroutine signalHandler */
  (void) signal(SIGSEGV, signalHandler);
  
  isReadable = readPrivs(address);
  if (isReadable == -1)
  {
    return -1;
  }
  return writePrivs(address);
}


int get_mem_layout (struct memchunk *chunk_list, int size)
{ 
  /* Get the page size of the system since each page in logical memory
   * has the same accessibility level for each byte in each corresponding
   * page in logical memory.
   */
  int pageSize = getpagesize();
  /* address gets assigned to a pointer to unsigned character starting at 0 */
  uint1 *address = (uint1*) 0x0000000;
  uint1 *prevAddress = (uint1*) 0x00000000;
  uint1 *finalMaxAddress = (uint1 *) 0xFFFFFFFF;
   
  int currentAccessibilityFlag;
  int oldAccessibilityFlag = 99;
  int chunkTotal = 0;
  
  struct memchunk *pointerArithmetic;
  
  while (address < finalMaxAddress && address >= prevAddress)
  {
    
    /* Get the accessibility level for the current address by calling
     * the subroutine getReadability. */ 
    currentAccessibilityFlag = getReadability(address);

    /* Assign the address of pointer to memchunk called pointerArithmetic  
     * to the starting address of chunk_list + the current number of chunks.
     * This address is the current memchunk struct that is being assigned
     * values to. */    
    pointerArithmetic = chunk_list + chunkTotal;
    
    /* If the next address has the same accessibility as the previous address,
     * then they are part of the same chunk in memory */
    if (oldAccessibilityFlag == currentAccessibilityFlag)
    {
  
      /* If we are currently looking at a memchunk that is within the confines
       * of the memory allocated to chunk_list, then increase the corresponding
       * memchunk struct's byte length by a full page size in logical memory. */
      if (chunkTotal <= size)
      {
        /* This is the same thing as writing chunk_list[chunkTotal-1].length */
        (*(pointerArithmetic-1)).length += pageSize;
      }
    }
    /* If the next address does not have the same accessibility as the previous
     * address,  then they are not part of the same chunk in memory, so we need
     * to allocate a new chunk to chunk_list if we have the size to do so. */ 
    else
    {
      /* We have encountered a new chunk, so increment the total regardless of
       * having enough size to store the details of this chunk in chunk_list */
      chunkTotal += 1;
      
      if (chunkTotal <= size)
      {
        /* Assign the starting length of the chunk to the size of a page
         * in logical memory since each byte in a page shares the same
         * accessibility level as each other. */ 
        (*pointerArithmetic).length = pageSize;
        /* Assign the starting address of the chunk to start */
	(*(pointerArithmetic-1)).end  = (uint1 *)address-1;
        (*pointerArithmetic).start = (uint1 *)address;
        /* Assign the accessibility level to RW */
        (*pointerArithmetic).RW = currentAccessibilityFlag;
      }
    }
    
    oldAccessibilityFlag = currentAccessibilityFlag;
    prevAddress = address; 
    /* Increment the current address by page size since each address in
     * the current page size has already had its accessibility level tested
     * Because each address shares the same accessibility as the starting 
     * address in a page of logical memory. */
    address += pageSize;
  }
  
  return chunkTotal;
}


void signalHandler(int signo)
{
  siglongjmp(env, 1);
}


