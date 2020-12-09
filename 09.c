#include <stdio.h>
#include <stdlib.h>

#define MAX_NUMBERS 1024

int
parse_input(long int numbers[MAX_NUMBERS], char file_name[8]) {
  int i;
  FILE * file = fopen(file_name, "r");
  char line[16];
  char number_char[16];
  int numbers_count = 0;

  if (file == NULL) return -1;

  while (fgets(line, sizeof line, file) != NULL) {
    for (i=0; line[i] != '\n'; i++) number_char[i] = line[i];
    number_char[i] = 0;
    numbers[numbers_count] = atoll(number_char);
    numbers_count++;
  }

  fclose(file);
  return numbers_count;
}

int
solve1(int preamble, char file_name[8]) {
  int i,j,k;
  long int numbers[MAX_NUMBERS];
  int numbers_count = parse_input(numbers, file_name);

  for (i=preamble; i<numbers_count; i++) {
    for (j=i-preamble; j<i; j++) {
      for (k=j+1; k<i; k++) {
        if (numbers[j] + numbers[k] == numbers[i]) {
          k = -1;
          break;
        }
      }

      if (k == -1) {
        j = -1;
        break;
      }
    }

    if (j != -1) return numbers[i];
  }
  
  return numbers[i];
}

int
main() {
  printf("%d\n", solve1(5, "09i1"));
  printf("%d\n", solve1(25, "09i2"));
  return 0;
}
