#include <stdio.h>
#include <stdlib.h>

int
get_id(char seat[11]) {
  size_t i;

  unsigned short rs = 0;        /* row start */
  unsigned short re = 128;      /* row end */
  unsigned short cs = 0;        /* column start */
  unsigned short ce = 8;        /* column end */

  for (i=0; seat[i] != '\0'; i++)
    /**/ if (seat[i] == 'F') re = rs + (re - rs) / 2;
    else if (seat[i] == 'B') rs = rs + (re - rs) / 2;
    else if (seat[i] == 'L') ce = cs + (ce - cs) / 2;
    else if (seat[i] == 'R') cs = cs + (ce - cs) / 2;

  return rs * 8 + cs;
}

void
test_get_id() {
  size_t i;
  int result;

  struct { char seat[11]; int id; } inputs[4] = {
    { "FBFBBFFRLR", 357 },
    { "BFFFBBFRRR", 567 },
    { "FFFBBBFRRR", 119 },
    { "BBFFBBFRLL", 820 },
  };

  for (i=0; i<4; i++) {
    result = get_id(inputs[i].seat);

    if (result != inputs[i].id) {
      printf("input       : '%s'\n", inputs[i].seat);
      printf("expected_id : '%d'\n", inputs[i].id);
      printf("actuall_id  : '%d'\n", result);
      exit(1);
    }
  }
}

int
main() {
  test_get_id();

  FILE * file = fopen("05input1", "r");
  char line[12];
  int result = 0;
  int highest_id = 0;

  int id_flags[2048] = { 0 };
  int start_searching = 0;

  if (file == NULL) return 1;

  /* Calculate all IDs and find highest of them */
  while(fgets(line, sizeof line, file) != NULL) {
    line[10] = '\0';            /* Remove new line char */
    result = get_id(line);
    if (result > highest_id) highest_id = result;
    id_flags[result] = 1;
  }

  printf("Highest id: %d\n", highest_id);

  /* Search for missing ID */
  while(start_searching == 0 || id_flags[result] == 1) {
    if (id_flags[result] == 1) start_searching = 1;
    result++;
  }
  printf("Missing id: %d\n", result);

  fclose(file);
  return 0;
}
