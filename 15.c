#include <stdio.h>
#include <stdlib.h>

#define HASHMAP_SIZE 65536      /* hehehe ;D */

int input1[3] = { 0,3,6 };
int input2[3] = { 1,3,2 };
int input3[3] = { 2,1,3 };
int input4[3] = { 1,2,3 };
int input5[3] = { 2,3,1 };
int input6[3] = { 3,2,1 };
int input7[3] = { 3,1,2 };
int input8[6] = { 0,3,1,6,7,5 };

typedef struct {
  unsigned long long key;
  unsigned long long value;
} hashmap_item;

unsigned long long
hashmap_get_index(unsigned long long key) {
  return key % HASHMAP_SIZE;
}

void
hashmap_set(hashmap_item * hash_array[HASHMAP_SIZE],
            unsigned long long key, unsigned long long value) {
   hashmap_item * item = (hashmap_item*) malloc(sizeof(hashmap_item));

   item->key = key;
   item->value = value;  

   unsigned long long index = hashmap_get_index(key); /* get the hash  */

   /* move in array until an empty or deleted cell */
   while(hash_array[index] != NULL) { /* TODO && .->key!= -1 */
      ++index;                  /* go to next cell */
      index %= HASHMAP_SIZE;    /* wrap around the table */
   }
	
   hash_array[index] = item;
}

hashmap_item *
hashmap_get(hashmap_item * hash_array[HASHMAP_SIZE],
            unsigned long long key) {
  unsigned long long index = hashmap_get_index(key);

  /* move in array until an empty */
  while (hash_array[index] != NULL) {
    if (hash_array[index]->key == key) return hash_array[index];

    ++index;                    /* go to next cell */
    index %= HASHMAP_SIZE;      /* wrap around the table */
  }

  return NULL;
}

unsigned long long
solve1(int input[6], size_t length, size_t rounds) {
  hashmap_item * hashmap_numbers[HASHMAP_SIZE];
  hashmap_item * tmp_hashmap_item_pt;

  size_t round;
  unsigned long long tmp_value;
  unsigned long long number = input[length-1];

  for (round=1; round < length; round++)
    hashmap_set(hashmap_numbers, input[round-1], round);

  for (; round<rounds; round++) {
    tmp_hashmap_item_pt = hashmap_get(hashmap_numbers, number);

    if (tmp_hashmap_item_pt) {
      tmp_value = tmp_hashmap_item_pt->value;
      tmp_hashmap_item_pt->value = round;
      number = round - tmp_value;
    } else {
      hashmap_set(hashmap_numbers, number, round);
      number = 0;
    }
  }

  return number;
}

int
main() {
  printf("%lld (18)\n",   solve1(input1, 3, 22));
  /* printf("%lld (436)\n",  solve1(input1, 3, 2020)); */
  /* printf("%lld (1)\n",    solve1(input2, 3, 2020)); */
  /* printf("%lld (10)\n",   solve1(input3, 3, 2020)); */
  /* printf("%lld (27)\n",   solve1(input4, 3, 2020)); */
  /* printf("%lld (78)\n",   solve1(input5, 3, 2020)); */
  /* printf("%lld (438)\n",  solve1(input6, 3, 2020)); */
  /* printf("%lld (1836)\n", solve1(input7, 3, 2020)); */
  /* printf("%lld (852)\n",  solve1(input8, 6, 2020)); */

  /* printf("%lld (436)\n",  solve1(input1, 3, 30000000)); */

  return 0;
}
