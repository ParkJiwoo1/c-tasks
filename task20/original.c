/*
 * echo program
 * Copyright (C) 2008 Ubiquoss
 *
 * $id: $
 *
 * This file is part of double linked list
 *
 * This program is Ubiquoss's proprietary software; you cannot redistributed it.
 */

/**\file main.c
 * \brief main function
 */
/*
 * modification history
 * --------------------
 *  01a,16apr08,mskang  create file
 */

/**
 * @mainpage Double linked list
 * 
 * @section INTRODUCTION
 * - echo program  
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

#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>

#include "thread.h"
#include "timeutil.h"

/*=C==========================================================================
 * CONSTANTS
 * =========================================================================*/

#define INPUT_BUFF_MAX 50
#define RECV_BUFF_MAX 50

#define MY_UDP_PORT 10000

#define GREETING_INTERVAL_SEC 5

/*=G==========================================================================
 * GLOBAL VARIABLES
 * =========================================================================*/

struct my_sock_info
{
	int socket_fd;
	struct sockaddr_in my_addr; 
};

struct lib_globals my_mux;


/*=F==========================================================================
 * FUNCTION DEFINITION
 * =========================================================================*/

/**
 * initialize my_mux global value
 */
void 
init_my_mux (void)
{
	my_mux.master = thread_master_create ();
}

/**
 * process data from standard input (handler function)
 *
 * @param[in] thread. thread pointer
 *
 * @return success on 0
 */
int
process_data_from_stdin (struct thread *thread)
{
	char buf[INPUT_BUFF_MAX];
	int read_len;

	if (thread == NULL)
	{
		errno = EINVAL;
		perror ("process_data_from_stdin");
		exit (1);
	}

	printf ("[Echo] : ");
	/* read */
	while (1)
	{
		read_len = read (STDIN_FILENO, buf, INPUT_BUFF_MAX);

		if (read_len == 0 || buf[0] == '\n') /* no data */
		{
			break;
		}

		if (read_len == INPUT_BUFF_MAX) /* over flow */
		{
			/* input len is INPUT_BUFF_MAX - 1 */
			if (buf[read_len - 1] == '\n') 					{
				buf[read_len - 1] = '\0';
				printf ("%s\n", buf);
				break;

			}
			else /* over flow */
			{
				buf[read_len] = '\0';
				printf ("%s", buf);
			}
		}
		else
		{
			buf[read_len - 1] = '\0';
			printf ("%s\n", buf);
			break;


		}
	}

	/* create thread for standard input */
	if (thread_add_read (&my_mux,
				process_data_from_stdin, NULL, STDIN_FILENO) == NULL)
	{
		perror ("process_data_from_stdin(thread_add_read)");
		exit (1);
	}

	return 0;
	
}

/**
 * process data from udp socket (handler function)
 *
 * @param[in] thread. thread pointer
 *
 * @return success on 0
 */
int
process_data_from_udp_sock (struct thread *thread)
{
	int recv_len;
	struct sockaddr_in their_addr;
	char recv_buf[RECV_BUFF_MAX];
	int addr_len;

	if (thread == NULL)
	{
		errno = EINVAL;
		perror ("process_data_from_dup_sock");
		exit (1);
	}

	/* recv from udp socket */
	recv_len = recvfrom (thread->u.fd, recv_buf, INPUT_BUFF_MAX - 1, 0,
			(struct sockaddr *)&their_addr, &addr_len);

	if (recv_len == -1)
	{
		perror ("process_data_from_udp_sock");
		exit (1);
	}

	printf ("[form UDP socket] : %s\n", recv_buf);

	/* create thread */
	if (thread_add_read (&my_mux, 
				process_data_from_udp_sock, NULL, thread->u.fd) == NULL)
	{
		perror ("process_data_from_udp_sock(thread_add_read)");
		exit (1);
	}

	return 0;
}

/**
 * create my_socket
 *
 * @parma[in] my_sock. indicate information related to socket
 */
void
create_socket (struct my_sock_info *my_sock)
{
	int sockfd;

	if (my_sock == NULL)
	{
		errno = EINVAL;
		perror ("create_socket");
		exit (1);
	}

	/* create socket */
	sockfd = socket (AF_INET, SOCK_DGRAM, 0);

	if (my_sock->socket_fd == -1) /* create error */
	{
		perror ("create_socket(socket)");
		exit (1);
	}

	my_sock->socket_fd = sockfd;

	/* set my ip address and port number */
	my_sock->my_addr.sin_family = AF_INET;
	my_sock->my_addr.sin_port = htons (MY_UDP_PORT);
	my_sock->my_addr.sin_addr.s_addr = htonl (INADDR_ANY);

	if (bind (sockfd, (struct sockaddr*)&my_sock->my_addr, 
				sizeof (struct sockaddr)) == -1)
	{
		perror ("creat_socket(bind)");
		exit (1);
	}

	/* create thread */
	if (thread_add_read (&my_mux, 
				process_data_from_udp_sock, NULL, sockfd) == NULL)
	{
		perror ("cread_socket(thread_add_read)");
		exit (1);
	}
}

/**
 * print greeting message and add timer (handler function)
 *
 * @param[in] thread. thread pointer
 *
 * @return success on 0
 */
int
greeting_timeout (struct thread *thread)
{
	struct timeval g_interval;

	if (thread == NULL)
	{
		errno = EINVAL;
		perror ("greeting_timeout");
		exit (1);
	}

	printf ("Hello!!\n");

	g_interval.tv_sec = GREETING_INTERVAL_SEC;
	g_interval.tv_usec = 0;

	/* add timer */
	if (thread_add_timer_timeval (&my_mux,
				greeting_timeout, NULL, g_interval) == NULL)
	{
		perror ("greeing_timeout(thread_add_timer_timeval)");
		exit (1);
	}

	return 0;
}

/**
 * initialize timer for greeting message
 *
 * @param[in] m. thread pointer
 */
void
init_timer_for_greeting_message (struct thread_master *m)
{
	struct timeval g_interval;

	if (m == NULL)
	{
		errno = EINVAL;
		perror ("init_time_for_greeting_message");
		exit (1);
	}

	g_interval.tv_sec = GREETING_INTERVAL_SEC;
	g_interval.tv_usec = 0;

	if (thread_add_timer_timeval (&my_mux,
				greeting_timeout, NULL, g_interval) == NULL)
	{
		perror ("init_timer_for_greeting_message");
		exit (1);
	}

}

/**
 * main function
 */
int
main (int argc, char *argv[])
{
	struct my_sock_info my_sock;
	struct thread fetch;

	init_my_mux (); /* initialize global variable */

	/* create udp socket and create thread */
	create_socket (&my_sock);

	/* create thread for standard input */
	if (thread_add_read (&my_mux,
				process_data_from_stdin, NULL, STDIN_FILENO) == NULL)
	{
		perror ("main(thread_add_read)");
		exit (1);
	}
	
	/* add timer to time slot for greeting message */
	init_timer_for_greeting_message (my_mux.master);

	while (1) /* main loop */
	{
		if (thread_fetch (&my_mux, &fetch))
		{
			thread_call (&fetch);
		}

	}

		
	return 0;
}
