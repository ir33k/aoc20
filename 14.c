#include <stdio.h>
#include <string.h>
#include <math.h>

#define MASK_LENGTH 36

void
print_36bin(unsigned long long number) {
  char result[MASK_LENGTH+1];
  unsigned long long initial_number = number;

  sprintf(result, "000000000000000000000000000000000000");
  for (int i=0; i<MASK_LENGTH; i++) {
    if (number % 2 > 0) result[MASK_LENGTH - 1 - i] = '1';
    number /= 2;
  }

  printf("%s (decimal %llu)\n", result, initial_number);
}

unsigned long long
solve1(char file_name[8]) {
  FILE * file = fopen(file_name, "r");

  if (file == NULL) return -1;

  char line[64];
  char mask[MASK_LENGTH+1];
  unsigned long long mem_index;
  unsigned long long mem_value;
  unsigned long long result;
  size_t i;

  unsigned long long map_k[2048];
  unsigned long long map_v[2048];
  size_t map_l = 0;

  while (fgets(line, sizeof line, file) != NULL) {
    if (line[1] == 'a') {
      sscanf(line, "mask = %s\n", mask);
      /* printf("\nMASK   %s\n", mask); */
    } else {
      sscanf(line, "mem[%llu] = %llu\n", &mem_index, &mem_value);
      /* printf("mem[%llu] = %llu\n", mem_index, mem_value); */
      /* printf("value  "); */
      /* print_36bin(mem_value); */
      /* printf("mask   %s\n", mask); */

      result = 0;
      for (i=0; i<MASK_LENGTH; i++) {
        if (mask[MASK_LENGTH - 1 - i] == '1' ||
            (mask[MASK_LENGTH - 1 - i] == 'X' && mem_value % 2 > 0)) {
          result += pow(2, i);
        }

        mem_value /= 2;
      }

      /* printf("result "); */
      /* print_36bin(result); */

      /* update existing value in memory if there is map_k with
         current mem_index */
      for (i=0; i<map_l; i++) {
        if (map_k[i] == mem_index) {
          map_v[i] = result;
          goto skip;
        }
      }

      /* if value is not defined in memory then set it for the first
         time */
      map_k[map_l] = mem_index;
      map_v[map_l] = result;
      map_l++;

    skip: continue;
    }
  }
  fclose(file);

  result = 0;
  for (i=0; i<map_l; i++) result += map_v[i];

  return result;
}

int
main() {
  printf("%llu (165)\n", solve1("14i1"));
  printf("%llu (5055782549997)\n", solve1("14i2"));
  return 0;
}
