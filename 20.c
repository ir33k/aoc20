#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct { int normal[4]; int fliped[4]; } Edges;
typedef struct { int id; Edges edges; } Tile;

int
binary_string_to_decimal(char * str, int revers) {
  int result = 0;
  int str_length = 0;
  int i;

  while (str[str_length] != 0) str_length++;

  for (i=0; i<str_length; i++)
    if (str[revers ? i : str_length - 1 - i] == '#')
      result += pow(2, i);

  return result;
}

int
parse_tile(Tile * tile, FILE * file) {
  int i;
  char line[16];
  char line_top[11];
  char line_right[11];
  char line_bottom[11];
  char line_left[11];

  if (fgets(line, sizeof line, file) == NULL)
    return 0;

  if (!sscanf(line, "Tile %d:", &tile->id))
    return 0;

  i=0;
  while (fgets(line, sizeof line, file) != NULL && i<10) {
    line_right[i] = line[9];
    line_left[i]  = line[0];

    if (i == 0) strncpy(line_top, line, 10);
    if (i == 9) strncpy(line_bottom, line, 10);

    i++;
  }

  line_top[i] = line_right[i] = line_bottom[i] = line_left[i] = 0;

  tile->edges.normal[0] = binary_string_to_decimal(line_top, 0);
  tile->edges.normal[1] = binary_string_to_decimal(line_right, 0);
  tile->edges.normal[2] = binary_string_to_decimal(line_bottom, 0);
  tile->edges.normal[3] = binary_string_to_decimal(line_left, 0);

  tile->edges.fliped[0] = binary_string_to_decimal(line_top, 1);
  tile->edges.fliped[1] = binary_string_to_decimal(line_right, 1);
  tile->edges.fliped[2] = binary_string_to_decimal(line_bottom, 1);
  tile->edges.fliped[3] = binary_string_to_decimal(line_left, 1);

  return 1;
}

int
parse_input(Tile * tiles, char * file_name) {
  FILE * file = fopen(file_name, "r");
  int tiles_count = 0;

  if (file == NULL) {
    printf("Issue with file.  Does it exist?  🤔");
    exit(1);
  }

  while (parse_tile(&tiles[tiles_count], file))
    tiles_count++;

  fclose(file);
  return tiles_count;
}

unsigned long long
solve1(char file_name[6]) {
  Tile tiles[256] = { 0 };
  int tiles_count;

  tiles_count = parse_input(tiles, file_name);

  /* 1951 2311 3079 */
  /* 2729 1427 2473 */
  /* 2971 1489 1171 */

  printf("Tiles count %d\n", tiles_count);
  while (tiles_count--) {
    printf("Id: %d\n", tiles[tiles_count].id);
    printf("Normal: ");
    for (int i=0; i<4; i++) printf("%d ", tiles[tiles_count].edges.normal[i]);
    printf("\b\nFliped: ");
    for (int i=0; i<4; i++) printf("%d ", tiles[tiles_count].edges.fliped[i]);
    printf("\b\n");
  }
  
  return 0;
}

int
main() {
  printf("%llu (20899048083289)\n", solve1("20i1"));

  return 0;
}
