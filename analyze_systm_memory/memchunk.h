
#ifndef MEMCHUNK_H_
#define MEMCHUNK_H_

/* Library Dependencies */
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <setjmp.h>
#include <sys/user.h>

/* Global type definition */
typedef unsigned char uint1; 
void signalHandler(int);
/* memchunk struct used to store memory chunk attributes */
/*struct memchunk 
{
  void *start;
  void *end;
  unsigned long length;
  int RW;
};*/
struct memregion {
	void *from;
	void *to;
	unsigned char mode;
};

int get_mem_layout (struct memregion *regions, unsigned int size);


/* memchunk.c function prototypes*/
/*int get_mem_layout (struct memregion * regions, unsigned int size);
int getReadability(uint1 *);
int readPrivs(uint1 *);
int writePrivs(uint1 *);
void signalHandler(int);
*/
#endif /* MEMCHUNK_H_ */
