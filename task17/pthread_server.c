#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <arpa/inet.h>
#include "linklist.h"

#define PORT 8081
#define BUFFER_SIZE 100
#define BUFF_SIZE 20
#define LISTEN_QUEUE_SIZE 5

struct List* list;

void *handle_client(void *arg)
{
	int connectFD = *((int *)arg);
	struct sockaddr_in peer_sock;
	socklen_t sock_len;
	char readBuff[BUFFER_SIZE],sendBuff[BUFFER_SIZE], listBuff[BUFFER_SIZE];
	ssize_t message_len;

	sock_len = sizeof(peer_sock);
	getpeername(connectFD, (struct sockaddr*)&peer_sock, &sock_len);

	printf("connected with %s:%d\n", inet_ntoa(peer_sock.sin_addr),ntohs(peer_sock.sin_port));
	while((message_len = recv(connectFD, readBuff, BUFF_SIZE,0))>=1){

		strcpy(listBuff,readBuff);
		readBuff[message_len] = '\0';//recv always return string +'\0'
		listBuff[strcspn(listBuff,"\n")]='\0';

		add_node(list,listBuff,ntohs(peer_sock.sin_port));

		printf("\nreceive from client %s:%d : ",inet_ntoa(peer_sock.sin_addr),ntohs(peer_sock.sin_port));

		fputs(readBuff, stdout); //print readBuff
		display_list(list,ntohs(peer_sock.sin_port));
		fflush(stdout); //clear buffer


		sprintf(sendBuff,"%s",readBuff);//save readBuff at sendBuff

		send(connectFD, sendBuff, strlen(sendBuff),0);
	}	
	printf("\nconnection end with %s\n", inet_ntoa(peer_sock.sin_addr));
	close(connectFD);

	pthread_exit(NULL);			
}

int main(){
	int listenFD, connectFD;
	struct sockaddr_in listen_sock, connect_sock;
	socklen_t sock_len;
	pthread_t tid;

	list = init_list();

	memset(&listen_sock, 0, sizeof(listen_sock));

	listen_sock.sin_family = AF_INET;
	listen_sock.sin_addr.s_addr = htonl(INADDR_ANY);
	listen_sock.sin_port = htons(PORT);

	listenFD = socket(AF_INET, SOCK_STREAM, 0);

	if (bind(listenFD, (struct sockaddr *) &listen_sock, sizeof(listen_sock)) == -1) {
		printf("Can not bind.\n");
		return -1;
	}

	if (listen(listenFD, LISTEN_QUEUE_SIZE) == -1) {
		printf("Listen fail.\n");
		return -1;
	}

	printf("Waiting for clients...\n");

	sock_len = sizeof(connect_sock);

	while((connectFD = accept(listenFD, (struct sockaddr*)&connect_sock, &sock_len)) >= 0)
	{

		if (connectFD < 0)
		{
			printf("Server: accept failed\n");
			exit(0);
		}

		if (pthread_create(&tid,NULL,handle_client,&connectFD)!=0){
			printf("fail to create thread\n");
			close(connectFD);
		}	
		pthread_detach(tid);
	}
	close(listenFD);
	return 0;
}
