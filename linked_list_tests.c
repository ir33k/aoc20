#include <stdio.h>
#include <assert.h>
#include "linked_list.h"

int
linked_list_log_all(struct linked_list * const linked_list,
                    FILE * target) {
  if (linked_list->size <= 0)
    return 0;

  struct linked_list_node * node = linked_list->head;
  size_t i = 0;

  while (node) {
    fprintf(target, "%2lu -> %d\n", i++, *(int *)node->value);
    node = node->next;
  }

  return 1;
}

int
main() {
  int values[5] = { 2, 4, 8, 16, 32 };
  struct linked_list list;
  struct linked_list_node * node;

  linked_list_init(&list);

  assert(list.size == 0);

  linked_list_push(&list, &values[0]);

  assert(list.head == list.tail);
  
  linked_list_push(&list, &values[1]);
  linked_list_push(&list, &values[2]);
  linked_list_push(&list, &values[3]);
  linked_list_push(&list, &values[4]);

  assert(list.size == 5);
  assert(values[0] == *(int *)linked_list_get_value(&list, 0));
  assert(values[1] == *(int *)linked_list_get_value(&list, 1));
  assert(values[2] == *(int *)linked_list_get_value(&list, 2));
  assert(values[3] == *(int *)linked_list_get_value(&list, 3));
  assert(values[4] == *(int *)linked_list_get_value(&list, 4));

  assert(linked_list_delete_by_index(&list, 1, 0));
  assert(linked_list_delete_by_index(&list, 1, 0));

  assert(list.size == 3);
  assert(values[0] == *(int *)linked_list_get_value(&list, 0));
  assert(values[3] == *(int *)linked_list_get_value(&list, 1));
  assert(values[4] == *(int *)linked_list_get_value(&list, 2));

  node = linked_list_get_node(&list, 0);

  linked_list_push_after(&list, &values[2], node);
  linked_list_push_after(&list, &values[1], node);

  assert(list.size == 5);
  assert(values[0] == *(int *)linked_list_get_value(&list, 0));
  assert(values[1] == *(int *)linked_list_get_value(&list, 1));
  assert(values[2] == *(int *)linked_list_get_value(&list, 2));
  assert(values[3] == *(int *)linked_list_get_value(&list, 3));
  assert(values[4] == *(int *)linked_list_get_value(&list, 4));

  linked_list_delete_by_node(&list, list.tail, 0);
  linked_list_delete_by_node(&list, list.head, 0);

  assert(list.head == linked_list_get_node(&list, 0));
  assert(list.tail == linked_list_get_node(&list, list.size-1));

  assert(list.head->prev == NULL);
  assert(list.tail->next == NULL);

  assert(list.size == 3);
  assert(values[1] == *(int *)linked_list_get_value(&list, 0));
  assert(values[2] == *(int *)linked_list_get_value(&list, 1));
  assert(values[3] == *(int *)linked_list_get_value(&list, 2));

  assert(linked_list_delete_by_index(&list, 1, 0) == 1);
  assert(linked_list_delete_by_index(&list, 1, 0) == 1);

  assert(list.size == 1);
  assert(list.head == list.tail);

  linked_list_log_all(&list, stdout);

  linked_list_destroy(&list, 0);

  return 0;
}
