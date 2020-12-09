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

long int
find_invalid(const int preamble, const long int numbers[MAX_NUMBERS],
             const int numbers_count) {
  int i,j,k;

  for (i=preamble; i<numbers_count; i++) {
    for (j=i-preamble; j<i; j++) {
      for (k=j+1; k<i; k++) {
        if (numbers[j] + numbers[k] == numbers[i]) {
          /*
           * I often set loop index to -1 as indicator to code outside
           * of this loop that loop was break.
           */
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

long int
solve1(const int preamble, char file_name[8]) {
  long int numbers[MAX_NUMBERS];
  int numbers_count = parse_input(numbers, file_name);
  return find_invalid(preamble, numbers, numbers_count);
}

long int
solve2(const int preamble, char file_name[8]) {
  int i;
  int start = 0;
  int end = 0;

  long int numbers[MAX_NUMBERS];
  int numbers_count = parse_input(numbers, file_name);
  long int invalid = find_invalid(preamble, numbers, numbers_count);

  int found = 0;
  long int sum = 0;
  long int min = 0;
  long int max = 0;

  for (start=0; start<numbers_count; start++) {
    for (end=start+1; end<numbers_count; end++) {
      sum = 0;
      for (i=start; i<end; i++) sum += numbers[i];
      if (sum == invalid) {
        found = 1;
        break;
      }
    }
    if (found) break;
  }

  min = numbers[start];         /* min can't be initially 0 */
  for (i=start; i<end; i++) {
    if (numbers[i] > max) max = numbers[i];
    if (numbers[i] < min) min = numbers[i];
  }

  return max + min;
}

int
main() {
  printf("%ld (127)\n", solve1(5, "09i1"));
  printf("%ld (1492208709)\n", solve1(25, "09i2"));

  printf("%ld (62)\n", solve2(5, "09i1"));
  printf("%ld (238243506)\n", solve2(25, "09i2"));
  return 0;
}
