#include "lib/kernel/list.h"
#include "lib/stdio.h"
#include "type.h"
#include "debug.h"
#include "lib/kernel/test.h"

void 
list_init (struct list* list)
{
    if (list == NULL)   return;

    list->head.prev = NULL;
    list->head.next = &list->tail;
    list->tail.prev = &list->head;
    list->tail.next = NULL;
};

void 
list_push_back (struct list* list, struct list_elem* e)
{
    if (list == NULL)   return;

    list_insert_before (&list->tail, e);    
};

void 
list_push_front (struct list* list, struct list_elem* e)
{
    if (list == NULL)   return;    

    list_insert_after (&list->head, e);
};

struct list_elem* 
list_pop_back (struct list* list)
{
    if (list == NULL)   return NULL;    

    struct list_elem* e = list_back (list);
	ASSERT (e->prev != NULL);
    list_remove (e->prev);
    return e;
};

struct list_elem* 
list_pop_front (struct list* list)
{
    if (list == NULL)   return NULL;

    struct list_elem* e = list_front (list);
    list_remove (e);
    return e;
};

void 
list_insert_before (struct list_elem* elem, struct list_elem* data)
{
    if (elem == NULL || data == NULL)   return;
    ASSERT (elem->prev != NULL);
    
    struct list_elem* prev = elem->prev;    
    data->prev = prev;
    data->next = elem;
    elem->prev = data;
    prev->next = data;
};

void 
list_insert_after (struct list_elem* elem, struct list_elem* data)
{
    if (elem == NULL || data == NULL)   return;    
    ASSERT (elem->next != NULL);

    struct list_elem* next = elem->next;
    data->prev = elem;
    data->next = next;
    elem->next = data;
    next->prev = data;
};

void 
list_remove (struct list_elem* elem)
{
    if (elem == NULL)   return;
    if (elem->next == NULL)	return;
    if (elem->prev == NULL)	return;

    elem->next->prev = elem->prev;
    elem->prev->next = elem->next;
	elem->next = NULL;
	elem->prev = NULL;
};

struct list_elem* 
list_front (struct list* list)
{
    if (list == NULL)   return NULL;   

    return list->head.next;
};

struct list_elem* 
list_back (struct list* list)
{
    if (list == NULL)   return NULL;

    return &list->tail;
};

struct list_elem* 
list_head (struct list* list)
{
    return list_front (list);
};

struct list_elem* 
list_tail (struct list* list)
{
    return list_back (list);    
};

struct list_elem* 
list_begin (struct list* list)
{
    return list_front (list);
};

struct list_elem* 
list_end (struct list* list)
{
    return list_back (list);
};

struct list_elem* 
list_next (struct list_elem* e)
{
    if (e == NULL)  return NULL;

    return e->next;    
};

struct list_elem*
list_prev (struct list_elem* e)
{
    if (e == NULL)  return NULL;

    return e->prev;
};

bool 
list_empty (struct list* list)
{
    ASSERT (list != NULL);

    return list_front (list) == list_back (list);
};

struct list_elem* 
list_find (struct list* list, struct list_elem* elem, 
        list_compare_func* func, void* aux)
{
    struct list_elem* e;
    uint32 val;

    ASSERT (list != NULL);    
    ASSERT (elem != NULL);
    ASSERT (func != NULL);
    
    for (e = list_head (list); e != list_tail (list); e = list_next (e))
    {
        val = func (e, elem, aux);

        if (val == 0)
            return e;
    }

    return NULL;
};


void
list_insert_ordered (struct list* list, struct list_elem* elem, 
        list_compare_func* func, void* aux)
{
    if (list == NULL || elem == NULL || func == NULL)   return;

    struct list_elem* e;
    for (e = list_front (list); e != list_back (list); e = list_next (e))
    {
        int val = func (e, elem, aux);
        if (val >= 0)
        {
            list_insert_before (e, elem);
            return;
        }
    }
    list_push_back (list, elem);
};

void 
list_print (struct list* list, list_print_func* func)
{
	struct list_elem* e;

	ASSERT (list != NULL);
	ASSERT (func != NULL);

	for (e = list_head (list); e != list_tail (list); e = list_next (e))
	{
//        framebuf_printf ("--------%p\n", e);
//		framebuf_printf ("Next: %p\n", e->next);
//		framebuf_printf ("Prev: %p\n", e->prev);
		func (e);
//		framebuf_printf ("\n");
	}
};
