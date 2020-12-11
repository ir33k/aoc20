#include <stdio.h>

#define MAX_LINE_LENGTH 128
#define MAX_LINES_COUNT 128

int
parse_input(char lines[MAX_LINES_COUNT][MAX_LINE_LENGTH],
            char file_name[8]) {
  int i;
  FILE * file = fopen(file_name, "r");
  char line[MAX_LINE_LENGTH];
  int lines_count = 0;

  if (file == NULL) return -1;

  while(fgets(line, sizeof line, file) != NULL) {
    for (i=0; line[i] != '\n'; i++) lines[lines_count][i] = line[i];
    lines_count++;
  }

  fclose(file);
  return lines_count;
}

/* Position (x,y) have 0,0 coordinates in top left corner. */
int
count_occupied_adjacent(char input[MAX_LINES_COUNT][MAX_LINE_LENGTH],
                        int rows_count, int x, int y) {
  int count = 0;
  int left  = x > 0;
  int right = input[x+1][y];
  int up    = y > 0;
  int down  = y < rows_count - 1;
                                                         /*  x, y */
  if (left  && up   && input[y-1][x-1] == '#') count ++; /* -1,-1 */
  if (         up   && input[y-1][x  ] == '#') count ++; /*  0,-1 */
  if (right && up   && input[y-1][x+1] == '#') count ++; /* +1,-1 */
  if (left          && input[y  ][x-1] == '#') count ++; /* -1, 0 */
  if (right         && input[y  ][x+1] == '#') count ++; /* +1, 0 */
  if (left  && down && input[y+1][x-1] == '#') count ++; /* -1,+1 */
  if (         down && input[y+1][x  ] == '#') count ++; /*  0,+1 */
  if (right && down && input[y+1][x+1] == '#') count ++; /* +1,+1 */

  return count;
}

void
print_input(char input[MAX_LINES_COUNT][MAX_LINE_LENGTH],
            int rows_count) {
  for (int i=0; i<rows_count; i++)
    printf("%s\n", input[i]);
}

void
clone_array(char input[MAX_LINES_COUNT][MAX_LINE_LENGTH],
            char target[MAX_LINES_COUNT][MAX_LINE_LENGTH],
            int rows_count) {
  for (int x=0; input[0][x] != 0; x++)
    for (int y=0; y<rows_count; y++)
      target[y][x] = input[y][x];
}

int
are_arrays_the_same(char input[MAX_LINES_COUNT][MAX_LINE_LENGTH],
                    char target[MAX_LINES_COUNT][MAX_LINE_LENGTH],
                    int rows_count) {
  for (int x=0; input[0][x] != 0; x++)
    for (int y=0; y<rows_count; y++)
      if (target[y][x] != input[y][x]) return 0;

  return 1;
}

unsigned long int
count_char(char input[MAX_LINES_COUNT][MAX_LINE_LENGTH],
           int rows_count, char c) {
  unsigned long int count = 0;

  for (int x=0; input[0][x] != 0; x++)
    for (int y=0; y<rows_count; y++)
      if (input[y][x] == c) count++;

  return count;
}

unsigned long int
solve1(char file_name[8]) {
  int x,y,occupied;
  char input[MAX_LINES_COUNT][MAX_LINE_LENGTH];
  char state[MAX_LINES_COUNT][MAX_LINE_LENGTH];
  char rows_count = parse_input(input, file_name);

  clone_array(input, state, rows_count);

  /* print_input(input, rows_count); */
  /* printf("\n"); */

  while (1) {
    for (y=0; y<rows_count; y++) {
      for (x=0; input[0][x] != 0; x++) {
        occupied = count_occupied_adjacent(input, rows_count, x, y);
        /**/ if (input[y][x] == 'L' && occupied == 0) state[y][x] = '#';
        else if (input[y][x] == '#' && occupied >= 4) state[y][x] = 'L';
      }
    }
  
    /* print_input(state, rows_count); */
    /* printf("\n"); */

    if (are_arrays_the_same(input, state, rows_count)) break;

    clone_array(state, input, rows_count);
  }

  return count_char(input, rows_count, '#');
}

int
main() {
  printf("%ld (37)\n", solve1("11i1"));
  printf("%ld (2142)\n", solve1("11i2")); /* too high */
  return 0;
}
