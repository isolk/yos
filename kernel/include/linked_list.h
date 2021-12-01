#include <stdint-gcc.h>
#include <stddef.h>
typedef struct linked_node
{
	void *data;
	struct linked_node *next;
	struct linked_node *prev;
} linked_node;

linked_node *new_list();

void append_data(linked_node *l, void *data);

void delete_node(linked_node *l);