// SPDX-License-Identifier: GPL-2.0 OR MIT

#include <linux/init.h>

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/nsproxy.h>
#include <linux/ns_common.h>
#include <../fs/mount.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Marwan AYOUB");
MODULE_DESCRIPTION("Kernel Module for mountpoints listing. Go see [FILE REPO]");

static struct proc_dir_entry *mymounts_proc_entry;

static void traverse_mounts(struct mount *mnt, struct seq_file *m)
{
	struct mount *child;
	struct path path;
	char *buf = kmalloc(PATH_MAX, GFP_KERNEL);

	list_for_each_entry(child, &mnt->mnt_mounts, mnt_child) {
		path.mnt = &child->mnt;
		path.dentry = path.mnt->mnt_root;

		seq_printf(m, "%-15s%s\n", child->mnt_devname, d_path(&path, buf, PATH_MAX));

		kfree(buf);
		traverse_mounts(child, m);
	}
}

static int mymounts_show(struct seq_file *m, void *v)
{
	// Get the mount namespace of the current process
	struct mnt_namespace *mount_ns = current->nsproxy->mnt_ns;

	if (!mount_ns) {
		seq_puts(m, "Erro: No mount namespace found for the current process.\n");
		return 0;
	}

	// Traverse the mount namespace tree and list all mounted filesystems
	traverse_mounts(mount_ns->root, m);

	return 0;
}

static int __init mymounts_init(void)
{
	pr_info("mymounts: Initializing module.\n");

	mymounts_proc_entry = proc_create_single("mymounts", 0666, NULL, mymounts_show);

	return 0;
}

static void __exit mymounts_exit(void)
{
	pr_info("mymounts: Exiting module.\n");
}

module_init(mymounts_init);
module_exit(mymounts_exit);
