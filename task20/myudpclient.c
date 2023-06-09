/*
 * udp send program
 * Copyright (C) 2008 Ubiquoss
 *
 * $id: $
 *
 * This file is part of echo program
 *
 * This program is Ubiquoss's proprietary software; you cannot redistributed it.
 */

/**\file myupdclient.c
 * \brief udp send program
 */
/*
 * modification history
 * --------------------
 *  01a,16apr08,mskang  create file
 */

/**
 * @mainpage Echo program
 * 
 * @section INTRODUCTION
 * - Double linked list program  
 * 
 * @section  CREATEINFO
 * - Writer : Kang, Minsu
 * - Date   : 2008/04/16
 *
 * @section  MODIFICATIONINFO
 * - 2008/04/XX : Kang, Minsu
 */


/*=H==========================================================================
 * HEADER INCLUSION
 * =========================================================================*/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netdb.h>
#include<netinet/in.h>
#include<arpa/inet.h>


/*=C==========================================================================
 * CONSTANTS
 * =========================================================================*/

#define MY_UDP_PORT 10000

/*=G==========================================================================
 * GLOBAL VARIABLES
 * =========================================================================*/


/*=F==========================================================================
 * FUNCTION DEFINITION
 * =========================================================================*/

/**
 * main function
 */
int 
main (int argc, char *argv[])
{
	int sockfd;
	struct hostent *he;
	struct servent *se;
	struct sockaddr_in their_addr;
	int numbytes;
	/*char buf[80] = "UDP socket message!!";*/
	char buf[80];
	char recv_buf[80];

	if (argc != 2)
	{
		fprintf (stderr, "usage: myudpclient hostname\n");
		exit (1);
	}

	if ((he = gethostbyname (argv[1])) == NULL)
	{
		perror ("gethostbyname");
		exit (1);
	}

	if ((sockfd = socket (AF_INET, SOCK_DGRAM, 0)) == -1)
	{
		perror ("scoket");
		exit(1);
	}

	their_addr.sin_family = AF_INET;
	their_addr.sin_port = htons (MY_UDP_PORT);
	their_addr.sin_addr = *((struct in_addr *)he->h_addr);
	memset (&(their_addr.sin_zero), '\0', 8);

	while (1)
	{
		//getchar ();
		printf("input message to send to server : ");
		fgets(buf,sizeof(buf),stdin);

		if ((numbytes = sendto (sockfd, buf, strlen(buf), 0,(struct sockaddr*)&their_addr, (int)sizeof(struct sockaddr))) == -1)
		{
			perror ("sendto");
			exit (1);
		}
		//recvfrom(sockfd,recv_buf,sizeof(recv_buf),0,(struct sockaddr*)&their_addr,(int)sizeof(struct sockaddr));
		//printf ("send %s to %s\n", 
		//		recv_buf, inet_ntoa (their_addr.sin_addr));
	}
	close (sockfd);

	return 0;
}
