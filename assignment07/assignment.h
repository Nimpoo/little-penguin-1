/* SPDX-License-Identifier: GPL-2.0 */
#ifndef ASSIGNMENT07_H
#define ASSIGNMENT07_H

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/debugfs.h>
#include <linux/fs.h>

struct module_debugfs {
	struct dentry *dir;
	struct dentry *id_file;
	struct dentry *jiffies_file;
	struct dentry *foo_file;
};

extern const struct file_operations id_fops;
extern const struct file_operations jiffies_fops;
extern const struct file_operations foo_fops;

extern struct mutex foo_mutex;

#endif
