#ifndef LIST_H
#define LIST_H 1

struct list_item {
	struct list_item *next;
	void *value;
	int id;
};

struct list {
	struct list_item *head;
	int length;
};

struct list *list_New();
void list_Append(struct list *this, void *v);
void list_Delete(struct list *this, int id);
void list_Destroy(struct list *this);
void *list_Get(struct list *this, int id);
#endif
