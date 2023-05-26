#include<stdio.h>
#include<string.h>
#include<unistd.h>

int displayboard(char str[], int len){
	int i,j;

	for(i=0;i<len;i++){
		for(j=i;j<i+len;j++){
			printf("%c", str[j%len]);
		}
		printf("\n");
		sleep(1);
	}
}

int main(){
	char str[30]="Happy Birthday!!!!! ";
	int length = strlen(str);

	displayboard(str,length);
	return 0;
}
