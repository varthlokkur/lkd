#ifndef __STACK_H__
#define __STACK_H__

#include <linux/list.h>

struct stack_entry;

struct stack_entry* stack_create(void);

void stack_destroy(struct stack_entry *stack);

void stack_push(struct stack_entry *stack, void *);

void* stack_pop(struct stack_entry *stack);

bool is_stack_empty(struct stack_entry *stack);

#endif // __STACK_H__

