#include <stdlib.h>
#include "list.h"

static int list_FirstID (struct list *this);
static struct list_item *list_item_New(int i, void *v);
static void list_item_Destroy(struct list_item *i);

/* public */

struct list *list_New ()
{
	struct list *this;
	this = malloc(sizeof(*this));
	this->head = NULL;
	this->length = 0;
	return this;
}

void list_Append (struct list *this, void *v)
{
	int i = list_FirstID(this);
	struct list_item **current = &this->head;
	while (*current) {
		current = &(*current)->next;
	}
	*current = list_item_New(i, v);
	this->length += 1;
}

void list_Delete (struct list *this, int id)
{
	struct list_item *head = this->head;
	struct list_item *prev = NULL;

	if (head->id == id) {
		this->head = head->next;
		goto destroy_item;
	}

	while (head && head->id != id) {
		prev = head;
		head = head->next;
	}

	if (!head)
		return;

	prev->next = head->next;
destroy_item:
	list_item_Destroy(head);
	this->length -= 1;
}

void list_Destroy(struct list *this)
{
	struct list_item *i = this->head;
	while (i) {
		struct list_item *n = i->next;
		list_item_Destroy(i);
		i = n;
	}
	free(this);
}

void *list_Get(struct list *this, int id)
{
	struct list_item *i = this->head;
	while (i) {
		if (i->id == id)
			return i->value;
		i = i->next;
	}
	return NULL;
}

/* private */

int list_FirstID (struct list *this)
{
	struct list_item *i = this->head;
	int a = 0;
	int b = -1;
	while (i) {
		if (i->id == a)
			a++;
		if (i->id > b)
			b = i->id;
		i = i->next;
	}
	return (a < b) ? a : b + 1;
}

struct list_item *list_item_New (int i, void *v)
{
	struct list_item *this;
	this = malloc(sizeof(*this));
	this->next = NULL;
	this->value = v;
	this->id = i;
	return this;
}

void list_item_Destroy (struct list_item *this)
{
	free(this);
}
