#include <stdio.h>

#define INITIAL_SUBJECT_NUMBER 7
#define ADVENT_DATE 20201227

unsigned long
solve(const unsigned long card_key, const unsigned long door_key) {
  unsigned long transformed = 1;
  unsigned long loop_size;

  for (loop_size = 0; transformed != card_key; loop_size++)
    transformed = transformed * INITIAL_SUBJECT_NUMBER % ADVENT_DATE;

  transformed = 1;

  while (loop_size--)
    transformed = transformed * door_key % ADVENT_DATE;

  return transformed;
}

int
main() {
  printf("%lu (14897079)\n", solve(5764801, 17807724));
  printf("%lu (11288669)\n", solve(5099500, 7648211));
  return 0;
}
