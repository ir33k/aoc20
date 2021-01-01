#include <stdio.h>
#include <assert.h>
#include "linked_list.h"

int
linked_list_log_all(struct linked_list * const linked_list,
                    FILE * target) {
  if (linked_list->size <= 0)
    return 0;

  struct linked_list_node * node = linked_list->head;
  size_t i;

  for (i=0; i<linked_list->size; i++, node = node->next)
    fprintf(target, "%2lu -> %d\n", i, *(int *)node->value);

  return 1;
}

void
init_and_destroy() {
  int value = 1;
  struct linked_list list;

  /* Init and destroy empty list */
  linked_list_init(&list);

  /*
   * NOTE Function return 0 because nothing was destroyed.  That is
   *      expected from empty list.
   */
  assert(0 == linked_list_destroy(&list, 0));

  assert(list.size == 0);

  /* Init and destroy list with single value */
  linked_list_init(&list);
  linked_list_push_value(&list, &value);

  assert(list.size == 1);

  assert(1 == linked_list_destroy(&list, 0));

  assert(list.size == 0);

  /* Init and destroy list with values */
  linked_list_init(&list);

  linked_list_push_value(&list, &value);
  linked_list_push_value(&list, &value);
  linked_list_push_value(&list, &value);
  linked_list_push_value(&list, &value);
  linked_list_push_value(&list, &value);

  assert(list.size == 5);

  assert(1 == linked_list_destroy(&list, 0));

  assert(list.size == 0);
}

void
head_and_tail() {
  int value = 1;
  struct linked_list list;

  /* Just initialized empty list don't have head and tail */
  linked_list_init(&list);

  assert(list.size == 0);
  assert(list.head == NULL);
  assert(list.tail == NULL);

  /* If list have single node its head and tail at the same time */
  linked_list_push_value(&list, &value);

  assert(list.size == 1);
  assert(list.head == list.tail);
  assert(list.head->next == NULL);
  assert(list.head->prev == NULL);

  /* If list have 2 nodes its just head and tail */
  linked_list_push_value(&list, &value);

  assert(list.head != list.tail);
  assert(list.head->next == list.tail);
  assert(list.tail->prev == list.head);
  assert(list.head->prev == NULL);
  assert(list.tail->next == NULL);

  /* Deleting head or tail nodes should update head and tail list
     pointers */
  linked_list_delete_by_index(&list, 0, 0);

  assert(list.size == 1);
  assert(list.head == list.tail);
  assert(list.head->next == NULL);
  assert(list.head->prev == NULL);

  /* Making list empty again should result in no head and tail */
  linked_list_delete_by_index(&list, 0, 0);

  assert(list.size == 0);
  assert(list.head == NULL);
  assert(list.tail == NULL);

  /* Test removing again with multiple values */
  linked_list_push_value(&list, &value);
  linked_list_push_value(&list, &value);
  linked_list_push_value(&list, &value);

  assert(list.size == 3);
  assert(list.head != list.tail);

  linked_list_delete_by_index(&list, 0, 0);
  linked_list_delete_by_index(&list, 0, 0);

  assert(list.size == 1);
  assert(list.head == list.tail);
  assert(list.head->next == NULL);
  assert(list.head->prev == NULL);

  linked_list_delete_by_index(&list, 0, 0);

  assert(list.size == 0);
  assert(list.head == NULL);
  assert(list.tail == NULL);

  linked_list_destroy(&list, 0);
}

void
push_value() {
  int values[6] = { 2, 4, 8, 16, 32, 64 };
  struct linked_list list;
  struct linked_list_node * prev;

  linked_list_init(&list);

  assert(list.size == 0);

  /* Push 5 values */
  linked_list_push_value(&list, &values[0]);
  linked_list_push_value(&list, &values[1]);
  linked_list_push_value(&list, &values[2]);
  linked_list_push_value(&list, &values[3]);
  linked_list_push_value(&list, &values[4]);

  assert(list.size == 5);
  assert(values[0] == *(int *)linked_list_get_value(&list, 0));
  assert(values[1] == *(int *)linked_list_get_value(&list, 1));
  assert(values[2] == *(int *)linked_list_get_value(&list, 2));
  assert(values[3] == *(int *)linked_list_get_value(&list, 3));
  assert(values[4] == *(int *)linked_list_get_value(&list, 4));

  /* Push more values after some node */
  prev = linked_list_get_node(&list, 2);

  linked_list_push_value_after(&list, &values[5], prev);
  linked_list_push_value_after(&list, &values[5], prev);
  linked_list_push_value_after(&list, &values[5], prev);

  assert(list.size == 8);
  assert(values[0] == *(int *)linked_list_get_value(&list, 0));
  assert(values[1] == *(int *)linked_list_get_value(&list, 1));
  assert(values[2] == *(int *)linked_list_get_value(&list, 2));
  assert(values[5] == *(int *)linked_list_get_value(&list, 3));
  assert(values[5] == *(int *)linked_list_get_value(&list, 4));
  assert(values[5] == *(int *)linked_list_get_value(&list, 5));
  assert(values[3] == *(int *)linked_list_get_value(&list, 6));
  assert(values[4] == *(int *)linked_list_get_value(&list, 7));

  linked_list_destroy(&list, 0);
}

void
push_node() {
  int values[8] = { 2, 4, 8, 16, 32, 64, 128, 256 };
  struct linked_list list;
  struct linked_list_node * prev;
  struct linked_list_node * nodes[8];

  for (int i = 0; i < 8; i++)
    nodes[i] = linked_list_node_create(&values[i]);

  linked_list_init(&list);

  assert(list.size == 0);

  /* Push 5 values */
  linked_list_push_node(&list, nodes[0]);
  linked_list_push_node(&list, nodes[1]);
  linked_list_push_node(&list, nodes[2]);
  linked_list_push_node(&list, nodes[3]);
  linked_list_push_node(&list, nodes[4]);

  assert(list.size == 5);
  assert(*(int *)nodes[0]->value == *(int *)linked_list_get_value(&list, 0));
  assert(*(int *)nodes[1]->value == *(int *)linked_list_get_value(&list, 1));
  assert(*(int *)nodes[2]->value == *(int *)linked_list_get_value(&list, 2));
  assert(*(int *)nodes[3]->value == *(int *)linked_list_get_value(&list, 3));
  assert(*(int *)nodes[4]->value == *(int *)linked_list_get_value(&list, 4));

  /* Push more values after some node */
  prev = linked_list_get_node(&list, 2);

  /*
   * WARNING Remember that each node have to be new.  If you put the
   *         same node in two different places in list it will cause
   *         problems. It's usually easier to add new values using
   *         push_value functions.
   */
  linked_list_push_node_after(&list, nodes[7], prev);
  linked_list_push_node_after(&list, nodes[6], prev);
  linked_list_push_node_after(&list, nodes[5], prev);

  assert(list.size == 8);
  assert(*(int *)nodes[0]->value == *(int *)linked_list_get_value(&list, 0));
  assert(*(int *)nodes[1]->value == *(int *)linked_list_get_value(&list, 1));
  assert(*(int *)nodes[2]->value == *(int *)linked_list_get_value(&list, 2));
  assert(*(int *)nodes[5]->value == *(int *)linked_list_get_value(&list, 3));
  assert(*(int *)nodes[6]->value == *(int *)linked_list_get_value(&list, 4));
  assert(*(int *)nodes[7]->value == *(int *)linked_list_get_value(&list, 5));
  assert(*(int *)nodes[3]->value == *(int *)linked_list_get_value(&list, 6));
  assert(*(int *)nodes[4]->value == *(int *)linked_list_get_value(&list, 7));

  linked_list_destroy(&list, 0);
}

int
main() {
  init_and_destroy();
  head_and_tail();
  push_value();
  push_node();
  /* TODO add more tests */

  return 0;
}
