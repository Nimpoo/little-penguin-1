// SPDX-License-Identifier: GPL-2.0 OR MIT

#include "assignment07.h"

#define LOGIN_SIZE 6
#define LOGIN "mayoub"

static ssize_t id_write(struct file *file, const char __user *user_buf, size_t user_len, loff_t *ppos)
{
	char buf[LOGIN_SIZE];

	if (user_len != LOGIN_SIZE) {
		pr_err("id_file debugfs - `id` write function called with incorrect login length.\n");
		return -EINVAL;
	}

	if (copy_from_user(buf, user_buf, user_len)) {
		pr_err("id_file debugfs - Copy from user failed.\n");
		return -EFAULT;
	}

	if (memcmp(buf, LOGIN, LOGIN_SIZE) != 0) {
		pr_err("id_file debugfs - Incorrect login.\n");
		return -EINVAL;
	}

	pr_info("id_file debugfs - `id` write called.\n");
	return user_len;
}

static ssize_t id_read(struct file *file, char __user *user_buf, size_t user_len, loff_t *ppos)
{
	if (!user_buf) {
		pr_err("id_file debugfs - `id` read function called with a NULL user' buffer.\n");
		return -EINVAL;
	}

	if (*ppos >= LOGIN_SIZE) {
		pr_info("id_file debugfs - `id` read function called but all the data has been read.\n");
		return 0; // EOF
	}

	int len = user_len < LOGIN_SIZE - *ppos ? user_len : LOGIN_SIZE - *ppos;

	if (copy_to_user(user_buf, LOGIN + *ppos, len)) {
		pr_err("id_file debugfs - Copy to user failed.\n");
		return -EFAULT;
	}

	pr_info("id_file debugfs - `id` read called.\n");
	*ppos += len;
	return len;
}

static int id_open(struct inode *inode, struct file *file)
{
	pr_info("id_file debugfs [%d, %d] - `id` open called.\n", iminor(inode), imajor(inode));
	return 0;
}

static int id_close(struct inode *inode, struct file *file)
{
	pr_info("id_file debugfs [%d, %d] - `id` close called.\n", iminor(inode), imajor(inode));
	return 0;
}

const struct file_operations id_fops = {
	.owner = THIS_MODULE,
	.read = id_read,
	.write = id_write,
	.open = id_open,
	.release = id_close,
};

EXPORT_SYMBOL(id_fops);
