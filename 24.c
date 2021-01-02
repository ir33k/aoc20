#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashmap.h"

#define MAX_LINE_LENGTH 64
#define MAX_POSITION_LENGTH 32
#define MAX_POSITIONS_COUNT 512

unsigned
solve1(const char file_name[6]) {
  FILE * file = fopen(file_name, "r");

  if (file == NULL) {
    fprintf(stderr, "It's a trap!  Does file exists?\n");
    exit(1);
  }

  unsigned result = 0;
  char line[MAX_LINE_LENGTH];
  int i,x,y;
  char position[MAX_POSITION_LENGTH];
  char positions[MAX_POSITIONS_COUNT][MAX_POSITION_LENGTH];
  int positions_count = 0;

  int tiles_state[2] = { 0, 1 };
  void * element;
  struct hashmap_s hashmap;

  if (0 != hashmap_create(2, &hashmap)) {
    fprintf(stderr, "ERROR: hashmap_create\n");
    exit(1);
  }

  while (fgets(line, sizeof line, file) != NULL) {
    i = x = y = 0;

    while (line[i] != '\n') {
      switch (line[i]) {
      case 'e': x -= 2; break;
      case 'w': x += 2; break;
      case 'n': y += 1; break;
      case 's': y -= 1; break;
      }

      if (line[i] == 'n' || line[i] == 's') {
        i++;

        switch (line[i]) {
        case 'e': x -= 1; break;
        case 'w': x += 1; break;
        }
      }

      i++;
    }

    sprintf(position, "%d,%d", x, y);
    element = hashmap_get(&hashmap, position, strlen(position));

    if (element == NULL) {
      strcpy(positions[positions_count], position);

      if (0 != hashmap_put(&hashmap, positions[positions_count],
                           strlen(position), &tiles_state[1])) {
        fprintf(stderr, "ERROR: hashmap_put %s\n", position);
        exit(1);
      }

      positions_count++;
      result++;
    } else {
      result--;
    }
  }

  fclose(file);
  hashmap_destroy(&hashmap);

  return result;
}

int
main() {
  printf("%u (10)\n", solve1("24i1"));
  printf("%u (332)\n", solve1("24i2"));

  return 0;
}
