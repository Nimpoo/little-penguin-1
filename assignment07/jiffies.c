// SPDX-License-Identifier: GPL-2.0 OR MIT

#include "assignment07.h"

static int jiffies_open(struct inode *inode, struct file *file)
{
	pr_info("jiffies_file [%d, %d] - `jiffies` open called.\n", iminor(inode), imajor(inode));
	return 0;
}

static int jiffies_close(struct inode *inode, struct file *file)
{
	pr_info("jiffies_file [%d, %d] - `jiffies` close called.\n", iminor(inode), imajor(inode));
	return 0;
}

static ssize_t jiffies_read(struct file *file, char __user *user_buf, size_t user_len, loff_t *ppos)
{
	if (!user_buf) {
		pr_err("jiffies_read - `jiffies` read function called with a NULL user' buffer.\n");
		return -EINVAL;
	}

	char buf[64];
	u64 jiffies = get_jiffies_64();
	int len = snprintf(buf, sizeof(buf), "%llu", jiffies);

	pr_info("jiffies_file debugfs - `id` read called.\n");
	return simple_read_from_buffer(user_buf, user_len, ppos, buf, len);
}

const struct file_operations jiffies_fops = {
	.owner = THIS_MODULE,
	.read = jiffies_read,
	.open = jiffies_open,
	.release = jiffies_close,
};

EXPORT_SYMBOL(jiffies_fops);
