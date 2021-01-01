#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdlib.h>

struct linked_list_node {
  void * value;
  struct linked_list_node * prev;
  struct linked_list_node * next;
};

struct linked_list {
  struct linked_list_node * head;
  struct linked_list_node * tail;
  size_t size;
};

void
linked_list_init(struct linked_list * const linked_list) {
  linked_list->head = NULL;
  linked_list->tail = NULL;
  linked_list->size = 0;
}

void
linked_list_destroy(struct linked_list * const linked_list,
                    const int free_values) {
  struct linked_list_node * current = linked_list->head;
  struct linked_list_node * next = current->next;

  size_t i = linked_list->size;

  while (i--) {
    if (free_values)
      free(current->value);

    free(current);
    current = next;

    if (current != NULL)
      next = current->next;
  }
}

struct linked_list_node *
linked_list_push_after(struct linked_list * const linked_list,
                       void * const value,
                       struct linked_list_node * const prev) {
  struct linked_list_node * node = malloc(sizeof(struct linked_list_node));

  node->value = value;
  node->prev = prev;
  node->next = prev->next;

  if (prev->next != NULL)
    prev->next->prev = node;

  prev->next = node;
  linked_list->size += 1;

  return node;
}

void
linked_list_push(struct linked_list * const linked_list,
                 void * const value) {
  if (linked_list->tail) {
    linked_list->tail = linked_list_push_after(linked_list, value,
                                               linked_list->tail);
  } else {
    struct linked_list_node * node = malloc(sizeof(struct linked_list_node));

    node->value = value;
    node->next = NULL;
    node->prev = NULL;

    linked_list->head = node;
    linked_list->tail = node;
    linked_list->size = 1;
  }
}

void *
linked_list_get_node(struct linked_list * const linked_list,
                     const size_t index) {
  if (index >= linked_list->size)
    return NULL;

  struct linked_list_node * node = linked_list->head;
  size_t i = index;

  while (i--)
    node = node->next;

  return node;
}

void *
linked_list_get_value(struct linked_list * const linked_list,
                      const size_t index) {
  struct linked_list_node * node;
  node = linked_list_get_node(linked_list, index);
  return node ? node->value : NULL;
}

void
linked_list_delete_by_node(struct linked_list * const linked_list,
                           struct linked_list_node * const node,
                           const int free_value) {
  /* Its a head */
  if (node->prev == NULL) {
    linked_list->head = node->next;
    linked_list->head->prev = NULL;
  } else {
    node->prev->next = node->next;
  }

  /* Its a tail */
  if (node->next == NULL) {
    linked_list->tail = node->prev;
    linked_list->tail->next = NULL;
  } else {
    node->next->prev = node->prev;
  }

  if (free_value)
    free(node->value);

  free(node);

  linked_list->size -= 1;
}

int
linked_list_delete_by_index(struct linked_list * const linked_list,
                            const size_t index,
                            const int free_value) {
  if (index >= linked_list->size)
    return 0;

  struct linked_list_node * node;
  node = linked_list_get_node(linked_list, index);

  if (!node)
    return 0;

  linked_list_delete_by_node(linked_list, node, free_value);
  return 1;
}

#endif /* LINKED_LIST_H */
