#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/sched.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <asm/current.h>
#include <linux/uaccess.h>

#include "chardev.h"
MODULE_LICENSE("GPL");

#define DRIVER_NAME "chardev"

static const unsigned int MINOR_BASE = 0;
static const unsigned int MINOR_NUM  = 1;
static unsigned int chardev_major;
static struct cdev chardev_cdev;
static struct class *chardev_class = NULL;

static int     chardev_open(struct inode *, struct file *);
static int     chardev_release(struct inode *, struct file *);
static ssize_t chardev_read(struct file *, char *, size_t, loff_t *);
static ssize_t chardev_write(struct file *, const char *, size_t, loff_t *);
static long chardev_ioctl(struct file *, unsigned int, unsigned long);

struct file_operations s_chardev_fops = {
	.open    = chardev_open,
	.release = chardev_release,
	.read    = chardev_read,
	.write   = chardev_write,
	.unlocked_ioctl = chardev_ioctl,
};

static int chardev_init(void)
{
	int alloc_ret = 0;
	int cdev_err = 0;
	int minor = 0;
	dev_t dev;

	printk("The chardev_init() function has been called.");

	/*allocate device number dynamically
	 *MINOR_BASE usually 0 when first allocated to device
	 */ 
	alloc_ret = alloc_chrdev_region(&dev, MINOR_BASE, MINOR_NUM, DRIVER_NAME);
	if (alloc_ret != 0) {
		printk(KERN_ERR  "alloc_chrdev_region = %d\n", alloc_ret);
		return -1;
	}
	/*Get the major number value in dev
	 *major number for define what kind of device
	 *minor for specefic like port number
	 */
	chardev_major = MAJOR(dev);
	dev = MKDEV(chardev_major, MINOR_BASE);

	/*initialize a cdev structure
	 * (cdev structure, pointer of structure that want to register)
	 */
	cdev_init(&chardev_cdev, &s_chardev_fops);

	/*pointer of module that own class*/
	chardev_cdev.owner = THIS_MODULE;

	/*add a char device to the system*/
	cdev_err = cdev_add(&chardev_cdev, dev, MINOR_NUM);
	if (cdev_err != 0) {
		printk(KERN_ERR  "cdev_add = %d\n", alloc_ret);
		unregister_chrdev_region(dev, MINOR_NUM);
		return -1;
	}
	/*create structure pointer to use when call device create
	*include name, owner module, class property
	*/
	chardev_class = class_create(THIS_MODULE, "chardev");
	if (IS_ERR(chardev_class)) {
		printk(KERN_ERR  "class_create\n");

		/*delete character device, device file, device number*/
		cdev_del(&chardev_cdev);
		unregister_chrdev_region(dev, MINOR_NUM);
		return -1;
	}
	/*create device file belongs to class in this system
	*
	*/
	device_create(chardev_class, NULL, MKDEV(chardev_major, minor), NULL, "chardev%d", minor);
	return 0;
}

static void chardev_exit(void)
{
	int minor = 0;
	dev_t dev = MKDEV(chardev_major, MINOR_BASE);

	printk("The chardev_exit() function has been called.");

	device_destroy(chardev_class, MKDEV(chardev_major, minor));

	class_destroy(chardev_class);
	cdev_del(&chardev_cdev);
	unregister_chrdev_region(dev, MINOR_NUM);
}

static int chardev_open(struct inode *inode, struct file *file)
{
	printk("The chardev_open() function has been called.");
	return 0;
}
/*close device*/
static int chardev_release(struct inode *inode, struct file *file)
{
	printk("The chardev_close() function has been called.");
	return 0;
}

static ssize_t chardev_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
	printk("The chardev_write() function has been called.");  
	return count;
}

static ssize_t chardev_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	printk("The chardev_read() function has been called.");
	return count;
}

static struct ioctl_info info;

/*request to cmd, rest to arg*/
static long chardev_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	printk("The chardev_ioctl() function has been called.");

	switch (cmd) {
		case SET_DATA:
			printk("SET_DATA\n");
			/*write kernel memory block data to data memory block*/
			if (copy_from_user(&info, (void __user *)arg, sizeof(info))) {
				return -EFAULT;
			}
			printk("info.size : %ld, info.buf : %s",info.size, info.buf);
			break;
		case GET_DATA:
			printk("GET_DATA\n");
			/*write user memory block data to kernel memory data block*/
			if (copy_to_user((void __user *)arg, &info, sizeof(info))) {
				return -EFAULT;
			}
			break;
		default:
			printk(KERN_WARNING "unsupported command %d\n", cmd);

			return -EFAULT;
	}
	return 0;
}

module_init(chardev_init);
module_exit(chardev_exit);
