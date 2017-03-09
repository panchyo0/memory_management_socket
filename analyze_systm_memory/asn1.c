#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include "memchunk.h"


#define MEM_RW 0
#define MEM_RO 1
#define MEM_NO 2

static sigjmp_buf env; 
void signalHandler(int signo)
{
  siglongjmp(env, 1);
}




int get_mem_layout (struct memregion *regions, unsigned int size){

int pagesize= getpagesize();	
uint1 *address = (uint1*)  0x00000000;
uint1 *prevAddress = (uint1*) 0x00000000;
uint1 *maxaddress = (uint1*) 0xFFFFFFFF;


(void) signal(SIGSEGV, signalHandler);

int signalVariable;

signalVariable = sigsetjmp(env,1);
int currentregion;
int previousregion=99;
int regiontotal=0;

struct memregion *memptr;
uint1 addrtest;

while (address < maxaddress && address >= prevAddress){\
    memptr= regions + regiontotal;
  
    (void) signal (SIGSEGV, signalHandler);
    signalVariable= sigsetjmp(env,1);
    if (signalVariable == 0 ){


	addrtest= *address;
        (void) signal(SIGSEGV, signalHandler);
  	signalVariable = sigsetjmp(env, 1);
        if (signalVariable == 0){
	   *address = addrtest;
           currentregion = MEM_RW;
        }
  	else{
	   currentregion = MEM_RO;

        }
     }
    else{
      currentregion = MEM_NO;

    }
    if (previousregion == currentregion){
       if (regiontotal <=size){
        address += pagesize;   

	}
    }
    else {
     regiontotal +=1;
     if (regiontotal <= size){
       (*memptr).from= (uint1 *)address;
       (*(memptr-1)).to = (uint1 *)address;
       (*memptr).mode = currentregion;

      }

    } 
    previousregion = currentregion;
    prevAddress = address; 
    address += pagesize;
}
return regiontotal;


}


/*int get_mem_diff (struct memregion *regions, unsigned int howmany,
		  struct memregion *thediff, unsigned int diffsize);
*/

