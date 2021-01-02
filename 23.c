#include <stdio.h>
#include <stdlib.h>
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

struct linked_list_node *
get_next_cup(struct linked_list * const list,
             struct linked_list_node * const current_node) {
  return current_node->next == NULL ? list->head : current_node->next;
}

struct linked_list_node *
pull_next_node(struct linked_list * const list,
               struct linked_list_node * const node) {
  return linked_list_pull_by_node(list, get_next_cup(list, node));
}

void
pull_after_node_to_other_list(struct linked_list * const source,
                              struct linked_list * const destination,
                              struct linked_list_node * const node,
                              unsigned short count) {
  struct linked_list_node * next_node;

  /* clear destination list */
  while (destination->tail != NULL)
    linked_list_delete_by_node(destination, destination->tail, 0);

  for (unsigned short i = 0; i < count; i++) {
    next_node = pull_next_node(source, node);
    linked_list_push_node(destination, next_node);
  }
}

struct linked_list_node *
get_destination_cup(struct linked_list * const cups,
                    struct linked_list * const pulled_cups,
                    struct linked_list_node * const current_node) {
  struct linked_list_node * node;
  int destination_node_value = *(int *)current_node->value - 1;

 again: node = pulled_cups->head;

  if (destination_node_value < 1)
    destination_node_value = 9;

  while (node != NULL) {
    if (*(int *)node->value == destination_node_value) {
      destination_node_value -= 1;
      goto again;
    }

    node = node->next;
  }

  node = cups->head;

  while (*(int *)node->value != destination_node_value)
    node = get_next_cup(cups, node);

  return node;
}

char *
solve1(char * result, char input[10], unsigned int moves_count) {
  /* printf("input: %s\n", input); */
  /* printf("moves_count: %d\n", moves_count); */

  struct linked_list cups;
  struct linked_list pulled_cups;
  struct linked_list_node * pulled_cup;
  struct linked_list_node * current_cup;
  struct linked_list_node * destination_cup;
  int values[9] = { 0 };
  unsigned int values_count = 9;
  const int pulled_cups_count = 3;
  size_t i;

  i = 0;
  while (input[i] != 0) {
    values[i] = (int)input[i]-'0';
    i++;
  }

  linked_list_init(&cups);
  linked_list_init(&pulled_cups);

  for (i=0; i<values_count; i++)
    linked_list_push_value(&cups, &values[i]);

  current_cup = cups.head;

  for (i=0; i < moves_count; i++) {
    pull_after_node_to_other_list(&cups, &pulled_cups, current_cup,
                                  pulled_cups_count);

    destination_cup = get_destination_cup(&cups, &pulled_cups,
                                          current_cup);

    while (pulled_cups.tail != NULL) {
      pulled_cup = linked_list_pull_by_node(&pulled_cups, pulled_cups.tail);
      linked_list_push_node_after(&cups, pulled_cup, destination_cup);
    }

    current_cup = get_next_cup(&cups, current_cup);
  }

  current_cup = cups.head;

  while (*(int *)current_cup->value != 1)
    current_cup = get_next_cup(&cups, current_cup);

  for (i=0; result[i] != 0; i++) {
    current_cup = get_next_cup(&cups, current_cup);
    result[i] = *(int *)current_cup->value + '0';
  }

  linked_list_destroy(&cups, 0);
  linked_list_destroy(&pulled_cups, 0);

  return result;
}

int
main() {
  char result[9] = "00000000";
  
  printf("%s (92658374)\n", solve1(result, "389125467", 10));
  printf("%s (67384529)\n", solve1(result, "389125467", 100));
  printf("%s (98752463)\n", solve1(result, "789465123", 100));

  return 0;
}
