#include <stdio.h>
#include <stdlib.h>
#define SIZE 9999999
#define MAX_MEMORY 33554432


int *page_array;
int *history;
int count1=0;
int count2=0;
int page_size;
int window_size;
int PAGE;
int WINDOWS;

void process();
void init(int psize, int winsize);
void put(unsigned int address, int value);
int get(unsigned int address);
void free_memory();
void done();

typedef struct hashItem {
   int data;   
   unsigned int key;
   struct hashItem* next;
}hashItem;

hashItem* hashArray[SIZE];

int hashCode(unsigned int key) {
   return key % SIZE;
}
hashItem* newhashItem(unsigned int address, int value){
    hashItem *newItem= malloc(sizeof(hashItem));
    newItem->key= address;
    newItem->data= value;
    newItem->next=NULL;
    return newItem;
}
struct hashItem *search(hashItem* item, unsigned int address) {
   
   
   while(item!= NULL) {
	if(item->key == address)
         return item; 
    item=item->next;
   }        
	
   return NULL;        
}
struct hashItem* insert(hashItem* item,hashItem * newItem) {

   newItem->next = item;
   return newItem;
 
}

