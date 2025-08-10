#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/stat.h>
#include <linux/init.h>
#include <linux/ctype.h>

#define pr_fmt(fmt) "hello: " fmt

// Parameters
#define HELLO_WORLD_SIZE 13

//H e l l o ,   W o r l  d  !  - 13 symbols
//0 1 2 3 4 5 6 7 8 9 10 11 12 -
static char str_buf[HELLO_WORLD_SIZE + 1] = { 0 };

static int idx = 0;
static char ch_val = '\0';

// Params configuration
static int idx_get(char *buffer, const struct kernel_param *kp)
{
	return param_get_int(buffer, kp);
}

static int idx_set(const char *val, const struct kernel_param *kp)
{
	int res;
	int index;

	res = kstrtoint(val, 10, &index);
	if (res) {
		pr_err("Error: invalid idx parameter value %d!\n", res);
		return -EINVAL;
	}

	if (index < 0 || index > HELLO_WORLD_SIZE) {
		pr_err("Error: idx value must be between 0 and %d!\n",
		       HELLO_WORLD_SIZE);
		return -EINVAL;
	}

	idx = index;

	pr_info("idx changed to: %d\n", index);
	return 0;
}

static int ch_val_get(char *buffer, const struct kernel_param *kp)
{
	return sysfs_emit(buffer, "%c", ch_val);
}

static int ch_val_set(const char *val, const struct kernel_param *kp)
{
	if (!val) {
		pr_err("Error: null input data for ch_val parameter!\n");
		return -EINVAL;
	}

	if (strlen(val) == 0) {
		pr_err("Error: empty input data for ch_val parameter value!\n");
		return -EINVAL;
	}

	pr_info("Char values: %s\n", val);

	char symbol = val[0];
	if (!isprint(symbol)) {
		pr_err("Error: invalid parameter value!\n");
		return -EINVAL;
	}

	ch_val = symbol;
	pr_info("ch_val changed to: %c\n", ch_val);
	str_buf[idx] = symbol;
	return 0;
}

static int str_buf_get(char *buffer, const struct kernel_param *kp)
{
	return sysfs_emit(buffer, "%s", str_buf);
}

static const struct kernel_param_ops idx_ops = {
	.set = idx_set,
	.get = idx_get,
};

static const struct kernel_param_ops ch_val_ops = {
	.set = ch_val_set,
	.get = ch_val_get,
};

static const struct kernel_param_ops str_buf_ops = {
	.get = str_buf_get,
};

module_param_cb(idx, &idx_ops, &idx, 0644);
MODULE_PARM_DESC(idx, "Index");

module_param_cb(ch_val, &ch_val_ops, NULL, 0644);
MODULE_PARM_DESC(ch_val, "Symbol");

module_param_cb(str_buf, &str_buf_ops, NULL, 0444);
MODULE_PARM_DESC(str_buf, "String");

// Initialization
static int __init mod_init(void)
{
	pr_info("init\n");
	return 0;
}

static void __exit mod_exit(void)
{
	pr_info("exit\n");
}

module_init(mod_init);
module_exit(mod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("varthlokkur");
MODULE_DESCRIPTION("Module HW 02");
