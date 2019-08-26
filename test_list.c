#include "minunit.h"
#include "list.h"

static char *test_New ()
{
	struct list *l = list_New();
	mu_assert("test_New: head should be NULL", l->head == NULL);
	mu_assert("test_New: length should be 0", l->length == 0);
	return 0;
}

static char *test_Append ()
{
	int i = 3;
	int i2 = 5;
	int i3 = 7;
	struct list *l = list_New();
	list_Append(l, &i);
	mu_assert("test_New: length should be 1", l->length == 1);
	list_Append(l, &i2);
	mu_assert("test_New: length should be 1", l->length == 2);
	list_Append(l, &i3);
	mu_assert("test_New: length should be 1", l->length == 3);

	struct list_item *item;
	item = l->head;
	mu_assert("test_Append: head should not be NULL", item != NULL);
	mu_assert("test_Append: head.id should be 0", item->id == 0);
	mu_assert(
			"list_Append: head.value should be ptr to 3",
			*(int *)item->value == i);

	item = item->next;
	mu_assert("test_Append: item1 should not be NULL", item != NULL);
	mu_assert("test_Append: item1.id should be 1", item->id == 1);
	mu_assert(
			"test_Append: item1.value should be ptr to 3",
			*(int *)item->value == i2);

	item = item->next;
	mu_assert("test_Append: item2 should not be NULL", item != NULL);
	mu_assert("test_Append: item2.id should be 2", item->id == 2);
	mu_assert(
			"test_Append: item2.value should equal i3",
			*(int *)item->value == i3);

	item = item->next;
	mu_assert("test_Append: item3 should be NULL", item == NULL);
	return 0;
}

static char *test_DeleteHead ()
{
	int i = 2;
	int i2 = 3;
	int i3 = 5;
	struct list *l = list_New();
	list_Append(l, &i);
	list_Append(l, &i2);
	list_Append(l, &i3);
	list_Delete(l, 0);
	mu_assert("test_New: length should be 1", l->length == 2);

	struct list_item *item;
	item = l->head;
	mu_assert("test_DeleteHead: head should not be NULL", item != NULL);
	mu_assert("test_DeleteHead: head.id should be 1", item->id == 1);
	mu_assert(
			"test_DeleteHead: *head.value should equal i2",
			*(int *)item->value == i2);

	item = item->next;
	mu_assert("test_DeleteHead: item1 should not be NULL", item != NULL);
	mu_assert("test_DeleteHead: item1.id should be 2", item->id == 2);
	mu_assert(
			"test_DeleteHead: *item1.value should equal i3",
			*(int *)item->value == i3);

	item = item->next;
	mu_assert("test_DeleteHead: item2 should be NULL", item == NULL);
	return 0;
}

static char *test_DeleteMiddle ()
{
	int i = 2;
	int i2 = 3;
	int i3 = 5;
	struct list *l = list_New();
	list_Append(l, &i);
	list_Append(l, &i2);
	list_Append(l, &i3);
	list_Delete(l, 1);

	struct list_item *item;
	item = l->head;
	mu_assert("test_DeleteMiddle: head should not be NULL", item != NULL);
	mu_assert("test_DeleteMiddle: head.id should be 0", item->id == 0);
	mu_assert(
			"test_DeleteMiddle: *head.value should equal i",
			*(int *)item->value == i);

	item = item->next;
	mu_assert("test_DeleteMiddle: item1 should not be NULL", item != NULL);
	mu_assert("test_DeleteMiddle: item1.id should be 2", item->id == 2);
	mu_assert(
			"test_DeleteMiddle: *item1.value should equal i3",
			*(int *)item->value == i3);

	item = item->next;
	mu_assert("test_DeleteMiddle: item2 should be NULL", item == NULL);
	return 0;
}

static char *test_Destroy ()
{
	int i = 2;
	struct list *l = list_New();
	list_Append(l, &i);
	list_Destroy(l);
	return 0;
}


const char *all_tests ()
{
	mu_run_test(test_New);
	mu_run_test(test_Append);
	mu_run_test(test_DeleteHead);
	mu_run_test(test_DeleteMiddle);
	mu_run_test(test_Destroy);
	return 0;
}
