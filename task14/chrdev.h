#ifndef CHAR_DEV_H_
#define CHAR_DEV_H_
#include <linux/ioctl.h>
  
struct ioctl_info{
       int data;
       char command;
};

#define             IOCTL_MAGIC         'G'
					/*write data from device driver*/
#define             SET_DATA            _IOW(IOCTL_MAGIC, 2 ,struct ioctl_info)
					/*read data from device driver*/
#define             GET_DATA            _IOR(IOCTL_MAGIC, 3 ,struct ioctl_info)

#define			PRINT_DATA	_IOR(IOCTL_MAGIC, 4, struct ioctl_info)

#define			DELETE_DATA	_IOW(IOCTL_MAGIC, 5, struct ioctl_info)
#endif
