#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include <syslog.h>
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
	fd_set rfds;
	int retval;
	struct timeval tv;
	int fd;
	openlog("daemon", LOG_PID, LOG_DAEMON);
	if ((pid = fork())<0)
		exit(1);

	if((sid = setsid())<0)
		exit(1);

	if(chdir("/")<0){
		exit(1);
	}

	umask(0);

	while(1){
		FD_ZERO(&rfds);
		FD_SET(0,&rfds);

		tv.tv_sec = 2;
		tv.tv_usec = 0;

		retval = select(1,&rfds,NULL,NULL,&tv);
		if (retval == -1) {
			perror("select failed");
			break;
		} else if (retval == 0) {
			check = check_file();
			if (check == 0) {
				syslog(LOG_INFO, "File does not exist");
			} else {
				syslog(LOG_INFO, "File exists");
				break;
			}
		}

	}

	printf("daemon end\n");	
	return 0;
}
