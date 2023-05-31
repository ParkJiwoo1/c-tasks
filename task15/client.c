#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>

#define PORT 8081
#define BUFFER_SIZE 100
#define BUFF_SIZE 100

int main(int argc, char** argv){
	struct sockaddr_in connect_sock;
	int connect_fd, message_len;
	char send_buffer[BUFFER_SIZE], receive_buffer[BUFFER_SIZE];

	if (argc != 2) 
	{
		printf("Usage: %s, input one IPv4-address\n", argv[0]);
		return -1;
	}

	memset(&connect_sock, 0, sizeof(connect_sock));

	connect_sock.sin_family = AF_INET;
	/*change ip address to binay */
	inet_aton(argv[1], (struct in_addr*) &connect_sock.sin_addr.s_addr);
	//host to network short 
	connect_sock.sin_port = htons(PORT);

	connect_fd = socket(AF_INET, SOCK_STREAM, 0);

	if (connect(connect_fd, (struct sockaddr*) &connect_sock, sizeof(connect_sock)) == -1) 
	{
		printf("Can not connect.\n");
		return -1;
	}
	else 
	{
		while (1) 
		{
			printf("input message to send to server : ");

			fgets(send_buffer,BUFF_SIZE,stdin);

			send(connect_fd, send_buffer, strlen(send_buffer),0);

			message_len = recv(connect_fd, receive_buffer, BUFF_SIZE,0);
			receive_buffer[message_len] = '\0';//NULL

			if(message_len>1){
				printf("echo reply from server : ");
				fputs(receive_buffer, stdout);
			}

		}
	}

	close(connect_fd);

	return 0;
}    
