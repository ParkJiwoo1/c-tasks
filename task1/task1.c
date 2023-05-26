#include<stdio.h>

void decimal(int num){
	int i;
	int bit;
	int comp=-1*num;

/*	if(num<0){
		num=-1*num;
		num=~num;
		num+=1;
	}
*/
	for(i=31;i>=0;i--){
		bit = (num>>i)&1;
		printf("%d",bit);
	}
	printf("(2)\n");
}

int main(){
	int number,binary,sum=0,i=1;
	printf("input integer in deciaml : ");
	scanf("%d",&number);
	printf("%d(10) = ",number);
	decimal(number);
	return 0;
}
