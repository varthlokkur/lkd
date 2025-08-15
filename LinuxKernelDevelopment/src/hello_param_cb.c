#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/stat.h>
#include <linux/init.h>

static int param;

static int param_get(char *buffer, const struct kernel_param *kp)
{
	return param_get_int(buffer, kp);
}

static int param_set(const char *val, const struct kernel_param *kp)
{
	int res;
	int _val;

	res = kstrtoint(val, 10, &_val);
	if (res) {
		pr_err("Ошибка: неверное значение параметра!\n");
		return res;
	}

	if (_val < 0 || _val > 100) {
		pr_err("Ошибка: значение должно быть от 0 до 100!\n");
		return -EINVAL;
	}

	param = _val;

	pr_info("Параметр изменен на: %d\n", param);
	return 0;
}

static const struct kernel_param_ops param_ops = {
	.set = param_set,
	.get = param_get,
};

module_param_cb(param, &param_ops, &param, 0644);
MODULE_PARM_DESC(param, "Пример параметра с callback (диапазон: 0-100)");

static int __init mod_init(void)
{
	pr_info("Модуль загружен. Текущее значение параметра: %d\n", param);
	return 0;
}

static void __exit mod_exit(void)
{
	pr_info("Модуль выгружен\n");
}

module_init(mod_init);
module_exit(mod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("OldALoneFag");
MODULE_DESCRIPTION("A simple param-callback module for the Linux kernel");
