#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/ioctl.h>
#include "chardev.h"
  
int main()
{
    int fd;
    struct ioctl_info set_info;
    struct ioctl_info get_info;
    char task[100] = "this is string for task13";
 
    set_info.size = 100;
    strncpy(set_info.buf,task,strlen(task));
 
    if ((fd = open("/dev/chardev0", O_RDWR)) < 0){
        printf("Cannot open /dev/chardev0. Try again later.\n");
    }
  
    if (ioctl(fd, SET_DATA, &set_info) < 0){
        printf("Error : SET_DATA.\n");
    }
 
 
    if (ioctl(fd, GET_DATA, &get_info) < 0){
        printf("Error : SET_DATA.\n");
    }
  
    printf("get_info.size : %ld, get_info.buf : %s\n", get_info.size, get_info.buf);
  
    if (close(fd) != 0){
        printf("Cannot close.\n");
    }
    return 0;
}
