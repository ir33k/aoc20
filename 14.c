#include <stdio.h>
#include <string.h>

#define MASK_LENGTH 37

int
solve1(char file_name[8]) {
  FILE * file = fopen(file_name, "r");

  if (file == NULL) return -1;

  char line[64];
  char mask[MASK_LENGTH];
  unsigned long mem_index;
  unsigned long mem_value;
  char result[MASK_LENGTH];
  int i;

  while (fgets(line, sizeof line, file) != NULL) {
    printf(">>>> %s", line);
    if (line[1] == 'a') {
      sscanf(line, "mask = %s\n", mask);
    } else {
      sscanf(line, "mem[%lu] = %lu\n", &mem_index, &mem_value);

      i = 35;
      strcpy(result, mask);
      while (i >= 0) {
        if (result[i] == 'X') {
          result[i] = (mem_value % 2 > 0) ? '1' : '0';
        }

        mem_value /= 2;
        i--;
      }
      printf("%s\n", result);
    }
  }

  fclose(file);
  return 0;
}

int
main() {
  printf("%d (165)\n", solve1("14i1"));
  return 0;
}
