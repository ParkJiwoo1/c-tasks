#ifndef CHAR_DEV_H_
#define CHAR_DEV_H_
#include <linux/ioctl.h>
  
struct ioctl_info{
       unsigned long size;
       char buf[128];
};

#define             IOCTL_MAGIC         'G'
					/*write data from device driver*/
#define             SET_DATA            _IOW(IOCTL_MAGIC, 2 ,struct ioctl_info)
					/*read data from device driver*/
#define             GET_DATA            _IOR(IOCTL_MAGIC, 3 ,struct ioctl_info)

#endif
