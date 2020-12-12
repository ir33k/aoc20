#include <stdio.h>
#include <stdlib.h>

int
solve1(char file_name[8]) {
  FILE * file = fopen(file_name, "r");
  char line[8];

  char type;                    /* type of instruction */
  int value;                    /* instruction value */

  int pos_x = 0;                /* its east, west (+right, -left) */
  int pos_y = 0;                /* its north, south (+up, -down) */
  int rotation = 0;

  if (file == NULL) return -1;

  while(fgets(line, sizeof line, file) != NULL) {
    type = line[0];
    /* Making first char "0" makes the whole like a valid int */
    line[0] = '0';
    value = atoi(line);

    /* For type F use one of directions base on current rotation. */
    if (type == 'F')
      switch (rotation) {
      case   0: type = 'E'; break; case 180: type = 'W'; break;
      case 270: type = 'N'; break; case  90: type = 'S'; break;
      }

    /* Rotation left, moving west and south are basically the same
       thing like corresponding types but with negative value */
    if (type == 'L' || type == 'W' || type == 'S') value *= -1;

    switch (type) {
    case 'E': case 'W': pos_x += value; continue;
    case 'N': case 'S': pos_y += value; continue;
    case 'R': case 'L': rotation = (360 + rotation + value) % 360;
    }
  }

  fclose(file);
  return abs(pos_x) + abs(pos_y);
}

int
solve2(char file_name[8]) {
  FILE * file = fopen(file_name, "r");
  char line[8];

  char type;                    /* type of instruction */
  int value;                    /* instruction value */

  int tmp_pos;
  int ship_x = 0,  ship_y = 0;
  int wayp_x = 10, wayp_y = 1;  /* NOTE initial values 10, 1*/

  if (file == NULL) return -1;

  while(fgets(line, sizeof line, file) != NULL) {
    type = line[0];
    /* Overwrite first line character with "0" to make the whole like
       a valid number for "atoi" function. */
    line[0] = '0';
    value = atoi(line);

    if (type == 'W' || type == 'S') value *= -1;

    switch (type) {
    case 'E': case 'W': wayp_x += value; break;
    case 'N': case 'S': wayp_y += value; break;
    case 'L': case 'R':
      if (type == 'R' && (value == 90 || value == 270))
        value = (value + 180) % 360;

      switch (value) {
      case 90:
        tmp_pos = wayp_x;
        wayp_x = wayp_y * -1;
        wayp_y = tmp_pos;
        break;
      case 180:
        wayp_x *= -1;
        wayp_y *= -1;
        break;
      case 270:
        tmp_pos = wayp_x;
        wayp_x = wayp_y;
        wayp_y = tmp_pos * -1;
        break;
      }
      break;
    case 'F':
      ship_x += wayp_x * value;
      ship_y += wayp_y * value;
      break;
    }
  }

  fclose(file);
  return abs(ship_x) + abs(ship_y);
}

int
main() {
  printf("%d (25)\n", solve1("12i1"));
  printf("%d (938)\n", solve1("12i2"));

  printf("%d (286)\n", solve2("12i1"));
  printf("%d (54404)\n", solve2("12i2"));

  return 0;
}
