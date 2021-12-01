#include "linked_list.h"
#include "page.h"
#include "pm.h"
linked_node *new_list()
{
	linked_node *r = kalloc(sizeof(linked_node));
	r->data = NULL;
	r->next = r;
	r->prev = r;
	return r;
}

void append_data(linked_node *l, void *data)
{
	linked_node *n = kalloc(sizeof(linked_node));
	n->data = data;

	linked_node *next = l->next;

	l->next = n;
	n->prev = l;

	next->prev = n;
	n->next = next;
}

void delete_node(linked_node *l)
{
	linked_node *prev = l->prev;
	prev->next = l->next;
	l->next->prev = prev;
	kfree(l);
}