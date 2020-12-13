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

unsigned long long int
solve2(char ids_str[256], unsigned long long int result) {
  int i=0, j;
  struct { unsigned int id; unsigned int index; } ids[512];
  int ids_count = 0;
  int index = 0;
  char number_char[16];

  while (1) {
    if (ids_str[i] == 'x') {
      index++;
      i += 2;
      continue;
    }

    for (j=0; ids_str[i] != ',' && ids_str[i] != '\0'; i++,j++)
      number_char[j] = ids_str[i];
    number_char[j] = 0;

    ids[ids_count].id = atoi(number_char);
    ids[ids_count].index = index;

    ids_count++;
    index++;

    if (ids_str[i] == '\0') break;
    i++;
  }

  while (++result) {
    if (result % ids[0].id != 0) {
      result += ids[0].id - (result % ids[0].id) - 1;
      continue;
    }
    /* printf("%d: %lld\n", ids[0].id, result); */
    for (i=1; i<ids_count; i++) {
      if (ids[i].index != ids[i].id - (result % ids[i].id)) {
        result += ids[0].id - (result % ids[0].id) - 1;
        /* printf("%d %d %lld %lld\n", */
        /*        ids[i].id, */
        /*        ids[i].index, */
        /*        result, */
        /*        result + ids[0].id - (result % ids[0].id) - 1); */
        goto next;
      }
      /* printf("%d: %lld\n", ids[i].id, result); */
    }
    break;
  next: continue;
  }
  
  /* for (i=0; i<ids_count; i++) */
  /*   printf("%d: %d\n", */
  /*          ids[i].index, */
  /*          ids[i].id); */

  return result;
}

int
main() {
  printf("%d (295)\n", solve1("13i1"));
  printf("%d (207)\n", solve1("13i2"));

  printf("%lld (1068781)\n", solve2("7,13,x,x,59,x,31,19", 0));
  printf("%lld (3417)\n", solve2("17,x,13,19", 0));
  printf("%lld (754018)\n", solve2("67,7,59,61", 0));
  printf("%lld (779210)\n", solve2("67,x,7,59,61", 0));
  printf("%lld (1261476)\n", solve2("67,7,x,59,61", 0));
  printf("%lld (1202161486)\n", solve2("1789,37,47,1889", 0));

  /* this will take a long time */
  /* printf("%lld ()\n", solve2("17,x,x,x,x,x,x,x,x,x,x,37,x,x,x,x,x,409,x,29,x,x,x,x,x,x,x,x,x,x,13,x,x,x,x,x,x,x,x,x,23,x,x,x,x,x,x,x,373,x,x,x,x,x,x,x,x,x,41,x,x,x,x,x,x,x,x,19", 100000000000000)); */

  return 0;
}
