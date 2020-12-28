#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct {
  int id;
  int edges[8];
} Tile;

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

  tile->edges[0] = binary_string_to_decimal(line_top, 0);
  tile->edges[1] = binary_string_to_decimal(line_right, 0);
  tile->edges[2] = binary_string_to_decimal(line_bottom, 0);
  tile->edges[3] = binary_string_to_decimal(line_left, 0);

  tile->edges[4] = binary_string_to_decimal(line_top, 1);
  tile->edges[5] = binary_string_to_decimal(line_right, 1);
  tile->edges[6] = binary_string_to_decimal(line_bottom, 1);
  tile->edges[7] = binary_string_to_decimal(line_left, 1);

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
  unsigned long long result = 1;
  Tile tiles[256] = { 0 };
  int tiles_count;
  int found_repeat_flag;
  int found_ids[4];
  int found_ids_count = 0;
  int i,j,k,l;

  tiles_count = parse_input(tiles, file_name);

  // printf("Tiles count %d\n", tiles_count);
  // for (i=0; i<tiles_count; i++) {
  //   printf("Id: %d (", tiles[i].id);
  //   for (j=0; j<8; j++) printf("%3d ", tiles[i].edges[j]);
  //   printf("\b)\n");
  // }

  for (i=0; i<tiles_count; i++) {
    for (j=0; j<8; j++) {
      found_repeat_flag = 0;

      for (k=i+1; k<tiles_count; k++)
        for (l=0; l<8; l++)
          if (tiles[i].edges[j] == tiles[k].edges[l]) {
            tiles[k].edges[l] = -1;
            found_repeat_flag = 1;
          }

      if (found_repeat_flag)
        tiles[i].edges[j] = -1;
    }
  }

  for (i=0; i<tiles_count; i++) {
    k=0;
    for (j=0; j<8; j++)
      if (tiles[i].edges[j] == -1) k++;

    if (k == 4)
      found_ids[found_ids_count++] = tiles[i].id;
  }

  for (i=0; i<found_ids_count; i++)
    result *= found_ids[i];

  return result;
}

int
main() {
  printf("%llu (20899048083289)\n", solve1("20i1"));
  printf("%llu (45079100979683)\n", solve1("20i2"));

  return 0;
}
