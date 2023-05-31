#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/ioctl.h>
#include "chrdev.h"
  
int main()
{
    int fd;
    struct ioctl_info set_info;
    struct ioctl_info delete_info;
    struct ioctl_info print_info;
    char cmd;
    int data; 

    while(1){
	    printf("a :add, d:delete, p:print, q:quit");
	    printf("\nenter command : ");
	    scanf(" %c",&cmd);
	    getchar();

	    if ((fd = open("/dev/chardev0", O_RDWR)) < 0){
		    printf("Cannot open /dev/chrdev. Try again later.\n");
		    exit(0);
	    }
	    switch(cmd){
		    case 'a':
			    printf("\nenter data : ");
			    scanf("%d",&data);
			    set_info.command = cmd;
			    set_info.data = data;

			    printf("data : %d, cmd : %c\n",set_info.data, set_info.command);
			    if (ioctl(fd, SET_DATA, &set_info) < 0){
				    printf("Error : SET_DATA.\n");
			    }
			    break;
		    case 'd':
			    printf("\nenter data : ");
			    scanf("%d",&data);
			    delete_info.command = cmd;
			    delete_info.data = data;
			    if (ioctl(fd, DELETE_DATA, &delete_info) < 0){
				    printf("Error : DELETE_DATA.\n");
			    }
			    break;
		    case 'p':
			    if (ioctl(fd, PRINT_DATA, &print_info) < 0){
				    printf("Error : PRINT_DATA.\n");
			    }
			    break;
		    case 'q':
			    close(fd);
			    exit(0);
		    default:
			    break;
	    }
    }


    if (close(fd) != 0){
	    printf("Cannot close.\n");
    }
    return 0;
}
