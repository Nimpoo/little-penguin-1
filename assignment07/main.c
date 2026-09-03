// SPDX-License-Identifier: GPL-2.0 OR MIT
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/debugfs.h>

#include "assignment07.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Marwan AYOUB");
MODULE_DESCRIPTION("Kernel module that create 3 virtual files for debug purposes.");

struct module_debugfs *debugfs_data;

static int error(char *msg)
{
	pr_err("%s\n", msg);
	debugfs_remove_recursive(debugfs_data->dir);
	return -ENOMEM;
}

static int __init debugfs_init(void)
{
	mutex_init(&foo_mutex);

	debugfs_data = kmalloc(sizeof(struct module_debugfs), GFP_KERNEL);
	if (!debugfs_data)
		return error("Failed to allocate memory for `debugfs_data`.");

	debugfs_data->dir = debugfs_create_dir("fortytwo", NULL);
	if (!debugfs_data->dir) {
		kfree(debugfs_data);
		return error("Failed to create debugfs directory.");
	}

	debugfs_data->id_file = debugfs_create_file("id", 0666, debugfs_data->dir, NULL, &id_fops);
	if (!debugfs_data->id_file) {
		kfree(debugfs_data);
		return error("Failed to create debugfs id file.");
	}

	debugfs_data->jiffies_file = debugfs_create_file("jiffies", 0444, debugfs_data->dir, NULL, &jiffies_fops);
	if (!debugfs_data->jiffies_file) {
		kfree(debugfs_data);
		return error("Failed to create debugfs jiffies file.");
	}

	debugfs_data->foo_file = debugfs_create_file("foo", 0644, debugfs_data->dir, NULL, &foo_fops);
	if (!debugfs_data->foo_file) {
		kfree(debugfs_data);
		return error("Failed to create debugfs foo file.");
	}

	return 0;
}

static void __exit debugfs_exit(void)
{
	if (debugfs_data) {
		pr_info("DebugFS module - removing directory and files.\n");
		debugfs_remove_recursive(debugfs_data->dir);
		kfree(debugfs_data);
	}
	pr_info("DebugFS module exited.\n");
}

module_init(debugfs_init);
module_exit(debugfs_exit);
