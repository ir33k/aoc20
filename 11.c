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
    lines[lines_count][i] = 0;
    lines_count++;
  }

  fclose(file);
  return lines_count;
}

/* Position (x,y) have 0,0 coordinates in top left corner. */
int
is_seeted_in_directory(char input[MAX_LINES_COUNT][MAX_LINE_LENGTH],
                       int rows_count, int x, int y, int dx, int dy, int repeat) {
  x += dx;
  y += dy;

  if (x < 0 || y < 0 || y >= rows_count || input[y][x] == 0) return 0;
  if (input[y][x] == '#') return 1;

  if (repeat) {
    if (input[y][x] == 'L') return 0;
    if (input[y][x] == '.')
      return is_seeted_in_directory(input, rows_count, x, y, dx, dy, 1);
  }

  return 0;
}

int
count_neighbors(char input[MAX_LINES_COUNT][MAX_LINE_LENGTH],
                int rows_count, int x, int y) {
  return
    is_seeted_in_directory(input, rows_count, x, y, -1, -1, 0) +
    is_seeted_in_directory(input, rows_count, x, y,  0, -1, 0) +
    is_seeted_in_directory(input, rows_count, x, y, +1, -1, 0) +
    is_seeted_in_directory(input, rows_count, x, y, +1,  0, 0) +
    is_seeted_in_directory(input, rows_count, x, y, +1, +1, 0) +
    is_seeted_in_directory(input, rows_count, x, y,  0, +1, 0) +
    is_seeted_in_directory(input, rows_count, x, y, -1, +1, 0) +
    is_seeted_in_directory(input, rows_count, x, y, -1,  0, 0);
}

int
count_visible(char input[MAX_LINES_COUNT][MAX_LINE_LENGTH],
              int rows_count, int x, int y) {
  return
    is_seeted_in_directory(input, rows_count, x, y, -1, -1, 1) +
    is_seeted_in_directory(input, rows_count, x, y,  0, -1, 1) +
    is_seeted_in_directory(input, rows_count, x, y, +1, -1, 1) +
    is_seeted_in_directory(input, rows_count, x, y, +1,  0, 1) +
    is_seeted_in_directory(input, rows_count, x, y, +1, +1, 1) +
    is_seeted_in_directory(input, rows_count, x, y,  0, +1, 1) +
    is_seeted_in_directory(input, rows_count, x, y, -1, +1, 1) +
    is_seeted_in_directory(input, rows_count, x, y, -1,  0, 1);
}

void
print_state(char input[MAX_LINES_COUNT][MAX_LINE_LENGTH],
            int rows_count) {
  for (int i=0; i<rows_count; i++)
    printf("%s\n", input[i]);
}

void
clone_array(char input[MAX_LINES_COUNT][MAX_LINE_LENGTH],
            char target[MAX_LINES_COUNT][MAX_LINE_LENGTH],
            int rows_count) {
  int x,y;
  for ( y=0; y<rows_count; y++) {
    for (x=0; input[0][x] != 0; x++) target[y][x] = input[y][x];
    target[y][x] = '\0';
  }
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
  char state1[MAX_LINES_COUNT][MAX_LINE_LENGTH];
  char state2[MAX_LINES_COUNT][MAX_LINE_LENGTH];
  char rows_count = parse_input(state1, file_name);

  clone_array(state1, state2, rows_count);

  while (1) {
    for (y=0; y<rows_count; y++) {
      for (x=0; state1[0][x] != 0; x++) {
        occupied = count_neighbors(state1, rows_count, x, y);
        /**/ if (state1[y][x] == 'L' && occupied == 0) state2[y][x] = '#';
        else if (state1[y][x] == '#' && occupied >= 4) state2[y][x] = 'L';
      }
    }
  
    if (are_arrays_the_same(state1, state2, rows_count)) break;
    clone_array(state2, state1, rows_count);
  }

  return count_char(state1, rows_count, '#');
}

unsigned long int
solve2(char file_name[8]) {
  int x,y,occupied;
  char state1[MAX_LINES_COUNT][MAX_LINE_LENGTH];
  char state2[MAX_LINES_COUNT][MAX_LINE_LENGTH];
  char rows_count = parse_input(state1, file_name);

  clone_array(state1, state2, rows_count);

  while (1) {
    for (y=0; y<rows_count; y++) {
      for (x=0; state1[0][x] != 0; x++) {
        occupied = count_visible(state1, rows_count, x, y);
        /**/ if (state1[y][x] == 'L' && occupied == 0) state2[y][x] = '#';
        else if (state1[y][x] == '#' && occupied >= 5) state2[y][x] = 'L';
      }
    }
  
    if (are_arrays_the_same(state1, state2, rows_count)) break;
    clone_array(state2, state1, rows_count);
  }

  return count_char(state1, rows_count, '#');
}

int
main() {
  printf("%ld (37)\n", solve1("11i1"));
  printf("%ld (2108)\n", solve1("11i2"));

  printf("%ld (26)\n", solve2("11i1"));
  printf("%ld (26)\n", solve2("11i2"));

  return 0;
}
