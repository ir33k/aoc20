#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"

#define SOLVE2_VALUES_COUNT 1000000

int
linked_list_log_all(struct linked_list * const linked_list,
                    FILE * target) {
  if (linked_list->size <= 0)
    return 0;

  struct linked_list_node * node = linked_list->head;
  size_t i = 0;

  while (node) {
    fprintf(target, "%2lu -> %d\n", i++, *(unsigned int *)node->value);
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
  unsigned int destination_node_value;

  destination_node_value = *(unsigned int *)current_node->value - 1;

 again: node = pulled_cups->head;

  if (destination_node_value < 1)
    destination_node_value = 9; /* TODO customize max value */

  while (node != NULL) {
    if (*(unsigned int *)node->value == destination_node_value) {
      destination_node_value -= 1;
      goto again;
    }

    node = node->next;
  }

  node = cups->head;

  while (*(unsigned int *)node->value != destination_node_value)
    node = get_next_cup(cups, node);

  return node;
}

struct linked_list_node *
get_destination_cup_from_list(struct linked_list * const pulled_cups,
                              struct linked_list_node ** const all_cups,
                              struct linked_list_node * const current_node) {
  struct linked_list_node * node;
  unsigned int destination_node_value;

  destination_node_value = *(unsigned int *)current_node->value - 1;

 again: node = pulled_cups->head;

  if (destination_node_value < 1)
    destination_node_value = 1000000;

  while (node != NULL) {
    if (*(unsigned int *)node->value == destination_node_value) {
      destination_node_value -= 1;
      goto again;
    }

    node = node->next;
  }

  return all_cups[destination_node_value];
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
  unsigned int values[9] = { 0 };
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

  while (*(unsigned int *)current_cup->value != 1)
    current_cup = get_next_cup(&cups, current_cup);

  destination_cup = current_cup;
  current_cup = get_next_cup(&cups, current_cup);

  for (i=0; current_cup != destination_cup; i++) {
    result[i] = *(unsigned int *)current_cup->value + '0';
    current_cup = get_next_cup(&cups, current_cup);
  }
  result[i] = 0;

  linked_list_destroy(&cups, 0);
  linked_list_destroy(&pulled_cups, 0);

  return result;
}

unsigned long long
solve2(char input[10], unsigned long moves_count) {
  struct linked_list cups;
  struct linked_list pulled_cups;
  struct linked_list_node * pulled_cup;
  struct linked_list_node * current_cup;
  struct linked_list_node * destination_cup;
  struct linked_list_node ** all_cups;
  unsigned int * values;
  const int pulled_cups_count = 3;
  size_t i;
  unsigned long long result = 1;

  all_cups = malloc(SOLVE2_VALUES_COUNT * sizeof(struct linked_list_node *));

  for (i=0; i<SOLVE2_VALUES_COUNT; i++)
    all_cups[i] = malloc(sizeof(struct linked_list_node *));
  
  values = malloc(SOLVE2_VALUES_COUNT * sizeof(unsigned int *));

  i = 0;
  while (input[i] != 0) {
    values[i] = (unsigned int)input[i]-'0';
    all_cups[values[i]] = linked_list_node_create(&values[i]);
    i++;
  }

  while (i < SOLVE2_VALUES_COUNT) {
    values[i] = i+1;
    all_cups[values[i]] = linked_list_node_create(&values[i]);
    i++;
  }

  linked_list_init(&cups);
  linked_list_init(&pulled_cups);

  for (i=0; i<SOLVE2_VALUES_COUNT; i++)
    linked_list_push_node(&cups, all_cups[values[i]]);

  current_cup = cups.head;

  for (i=0; i < moves_count; i++) {
    pull_after_node_to_other_list(&cups, &pulled_cups, current_cup,
                                  pulled_cups_count);

    destination_cup = get_destination_cup_from_list(&pulled_cups, all_cups,
                                                    current_cup);

    while (pulled_cups.tail != NULL) {
      pulled_cup = linked_list_pull_by_node(&pulled_cups, pulled_cups.tail);
      linked_list_push_node_after(&cups, pulled_cup, destination_cup);
    }

    current_cup = get_next_cup(&cups, current_cup);
  }

  current_cup = cups.head;

  while (*(unsigned int *)current_cup->value != 1)
    current_cup = get_next_cup(&cups, current_cup);

  current_cup = get_next_cup(&cups, current_cup);
  result *= *(unsigned int *)current_cup->value;

  current_cup = get_next_cup(&cups, current_cup);
  result *= *(unsigned int *)current_cup->value;


  linked_list_destroy(&cups, 0);
  linked_list_destroy(&pulled_cups, 0);
  free(all_cups);
  free(values);

  return result;
}

int
main() {
  char result[64];
  
  printf("%s (92658374)\n", solve1(result, "389125467", 10));
  printf("%s (67384529)\n", solve1(result, "389125467", 100));
  printf("%s (98752463)\n", solve1(result, "789465123", 100));

  printf("%llu (149245887792)\n", solve2("389125467", 10000000));
  printf("%llu (2000455861)\n",   solve2("789465123", 10000000));

  return 0;
}
