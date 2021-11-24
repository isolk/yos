#include "linked_list.h"
#include "page.h"

linked_list *new_linked_list()
{
	linked_list *t = kalloc(sizeof(linked_list));
	return t;
}

void push_data(linked_list *l, void *dt)
{
	if (l->head == NULL)
	{
		l->head = kalloc(sizeof(linked_node));
		l->head->data = dt;
		l->head->next = NULL;
		return;
	}

	linked_node *it = l->head;
	while (it->next != NULL)
	{
		it = it->next;
	}
	it->next = kalloc(sizeof(linked_node));
	it->next->data = dt;
	it->next->next = NULL;
}