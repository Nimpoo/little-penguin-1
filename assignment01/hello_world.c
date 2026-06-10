#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jean-Proute De La Villardiaire");
MODULE_DESCRIPTION("A simple Hello World module");

static int __init my_init(void) {
	printk(KERN_INFO "Hello, world!\n");
	return 0;
}

static void __exit my_exit(void) {
	printk(KERN_INFO "Goodbye, world!\n");
}

module_init(my_init);
module_exit(my_exit);
