#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include "linklist.h"

#define PORT 8081
#define BUFFER_SIZE 100
#define BUFF_SIZE 20
#define LISTEN_QUEUE_SIZE 5

int main() {

	struct sockaddr_in listen_sock, connect_sock, peer_sock;
	int listenFD, connectFD;
	char readBuff[BUFFER_SIZE],sendBuff[BUFFER_SIZE], listBuff[BUFFER_SIZE];
	ssize_t message_len;
	socklen_t sock_len;
	pid_t pid;
	struct List* list=init_list();

	memset(&listen_sock, 0, sizeof(listen_sock));
	//list=init_list(); 

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

	while ((connectFD = accept(listenFD, (struct sockaddr*)&connect_sock, &sock_len)) >= 0)
	{
		getpeername(connectFD, (struct sockaddr*)&peer_sock, &sock_len);
		//get address of connected socket (socket descriptor, ?buffer, buffer length)

		if (connectFD < 0)
		{
			printf("Server: accept failed\n");
			exit(0);
		}

		printf("connected with %s:%d\n", inet_ntoa(peer_sock.sin_addr),ntohs(peer_sock.sin_port));

		pid=fork(); //create child process
		if (pid==0) //pid of child to parent, 0 to child
		{
			close(listenFD);
			while ((message_len = recv(connectFD, readBuff, BUFF_SIZE,0)) >=1)
			{            
			    	strcpy(listBuff,readBuff);	
				readBuff[message_len] = '\0';//recv always return string +'\0'
				listBuff[strcspn(listBuff,"\n")]='\0';
				printf("\nreceive from client %s : ",inet_ntoa(peer_sock.sin_addr));
				
				/*add received message in node*/
				add_node(list, listBuff);

				fputs(readBuff, stdout); //print readBuff

				/*show all node of list*/
				display_list(list);

				fflush(stdout); //clear buffer
				sprintf(sendBuff,"%s",readBuff);//save readBuff at sendBuff
				
				send(connectFD, sendBuff, strlen(sendBuff),0);
			}
			printf("\nconnection end with %s\n",inet_ntoa(peer_sock.sin_addr));
			close(connectFD);

			return 0; 

		}
		else
			close(connectFD);
	}
	close(listenFD);
	return 0;
}
