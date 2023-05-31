#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/select.h>

#define PORT 8081
#define BUFFER_SIZE 100
#define LISTEN_QUEUE_SIZE 5
#define TIMEOUT 5

int main(){
	struct sockaddr_in cli_sock, serv_sock, peer_sock;
	struct timeval tv;
	int listen_fd, connect_fd, fd_max, fd_num, str_len, cli_port;
	fd_set reads, cp_reads; //max 1024 file descriptor
	socklen_t adr_sz, sock_len;
	char buff[BUFFER_SIZE],*cli_addr;

	memset(&serv_sock, 0, sizeof(serv_sock));

	serv_sock.sin_family = AF_INET;
	serv_sock.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_sock.sin_port = htons(PORT);

	adr_sz = sizeof(cli_sock);

	listen_fd = socket(AF_INET, SOCK_STREAM,0);

	if(bind(listen_fd, (struct sockaddr *)&serv_sock,sizeof(serv_sock))==-1){
		perror("bind error");
		return -1;
	}

	if(listen(listen_fd, LISTEN_QUEUE_SIZE)==-1){
		perror("listen error");
		return -1;
	}

	printf("waiting for clients....\n");

	FD_ZERO(&reads);
	FD_SET(listen_fd,&reads);
	fd_max = listen_fd;//0,1,2 is already set to stdin,stdout,stderr

	while(1){
		cp_reads = reads;

		tv.tv_sec = TIMEOUT;
		tv.tv_usec = 0;

		if ((fd_num = select(fd_max+1,&cp_reads,0,0,&tv))==-1){//return number of DF,detect changes of DF
			perror("select error");
		}

		if (fd_num == 0){
			printf("no message for 5 seconds\n");
		}

		for(int i=0;i<fd_max+1;i++){
			if(FD_ISSET(i,&cp_reads)){//identify index i is set to 1
				if(i==listen_fd){
					adr_sz = sizeof(cli_sock);
					connect_fd = accept(listen_fd,(struct sockaddr*)&cli_sock,&adr_sz);

					sock_len = sizeof(peer_sock);

					FD_SET(connect_fd,&reads);
					if(fd_max<connect_fd)
						fd_max = connect_fd;

					getpeername(connect_fd, (struct sockaddr*)&peer_sock, &sock_len);
					cli_addr = inet_ntoa(peer_sock.sin_addr);
					cli_port = ntohs(peer_sock.sin_port);

					printf("connected client %s:%d, FD: %d \n",cli_addr,cli_port,connect_fd);
				}else{
					getpeername(i, (struct sockaddr*)&peer_sock, &sock_len);
					cli_addr = inet_ntoa(peer_sock.sin_addr);
					cli_port = ntohs(peer_sock.sin_port);

					str_len = recv(i,buff,BUFFER_SIZE,0);

					if(str_len<1){
						printf("close client %s FD:%d \n",cli_addr,i);
						FD_CLR(i,&reads);
						close(i);
					}else if(str_len>=1){
						printf("received message from %s:%d = ",cli_addr,cli_port);
						buff[str_len]='\0';
						fputs(buff,stdout);
						send(i,buff,str_len,0);
					}
				}
			}
		}
	}
	close(listen_fd);
	return 0;
}
