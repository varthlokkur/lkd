#include "klist.h"
#define pr_fmt(fmt) "ex_list: " fmt

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/stat.h>
#include <linux/string.h>

/*typedef int element_t;
struct s_list
{
    element_t data;
    struct list_head node;
};

typedef struct s_list t_list;

static void list_destroy(struct list_head *head)
{
    t_list* element, *next;
    if (!list_empty(head))
    {
        list_for_each_entry_safe(element, next, head, node) {
            list_del(&element->node);
            kfree(element);
        }
    }
}

static void list_push_back(struct list_head *head, element_t data)
{
    t_list *element = kzalloc(sizeof(t_list), GFP_KERNEL);
    if (element == NULL) {
        pr_err("Can't allocate memory to list node\n");
        return;
    }
    element->data = data;
    INIT_LIST_HEAD(&element->node);
    list_add(&element->node, head);
}

static void list_push_front(struct list_head *list)
{
    kfree(list);
}

static void list_pop_back(struct list_head *head)
{
    t_list *tmp = NULL;
    if (!list_empty(head))
    {
        tmp = list_entry(head->next, struct s_list, node);
        list_del(head->next);
        kfree(tmp);
    }
}

static void list_pop_front(struct list_head *list)
{
    kfree(list);
}

static size_t list_size(struct list_head *head)
{
        struct list_head *pos;
        size_t count = 0;

        list_for_each(pos, head)
                count++;

        return count;
}

static void print_list(struct list_head *head)
{
    t_list *element = NULL;
    if (list_empty(head)) {
        pr_info("List is empty\n");
    }
    else {
        pr_info("List contains %d nodes", list_size(head));
        list_for_each_entry(element, head, node)
            pr_info("Data: %d\n", element->data);
    }
}*/

static void *copy_int(void *data) { 
    return data;
}

static void free_int(void *) {
  // Nothing to do
}

static void print_element(void *data) { 
    pr_info("Element: %d\n", (int)data); 
}

static const klist_cbs_t int_ops = {
    .klcopy = copy_int,
    .klfree = free_int,
};

static void *copy_string(void *data) { 
    const char* ch_data = (const char*)(data);
    size_t size = strlen(ch_data);

    char* new_data = kzalloc(sizeof(size + 1), GFP_KERNEL);
    strcpy(new_data, ch_data);
    return new_data; 
}

static void free_string(void * data) {
  kfree(data);
}

static void print_string(void *data) { 
    pr_info("Element: %s\n", (char*)data); 
}

static const klist_cbs_t string_ops = {
    .klcopy = copy_string,
    .klfree = free_string,
};

// Initialization
static int __init mod_init(void) {
  pr_info("init\n");

  klist_t *ltop = list_create(&int_ops);
  if (ltop == NULL) {
    pr_err("Cannot create integer list\n");
  }
  pr_info("List integer created\n");

  for (int i = 0; i < 3; ++i) {
    list_push_back(ltop, (void *)i);
    pr_info("List integer add element %d\n", i);
  }

  list_iterate(ltop, print_element);
  list_destroy(ltop);
  pr_info("List integer destroyed\n");


  klist_t *sltop = list_create(&string_ops);
  if (sltop == NULL) {
    pr_err("Cannot create string list\n");
  }
  pr_info("List string created\n");

  for (int i = 0; i < 3; ++i) {
    list_push_back(sltop, "aaaaa");
    pr_info("List string add element %d\n", i);
  }

  list_iterate(sltop, print_string);
  list_destroy(sltop);
  pr_info("List string destroyed\n");

  /*struct list_head top_list;

  INIT_LIST_HEAD(&top_list);
  print_list(&top_list);

  for (int i = 0; i < 3; ++i)
  {
      list_push_back(&top_list, i);
  }

  print_list(&top_list);

  list_pop_back(&top_list);
  print_list(&top_list);

  list_pop_back(&top_list);
  print_list(&top_list);

  list_pop_back(&top_list);
  print_list(&top_list);

  //t_list tmp = list_entry(top_list.next, struct s_list, nodes);

  list_destroy(&top_list);
  */

  return 0;
}

static void __exit mod_exit(void) { pr_info("exit\n"); }

module_init(mod_init);
module_exit(mod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("varthlokkur");
MODULE_DESCRIPTION("Module HW 03 list");
