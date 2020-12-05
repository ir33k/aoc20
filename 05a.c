#include <stdio.h>
#include <stdlib.h>

int
get_id(char seat[11]) {
  size_t i;

  /*
   * End values are more like a size then end value.  Because in case
   * of rows we have 128 rows in total.  But we count from 0 to 127.
   * I just 128 as end value so its easier to divide by 2.  I just
   * have to remember that end value is actually -= 1.  Its the same
   * in case of columns.  I use 8 as end value but actually its 7.
   *
   * Even tho I'm not subtracting 1 from end values because they can't
   * be higher then start value and at then end they always end up
   * being higher by 1 from start value.  In case of this task the
   * lower values should be chosen and that is always the start value.
   */

  unsigned short rs = 0;        /* row start */
  unsigned short re = 128;      /* row end */
  unsigned short cs = 0;        /* column start */
  unsigned short ce = 8;        /* column end */
  unsigned short fr;            /* final row */
  unsigned short fc;            /* final column */

  for (i=0; seat[i] != '\0'; i++)
    /**/ if (seat[i] == 'F') re = rs + (re - rs) / 2;
    else if (seat[i] == 'B') rs = rs + (re - rs) / 2;
    else if (seat[i] == 'L') ce = cs + (ce - cs) / 2;
    else if (seat[i] == 'R') cs = cs + (ce - cs) / 2;

  /* 
   * Unnecessary conditions just in case.  But end values should never
   * end up being higher then start values.
   */

  fr = rs; if (re < rs) fr = re;
  fc = cs; if (ce < cs) fc = ce;

  return fr * 8 + fc;
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
  int result;
  int highest_id = 0;

  if (file == NULL) return 1;

  while(fgets(line, sizeof line, file) != NULL) {
    line[10] = '\0';            /* Remove new line char */
    result = get_id(line);
    if (result > highest_id) highest_id = result;
    printf("%d\n", result);
  }

  printf("Highest id: %d\n", highest_id);

  fclose(file);
  return 0;
}
