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

#include "chrdev.h"
MODULE_LICENSE("GPL");

#define DRIVER_NAME "chrdev"

static const unsigned int MINOR_BASE = 0;
static const unsigned int MINOR_NUM  = 1;
static unsigned int chardev_major;
static struct cdev chardev_cdev;
static struct class *chardev_class = NULL;

static struct ioctl_info info;
struct list_node *new_node;
struct list_node *current_node, *next_node;
int data_to_delete;
static struct ioctl_info del_info;

/*static int     chardev_open(struct inode *, struct file *);
static int     chardev_release(struct inode *, struct file *);
static ssize_t chardev_read(struct file *, char *, size_t, loff_t *);
static ssize_t chardev_write(struct file *, const char *, size_t, loff_t *);
*/static long chardev_ioctl(struct file *, unsigned int, unsigned long);

struct file_operations s_chardev_fops = {
/*	.open    = chardev_open,
	.release = chardev_release,
	.read    = chardev_read,
	.write   = chardev_write,
*/	.unlocked_ioctl = chardev_ioctl,
};

struct list_node {
    int data;
    struct list_head list;
};

LIST_HEAD(my_list);

static int __init chardev_init(void)
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
	chardev_class = class_create(THIS_MODULE, "chrdev");
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

static void __exit chardev_exit(void)
{
	struct list_node *current_node, *tmp_node;
	int minor = 0;
	dev_t dev = MKDEV(chardev_major, MINOR_BASE);

	printk("The chardev_exit() function has been called.");
	list_for_each_entry_safe(current_node, tmp_node, &my_list, list) {
		list_del(&current_node->list);
		kfree(current_node);
	}
	device_destroy(chardev_class, MKDEV(chardev_major, minor));

	class_destroy(chardev_class);
	cdev_del(&chardev_cdev);
	unregister_chrdev_region(dev, MINOR_NUM);
}
/*
   static int chardev_open(struct inode *inode, struct file *file)
   {
   printk("The chardev_open() function has been called.");
   return 0;
   }
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
   */
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
			printk("info.data : %d, info.cmd : %c",info.data, info.command);
			new_node = kmalloc(sizeof(*new_node), GFP_KERNEL);
			if (!new_node)
				return -ENOMEM;

			new_node->data = info.data;
			INIT_LIST_HEAD(&new_node->list);

			list_add_tail(&new_node->list, &my_list);
			break;
		case DELETE_DATA:
			printk("DELETE_DATA\n");
			if (copy_from_user(&del_info, (int __user *)arg, sizeof(del_info))) {
				return -EFAULT;
			}

			printk("delete data : %d",del_info.data);
			list_for_each_entry_safe(current_node, next_node, &my_list, list) {
				if (current_node->data == del_info.data) {
					list_del(&current_node->list);
					kfree(current_node);
				}
			}
			break;
		case PRINT_DATA:
			printk("PRINT_DATA\n");
			list_for_each_entry(current_node, &my_list, list){
				printk(" [%d] ", current_node->data);
			}
			break;
		default:
			printk(KERN_WARNING "unsupported command %d\n", cmd);
			return -EFAULT;
			break;
	}
	return 0;
}

module_init(chardev_init);
module_exit(chardev_exit);
