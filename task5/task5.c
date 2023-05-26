#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define ROW 5
#define COL 20

void sort(char (*arr)[COL]){
	int i,j;

	for(i=0;i<ROW-1;i++){ //bubble
		for(j=0;j<ROW-1-i;j++){
			if(strcmp(arr[j],arr[j+1])>0){
				char temp_str[COL];
				strcpy(temp_str,arr[j]);
				strcpy(arr[j],arr[j+1]);
				strcpy(arr[j+1],temp_str);
			}
		}
	}

	for(i=0;i<ROW;i++){
		printf("%s\n",arr[i]);
	}
}

int main(){
	char arr[ROW][COL];
	int i;
	printf("enter 5 message to save in array\n");
	for(i=0;i<ROW;i++){
		printf("message %d : ",i+1);
		fgets(arr[i],COL,stdin);
		arr[i][strcspn(arr[i],"\n")]='\0';
	}

	sort(arr);
	return 0;
}
