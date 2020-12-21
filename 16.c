#include <stdio.h>
#include <stdlib.h>

int
solve1(char file_name[6]) {
  FILE * file = fopen(file_name, "r");

  if (file == NULL) return -1;

  size_t i, j, k;
  char line_buffer[128];
  char * line = line_buffer;
  char * line_start = line_buffer;

  int valid_numbers_flag = 1;
  int nearby_tickets_flag = 0;

  int valid_numbers[1024];
  size_t valid_numbers_count = 0;
  int number;
  unsigned long long result = 0;

  while(fgets(line, sizeof line_buffer, file) != NULL) {
    if (line[0] == '\n') {
      valid_numbers_flag = 0;
      continue;
    }

    if (line[0] == 'n') {
      nearby_tickets_flag = 1;
      continue;
    }

    if (valid_numbers_flag) {
      sscanf(line, "%*[^:]: %d-%d or %d-%d\n",
             &valid_numbers[valid_numbers_count+0],
             &valid_numbers[valid_numbers_count+1],
             &valid_numbers[valid_numbers_count+2],
             &valid_numbers[valid_numbers_count+3]);
      valid_numbers_count += 4;
    }

    if (nearby_tickets_flag) {
      i = 0;
      while (1) {
        if (line[i] != ',' && line[i] != '\n') {
          i++;
          continue;
        }

        line[i] = 0;
        number = atoi(line);

        for (j=0; j<valid_numbers_count; j+=2)
          for (k=valid_numbers[j]; k<=(unsigned int)valid_numbers[j+1]; k++)
            if (k == (unsigned int)number) goto skip;

        result += number;

      skip: if (line[i+1] == 0) break;
        line += i + 1;
        i = 0;
      }

      line = line_start;
    }
  }

  fclose(file);
  return result;
}

int
main() {
  printf("%d (71)\n",    solve1("16i1"));
  printf("%d (22073)\n", solve1("16i2"));

  return 0;
}
