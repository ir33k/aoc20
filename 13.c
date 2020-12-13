#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int
solve1(char file_name[8]) {
  FILE * file = fopen(file_name, "r");
  char line[256], number_char[16];
  int i=0, j;
  int ids[12];
  int ids_count = 0;
  int diff;
  int min = INT_MAX;
  int min_index = 0;

  if (file == NULL) return 0;

  if (fgets(line, sizeof line, file) == NULL) return 0;
  int timestamp = atoi(line);

  if (fgets(line, sizeof line, file) == NULL) return 0;
  while (line[i] != '\n') {
    while (line[i] == ',' || line[i] == 'x') i++;
    for (j=0; line[i] != ',' && line[i] != '\n'; i++,j++)
      number_char[j] = line[i];
    number_char[j] = 0;
    ids[ids_count] = atoi(number_char);
    diff = ids[ids_count] - (timestamp % ids[ids_count]);
    if (diff < min) {
      min = diff;
      min_index = ids_count;
    }
    ids_count++;
  }

  fclose(file);
  return ids[min_index] * min;
}

int
main() {
  printf("%d (295)\n", solve1("13i1"));
  printf("%d (207)\n", solve1("13i2"));

  return 0;
}
