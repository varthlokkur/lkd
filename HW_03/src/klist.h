#ifndef __KLIST_HEADER__
#define __KLIST_HEADER__

#include <linux/stat.h>

// Kernel list interface
struct klist;
typedef struct klist klist_t;

typedef void* (*copy_cb)(void *);
typedef void (*free_cb)(void *);
typedef void (*elem_cb)(void *);

struct klist_cbs{
    copy_cb klcopy;
    free_cb klfree;
};

typedef struct klist_cbs klist_cbs_t;

// create and destory object
klist_t *list_create(const klist_cbs_t *cbs);
void list_destroy(klist_t*);

// operations
size_t list_len(const klist_t*);
void list_push_back(klist_t*, void* data);
void list_push_front(klist_t*, void* data);

void list_iterate(const klist_t*, elem_cb);

#endif // __KLIST_HEADER__