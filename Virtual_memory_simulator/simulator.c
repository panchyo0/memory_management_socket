#include "simulator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void init(int psize, int winsize){


  PAGE = MAX_MEMORY / psize;
  WINDOWS = MAX_MEMORY /winsize;
  //printf("Page %d Window %d\n",PAGE,WINDOWS);
  history =(int* )(malloc(sizeof(int)*WINDOWS)); 
      page_array = (int*)malloc(sizeof(int) * PAGE);
  /*hashItem* hashArray= (hashItem *)malloc(PAGE * sizeof(hashItem));*/	

}
void put(unsigned address, int value){
  
 int page_index;
 int window;
 page_index= address/page_size;
 window = count2/ window_size;

 if (page_array[page_index]==0){
    page_array[page_index]=1;
    count1++;
 }

 if (count2%window_size == (window_size-1)){
    history[window]=count1;
    memset(page_array, 0,PAGE*sizeof(int));
    count1=0;
 }
 count2++;

 hashItem* item= search(hashArray[hashCode(address)],address);
 if (item==NULL){
  hashItem* newItem=newhashItem(address,value);
  hashArray[hashCode(address)]=insert(hashArray[hashCode(address)],newItem);
 } 
 else{
    item->data = value;
 }

}



int get(unsigned int address){
 
 int page_index;
 int window;
 page_index= address/page_size;
 window = count2/ window_size;
 if (page_array[page_index]==0){
    page_array[page_index]=1;
    count1++;
 }
 if (count2%window_size == (window_size-1)){
    history[window]=count1;
    memset(page_array, 0,PAGE*sizeof(int));
    count1=0;
 }
 count2++; 
  
 hashItem* item= search(hashArray[hashCode(address)],address);

 return item->data;
 
}
void free_memory(){

    free( *hashArray);
    free(page_array);
    free(history);

}
void done(){


    float total_count=0;
    //printf("count2 %d, WINDOWS %d\n",count2,WINDOWS);
    float windows_used = count2 / window_size;
   

    int i;
    //printf("windows_used, %f\n",windows_used );
    for (i = 0; i < (int)windows_used; i++){
       
        //printf("Window Interval %i | %i to %i| Working Set Size: %i\n", i, window_size*i, window_size*(i+1)-1, history[i]);


        total_count += history[i];
    }
    
   
    float avg = total_count / windows_used;
    printf("---------------------------------------------------\n");
    printf("Average working set size: %f\n", avg);
    printf("working set requirements is %f\n",total_count);
    printf("---------------------------------------------------\n");
   

    free_memory();


}

int main(int argc, char *argv[]) {
    
    if (argc != 3) {
        fprintf(stderr, "Usage:./filename pagesize windowsize\n");
        exit(-1);
    }
    
    page_size = atoi(argv[1]);
    window_size = atoi(argv[2]);
//    int i,j;
//    int parry[]={64, 128, 256, 512};
//    int warry[]={128, 256, 512, 1024, 2048, 4096, 8192, 16384};
// 
//    for (i=0; i<sizeof(parry); i++) {
//        page_size=parry[i];
//        for (j=0;j<sizeof(warry);j++){
//        
//            window_size=warry[j];
//            //init(page_size, window_size);
//            printf("-----------------------------------------------------------\n");
//            printf("page size is %d window size is %d\n",page_size,window_size);
//            process();
//        }
//    }

    init(page_size, window_size);
   
    process();
    return 0;
}

