#include <linux/ctype.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/stat.h>

#define pr_fmt(fmt) "hello: " fmt

// Initialization
static int __init mod_init(void) {
  pr_info("init\n");
  return 0;
}

static void __exit mod_exit(void) { pr_info("exit\n"); }

module_init(mod_init);
module_exit(mod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("varthlokkur");
MODULE_DESCRIPTION("Module HW 03 bitmap");