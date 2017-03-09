#include <stdio.h>
#include <stdlib.h>

#define N 200000

int partition(int left, int end);
void quicksort(int left, int end);
void swap(int left, int right);
void process(){
int i;
	for(i = 0; i<N; i++){
		unsigned ran=(unsigned int)lrand48();
		put(i, ran);
	}

	quicksort(0, N-1);
       done();

}

void quicksort(int left, int end){
	if(end > left){
		int pivot = partition(left, end);
		quicksort(left, pivot-1);
		quicksort(pivot+1, end);
	}
}


int partition(int left, int end){
	unsigned int pivotIndex = (left+end)/2;
	unsigned int pivot = get(pivotIndex);

	swap(end, pivotIndex);

	int endIndex = end;
	end = end -1;

	while(left<=end){
		if(get(left)>=pivot && get(end)<pivot){
			swap(left, end);
		}
		if(get(end)>=pivot){
			end--;		
		}
		if(get(left)<pivot){
			left++;
		}
	}
	swap(left, endIndex);
	return left;
}

void swap(int left, int right){
	unsigned int temp = get(left);
	put(left, get(right));
	put(right, temp);
}
