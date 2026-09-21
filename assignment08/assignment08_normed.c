// SPDX-License-Identifier: GPL-2.0 OR MIT

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Louis Solofrizzo <louis@ne02ptzero.me>");
MODULE_DESCRIPTION("Reverse string misc device module");

char str[PAGE_SIZE];

static ssize_t myfd_read(struct file *fp, char __user *user, size_t size, loff_t *offs)
{
	size_t rev_index = strlen(str);
	size_t i = 0;
	ssize_t res = 0;
	char *tmp = kmalloc(sizeof(char) * PAGE_SIZE + 1, GFP_KERNEL);

	if (!tmp)
		return -ENOMEM;

	while (rev_index > 0) {
		tmp[i] = str[rev_index];
		rev_index--;
		i++;
	}

	pr_info("reverse read function called, reversed string.\n");

	res = simple_read_from_buffer(user, PAGE_SIZE, offs, tmp, i);
	if (res < 0)
		pr_err("reverse read function failed.\n");
	kfree(tmp);

	return res;
}

static ssize_t myfd_write(struct file *fp, const char __user *user, size_t size, loff_t *offs)
{
	ssize_t res = simple_write_to_buffer(str, size, offs, user, size);

	if (res < 0)
		pr_err("reverse write function failed.\n");
	else
		pr_info("reverse write function called, written string: %s.\n", str);

	return res;
}

static const struct file_operations myfd_fops = {
	.owner = THIS_MODULE,
	.read = &myfd_read,
	.write = &myfd_write
};

static struct miscdevice myfd_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "reverse",
	.fops = &myfd_fops
};

static int __init myfd_init(void)
{
	int retval = misc_register(&myfd_device);

	if (retval) {
		pr_err("reverse: failed to register device.\n");
		return -retval;
	}

	pr_info("reverse: device registered successfully.\n");
	return 0;
}

static void __exit myfd_cleanup(void)
{
	misc_deregister(&myfd_device);
	pr_info("reverse: device unregistered successfully.\n");
}

module_init(myfd_init);
module_exit(myfd_cleanup);
