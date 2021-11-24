#include <stdint-gcc.h>
#include <stddef.h>
typedef struct linked_node
{
	void *data;
	struct linked_node *next;
} linked_node;

typedef struct linked_list
{
	uint8_t count;
	linked_node *head;
} linked_list;

linked_list *new_linked_list();

void push_data(linked_list *l, void *dt);

void delete_node(linked_list *l, linked_node *node);