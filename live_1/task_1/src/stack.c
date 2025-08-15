#include "stack.h"

struct stack_entry 
{
    struct list_head list;
    void *data;
};

struct stack_entry* stack_create(void)
{
    struct stack_entry *stack;

	stack = kzalloc(sizeof(*stack), GFP_KERNEL);
	if (!stack)
		return NULL;
		
	stack->data = NULL;
	INIT_LIST_HEAD(&stack->list);
	
	return ec;
}

void stack_destroy(struct stack_entry *stack)
{
    kfree(stack);
}

void stack_push(struct stack_entry *stack, void* data)
{
    struct list_head *ptr;
    struct stack_entry *entry;

    list_for_each(ptr, &entry) 
    {
        entry = list_entry(ptr, struct todo_struct, list);

        if (entry->priority < new->priority) {

            list_add_tail(&new->list, ptr);

            return;

        }

    }

    list_add_tail(&new->list, &todo_list)
}

void* stack_pop(struct stack_entry *stack)
{
    
}

bool is_stack_empty(struct stack_entry *stack)
{
    return stack->data == NULL
}

void stack_push(struct stack_entry *stack, void* data) 
{

}
