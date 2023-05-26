#include<stdio.h>
#include<stdlib.h>

void sortArray(int arr[],int n){//insertion sort
	int i,j,key;
	for(i=1;i<n;i++){
		key = arr[i];
		j=i-1;

		while(j>=0 &&arr[j]<key){
			arr[j+1]=arr[j];
			j-=1;
		}
		arr[j+1]=key;
	}
}

int main(int argc, char *argv[]){
	int size = argc-1, i;
	if(size<10){
		printf("put more than 10 number\n");
		return 1;
	}

	int numbers[size];
	printf("original array\n");
	for(i=0;i<size;i++){
		numbers[i]=atoi(argv[i+1]);
		printf("%d ",numbers[i]);
	}
	printf("\n");

	sortArray(numbers,size);

	printf("after sorting\n");
	for(i=0;i<size;i++){
		printf("%d ",numbers[i]);
	}
	printf("\n");
	
	return 0;
}
