#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>

int check_file(){
	struct stat file;
	if(stat("/tmp/test.txt", &file)==0){
		return 1;
	}else{
		return 0;
	}
}

int main(){
	int check;
	pid_t pid, sid;

	if ((pid = fork())<0)
		exit(1);

	if((sid = setsid())<0)
		exit(1);

	if(chdir("/")<0){
		exit(1);
	}

	umask(0);

	while(1){
		check = check_file();
		if(check==0){
		sleep(1);
		printf("running...\n");
		}else{
			break;
		}
	}

	printf("daemon end\n");	
	return 0;
}
