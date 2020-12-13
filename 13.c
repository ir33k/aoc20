#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int
solve1(char file_name[8]) {
  FILE * file = fopen(file_name, "r");
  char line[256], number_char[16];
  int i=0, j;

  int timestamp;
  int ids[12];
  int ids_count = 0;

  int diff;
  int min = INT_MAX;
  int min_index = 0;

  if (file == NULL) return 0;

  /* read first line */
  if (fgets(line, sizeof line, file) == NULL) return 0;

  timestamp = atoi(line);

  /* read second line */
  if (fgets(line, sizeof line, file) == NULL) return 0;

  while (line[i] != '\n') {
    /* skip , and x */
    while (line[i] == ',' || line[i] == 'x') i++;

    /* get id as string */
    for (j=0; line[i] != ',' && line[i] != '\n'; i++,j++)
      number_char[j] = line[i];
    number_char[j] = 0;

    /* store id in ids array */
    ids[ids_count] = atoi(number_char);

    /* find out how many minutes you have to wait */
    diff = ids[ids_count] - (timestamp % ids[ids_count]);

    /* set min wait time, remember min id index */
    if (diff < min) {
      min = diff;
      min_index = ids_count;
    }

    ids_count++;
  }

  fclose(file);
  return ids[min_index] * min;
}

/* https://www.geeksforgeeks.org/chinese-remainder-theorem-set-2-implementation/ */
long long int
inv(long long int a, long long int m) {
  long long int m0 = m, t, q;
  long long int x0 = 0, x1 = 1;

  if (m == 1) return 0;

  while (a > 1) {
    q = a / m;
    t = m;
    m = a % m, a = t;
    t = x0;
    x0 = x1 - q * x0;
    x1 = t;
  }

  if (x1 < 0) x1 += m0;

  return x1;
}

unsigned long long int
findMinX(int num[512], int rem[512], int k) {
  int i;
  unsigned long long int prod = 1;
  unsigned long long int result = 0;

  for (i = 0; i < k; i++)
    prod *= num[i];

  for (i = 0; i < k; i++) {
    unsigned long long int pp = prod / num[i];
    result += rem[i] * inv(pp, num[i]) * pp;
  }

  return result % prod;
}

unsigned long long int
solve2(char input[256]) {
  int i=0, j;

  int num[512];
  int rem[512];
  int num_count = 0;

  int index = 0;
  char number_char[16];

  while (1) {
    if (input[i] == 'x') {
      index++;
      i += 2;
      continue;
    }

    for (j=0; input[i] != ',' && input[i] != '\0'; i++,j++)
      number_char[j] = input[i];
    number_char[j] = 0;

    num[num_count] = atoi(number_char);
    rem[num_count] = num[num_count] - index;

    num_count++;
    index++;

    if (input[i] == '\0') break;
    i++;
  }

  return findMinX(num, rem, num_count);
}

int
main() {
  printf("%d (295)\n", solve1("13i1"));
  printf("%d (207)\n", solve1("13i2"));

  printf("%lld (3417)\n", solve2("17,x,13,19"));
  printf("%lld (1068781)\n", solve2("7,13,x,x,59,x,31,19"));
  printf("%lld (754018)\n", solve2("67,7,59,61"));
  printf("%lld (779210)\n", solve2("67,x,7,59,61"));
  printf("%lld (1261476)\n", solve2("67,7,x,59,61"));
  printf("%lld (1202161486)\n", solve2("1789,37,47,1889"));

  /* this will take a long time */
  printf("%lld (530015546283687)\n", solve2("17,x,x,x,x,x,x,x,x,x,x,37,x,x,x,x,x,409,x,29,x,x,x,x,x,x,x,x,x,x,13,x,x,x,x,x,x,x,x,x,23,x,x,x,x,x,x,x,373,x,x,x,x,x,x,x,x,x,41,x,x,x,x,x,x,x,x,19"));

  return 0;
}
