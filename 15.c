#include <stdio.h>

int input1[3] = { 0,3,6 };
int input2[3] = { 1,3,2 };
int input3[3] = { 2,1,3 };
int input4[3] = { 1,2,3 };
int input5[3] = { 2,3,1 };
int input6[3] = { 3,2,1 };
int input7[3] = { 3,1,2 };
int input8[6] = { 0,3,1,6,7,5 };

typedef struct {
  size_t key;
  int value;
} key_value_int_pair;

typedef struct {
  key_value_int_pair items[2048];
  size_t size;
} map_int;

int
map_int_has(map_int * map, size_t key) {
  for (size_t i=0; i<map->size; i++)
    if (map->items[i].key == key) return 1;

  return 0;
}

key_value_int_pair *
map_int_get_item(map_int * map, size_t key) {
  for (size_t i=0; i<map->size; i++)
    if (map->items[i].key == key)
      return &map->items[i];

  return NULL;
}

key_value_int_pair *
map_int_set_value(map_int * map, size_t key, int value) {
  for (size_t i=0; i<map->size; i++)
    if (map->items[i].key == key) {
      map->items[i].value = value;
      return &map->items[i];
    }

  map->items[map->size].key = key;
  map->items[map->size].value = value;
  map->size++;

  return &map->items[map->size-1];
}

int
solve1(int input[6], size_t length, size_t rounds) {
  size_t round;
  map_int numbers = { .size = 0 };
  int tmp_value;
  int number = input[length-1];

  for (round=1; round < length; round++)
    map_int_set_value(&numbers, input[round-1], round);

  for (; round<rounds; round++) {
    if (map_int_has(&numbers, number)) {
      tmp_value = map_int_get_item(&numbers, number)->value;
      map_int_set_value(&numbers, number, round);
      number = round - tmp_value;
    } else {
      map_int_set_value(&numbers, number, round);
      number = 0;
    }
  }

  return number;
}

int
main() {
  printf("%d (436)\n",  solve1(input1, 3, 2020));
  printf("%d (1)\n",    solve1(input2, 3, 2020));
  printf("%d (10)\n",   solve1(input3, 3, 2020));
  printf("%d (27)\n",   solve1(input4, 3, 2020));
  printf("%d (78)\n",   solve1(input5, 3, 2020));
  printf("%d (438)\n",  solve1(input6, 3, 2020));
  printf("%d (1836)\n", solve1(input7, 3, 2020));
  printf("%d (852)\n",  solve1(input8, 6, 2020));

  return 0;
}
