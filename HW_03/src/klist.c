#include "klist.h"

#include <linux/list.h>
#include <linux/slab.h>

struct klist {
  void *data;
  klist_cbs_t cbs;
  struct list_head node;
};

klist_t *list_create(const klist_cbs_t *cbs) {
  klist_t *lhead;

  if (cbs == NULL)
    return NULL;

  lhead = kzalloc(sizeof(klist_t), GFP_KERNEL);
  if (!lhead)
    return NULL;

  lhead->data = NULL;
  INIT_LIST_HEAD(&lhead->node);

  lhead->cbs.klcopy = cbs->klcopy;
  lhead->cbs.klfree = cbs->klfree;

  return lhead;
}

void list_destroy(klist_t *head) {
  if (head == NULL)
    return;

  klist_t *element, *next;
  if (!list_empty(&head->node)) {
    list_for_each_entry_safe(element, next, &head->node, node) {
      list_del(&element->node);
      kfree(element);

      if (head->cbs.klfree != NULL) {
        head->cbs.klfree(element->data);
      }
    }
  }

  kfree(head);
}

size_t list_len(const klist_t *head) {

  struct list_head *pos;
  size_t count = 0;

  if (head == NULL)
    return 0;

  list_for_each(pos, &head->node) count++;

  return count;
}

void list_push_back(klist_t *head, void *data) {
  if (data == NULL || head == NULL) {
    return;
  }

  klist_t *element = kzalloc(sizeof(klist_t), GFP_KERNEL);
  if (head->cbs.klcopy != NULL) {
    element->data = head->cbs.klcopy(data);
  }
  else {
    element->data = data;
  }
  
  INIT_LIST_HEAD(&element->node);
  list_tail_add(&element->node, &head->node);
}

void list_push_front(klist_t*, void* data)
{
  if (data == NULL || head == NULL) {
    return;
  }

  klist_t *element = kzalloc(sizeof(klist_t), GFP_KERNEL);
  if (head->cbs.klcopy != NULL) {
    element->data = head->cbs.klcopy(data);
  }
  else {
    element->data = data;
  }
  
  INIT_LIST_HEAD(&element->node);
  list_add(&element->node, &head->node);
}

void list_iterate(const klist_t *head, elem_cb cb) {

  if (cb == NULL || head == NULL) {
    return;
  }

  klist_t *element = NULL;
  if (!list_empty(&head->node)) {
    list_for_each_entry(element, &head->node, node) {
        cb(element->data);
    }
  }
}