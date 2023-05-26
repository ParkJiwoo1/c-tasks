#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define row 5
#define col 5
void sum(int arr[row][col]){
	int i,j;
	int sum[3]={0,};         //save 3 different sum value to sum array
	for(i=0;i<row;i++){
		for(j=0;j<col;j++){
			if(i==j){       
				sum[0]+=arr[i][j];   //diagonal
			}else if(i<j){
				sum[1]+=arr[i][j];   //upper diagonal
			}else{
				sum[2]+=arr[i][j];   //under diagonal
			}
		}
	}

	for(i=0;i<3;i++){
		printf("sum%d = %d\n",i+1,sum[i]);
	}
	return;
}

int main(){
	int i;
	srand(time(NULL));
	int arr[row][col]={0,};   //initialize array to 0

	for(i=0;i<row;i++){
		for(int j=0;j<col;j++){
			arr[i][j]=rand()%100+1;    //random number between 1 and 100
			printf("%d  ", arr[i][j]);
		}
		printf("\n");
	}
	sum(arr);
	return 0;
}

