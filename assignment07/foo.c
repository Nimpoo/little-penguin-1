// SPDX-License-Identifier: GPL-2.0 OR MIT

#include "assignment07.h"

static char buf[PAGE_SIZE];
static size_t len;

// `foo_mutex' initialized in the `main.c' file, in the `__init' function.
struct mutex foo_mutex;

static int foo_open(struct inode *inode, struct file *file)
{
	if (mutex_lock_interruptible(&foo_mutex)) {
		pr_err("foo_file [%d, %d] - `foo` open function called but mutex lock failed.\n",
		       iminor(inode), imajor(inode));
		return -EINTR;
	}

	pr_info("foo_file [%d, %d] - `foo` open called.\n", iminor(inode), imajor(inode));

	if (file->f_mode & FMODE_WRITE)
		pr_info("foo_file [%d, %d] - `foo` open function called with write permissions.\n",
			iminor(inode), imajor(inode));

	if (file->f_mode & FMODE_READ)
		pr_info("foo_file [%d, %d] - `foo` open function called with read permissions.\n",
			iminor(inode), imajor(inode));

	return 0;
}

static int foo_close(struct inode *inode, struct file *file)
{
	mutex_unlock(&foo_mutex);
	pr_info("foo_file [%d, %d] - `foo` close called.\n", iminor(inode), imajor(inode));
	return 0;
}

static ssize_t foo_read(struct file *file, char __user *user_buf, size_t user_len, loff_t *ppos)
{
	int ret = simple_read_from_buffer(user_buf, user_len, ppos, buf, len);

	if (!ret)
		pr_info("foo_file debugfs - `foo` read function called but all the data has been read.\n");
	else if (ret < 0)
		pr_err("foo_file debugfs - `foo` read function failed.\n");
	else
		pr_info("foo_file debugfs - `foo` read function called, %d bytes read.\n", ret);

	return ret;
}

static ssize_t foo_write(struct file *file, const char __user *user_buf,
			 size_t user_len, loff_t *ppos)
{
	if (user_len + *ppos > PAGE_SIZE) {
		pr_err("foo_file debugfs - `foo` write function called with a size greater than `PAGE_SIZE' (4096 bytes).\n");
		return -EINVAL;
	}

	int ret = simple_write_to_buffer(buf, PAGE_SIZE, ppos, user_buf, user_len);

	len = *ppos;
	pr_info("foo_file debugfs - `foo` write function called, %d bytes written.\n", ret);
	return ret;
}

const struct file_operations foo_fops = {
	.owner = THIS_MODULE,
	.read = foo_read,
	.write = foo_write,
	.open = foo_open,
	.release = foo_close,
};
EXPORT_SYMBOL(foo_fops);
