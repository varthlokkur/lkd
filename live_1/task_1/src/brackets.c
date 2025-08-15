#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/stat.h>
#include <linux/init.h>

#include "stack.h"

#define pr_fmt(fmt) "brackets: " fmt

// Parameters
static char* brackets_sequence = NULL;

// Params configuration
static int sequence_get(char *buffer, const struct kernel_param *kp) 
{
    return param_get_int(buffer, kp); // Стандартный вывод для типа int
}


static int sequence_set(const char *val, const struct kernel_param *kp) 
{
    if (new_value < 0 || new_value > 100) 
    {
        pr_err("Ошибка: значение должно быть от 0 до 100!\n");
        return -EINVAL;
    }

    struct stack_entry* stk = stack_create();
    
    int error = 0;

    while(val)
    {
        if(*val == '(' || *val == '{' || *val == '[')
        {
            
        }
        else if(*val == ')' || *val == '}' || *val == ']')
        {
        }
        else
        {
            pr_err("The sequence must consist of characters (), [] or {}\n");
            error = -EINVAL;
        }
    };
    
    stack_destroy(stk);
    return error;
}


static const struct kernel_param_ops sequence_ops = {
    .set = sequence_set,  
    .get = sequence_get,
};


module_param_cb(brackets_sequence, &sequence_ops, &brackets_sequence, 0644);
MODULE_PARM_DESC(brackets_sequence, "Bracket sequence");


// Initialization 
static int __init mod_init(void) {
    pr_info("Module loaded: %d\n", my_param);
    return 0;
}

static void __exit mod_exit(void) {
    pr_info("Module unloaded\n");
}

module_init(mod_init);
module_exit(mod_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("varthlokkur");
MODULE_DESCRIPTION("Module to brackets sequence check");
