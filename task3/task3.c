#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(){
	FILE *fp;
	char buf[1024];
	char word[100];
	int cnt=0;
	int line=0;
	printf("word to search : ");
	fgets(word,sizeof(word),stdin); //word or sentence that want to search
	word[strcspn(word,"\n")]='\0';  //change '\n' to '\0' because of string compare

	if(fp = fopen("./example.txt","r")){ //open file for read
		if(fp==NULL){
			printf("cannot open file\n");
			return -1;
		}
		memset(buf,0,sizeof(buf));

		while(!feof(fp)){   //end of file
			line+=1;
			fgets(buf,sizeof(buf),fp); //read line by line

			if(strstr(buf,word)!=NULL){ //if found word in buf
				cnt+=1;
			}
		}
		if(cnt==0) printf("cannot find word or sentence\n");
		fclose(fp);
	}
	if(cnt>0)
		printf("number of %s is %d\n",word,cnt);

	return 0;
}
