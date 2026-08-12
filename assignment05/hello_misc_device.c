// SPDX-License-Identifier: GPL-2.0 OR MIT

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/miscdevice.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Marwan AYOUB");
MODULE_DESCRIPTION("A simple Linux misc character device driver");

#define LOGIN_SIZE 6
#define LOGIN "mayoub"

static ssize_t my_write(struct file *file, const char __user *user_buf, size_t user_len, loff_t *ppos)
{
	char buf[LOGIN_SIZE];

	if (user_len != LOGIN_SIZE) {
		pr_err("fortytwo_misc_device - My write function called with incorrect login length.\n");
		return -EINVAL;
	}

	if (copy_from_user(buf, user_buf, user_len)) {
		pr_err("fortytwo_misc_device - Copy from user failed.\n");
		return -EFAULT;
	}

	if (memcmp(buf, LOGIN, LOGIN_SIZE) != 0) {
		pr_err("fortytwo_misc_device - Incorrect login.\n");
		return -EINVAL;
	}

	pr_info("fortytwo_misc_device - My write called.\n");
	return user_len;
}

static ssize_t my_read(struct file *file, char __user *user_buf, size_t user_len, loff_t *ppos)
{
	if (!user_buf) {
		pr_err("fortytwo_misc_device - My read function called with a NULL user' buffer.\n");
		return -EINVAL;
	}

	if (*ppos >= LOGIN_SIZE) {
		pr_info("fortytwo_misc_device - My read function called but all the data has been read.\n");
		return 0; // EOF
	}

	int len = user_len < LOGIN_SIZE - *ppos ? user_len : LOGIN_SIZE - *ppos;

	if (copy_to_user(user_buf, LOGIN + *ppos, len)) {
		pr_err("fortytwo_misc_device - Copy to user failed.\n");
		return -EFAULT;
	}

	pr_info("fortytwo_misc_device - My read called.\n");
	*ppos += len;
	return len;
}

static int my_open(struct inode *inode, struct file *file)
{
	pr_info("fortytwo_misc_device [%d, %d] - My open called.\n", iminor(inode), imajor(inode));
	return 0;
}

static int my_close(struct inode *inode, struct file *file)
{
	pr_info("fortytwo_misc_device [%d, %d] - My close called.\n", iminor(inode), imajor(inode));
	return 0;
}

static const struct file_operations hello_fops = {
	.owner = THIS_MODULE,
	.read = my_read,
	.write = my_write,
	.open = my_open,
	.release = my_close,
};

static struct miscdevice fortytwo_misc_device = {
		.minor = MISC_DYNAMIC_MINOR,  // Use dynamic minor number
		.name = "fortytwo",           // Name of the device
		.fops = &hello_fops,          // Pointer to file operations structure
};

static int __init my_init(void)
{
	int status;

	pr_info("fortytwo_misc_device - Hello, misc device!\n");

	// Register the device with the kernel, return 0 on success, negative error code on failure
	status = misc_register(&fortytwo_misc_device);
	if (status) {
		pr_err("fortytwo_misc_device - Unable to register misc device.\n");
		return status;
	}
	return 0;
}

static void __exit my_exit(void)
{
	pr_info("fortytwo_misc_device - Goodbye, misc device!\n");
	misc_deregister(&fortytwo_misc_device); // Deregister the device from the kernel
}

module_init(my_init);
module_exit(my_exit);
