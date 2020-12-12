#include <stdio.h>
#include <stdlib.h>

#define MAX_INSTRUCTIONS 800

typedef struct {
  char type;
  int value;
} instruction;

/* This also can be read as X (+right, -left) and Y (+up, -down) */
typedef struct {
  int east_west;       /* + value is east (right), - is west (left) */
  int north_south;     /* + value is north (up), - south (down) */
  int rotation;        /* 0 == east (right) */
} position;

int
parse_input(char file_name[8],
            instruction instructions[MAX_INSTRUCTIONS]) {
  int i;
  int instructions_count = 0;
  FILE * file = fopen(file_name, "r");
  char line[8];
  char number_char[8];

  if (file == NULL) return -1;

  while(fgets(line, sizeof line, file) != NULL) {
    instructions[instructions_count].type = line[0];

    for (i=0; line[i+1] != '\n'; i++) number_char[i] = line[i+1];
    number_char[i] = 0;

    instructions[instructions_count].value = atoi(number_char);
    instructions_count++;
  }

  return instructions_count;
}

int
solve1(char file_name[8]) {
  int i;
  instruction instructions[MAX_INSTRUCTIONS];
  int instructions_count = parse_input(file_name, instructions);

  position pos = { 0, 0, 0 };

  for (i=0; i<instructions_count; i++) {
    switch (instructions[i].type) {
    case 'E': pos.east_west   += instructions[i].value; break;
    case 'W': pos.east_west   -= instructions[i].value; break;
    case 'N': pos.north_south += instructions[i].value; break;
    case 'S': pos.north_south -= instructions[i].value; break;
    case 'L':
      pos.rotation -= instructions[i].value;
      if (pos.rotation < 0) pos.rotation += 360;
      break;
    case 'R':
      pos.rotation += instructions[i].value;
      if (pos.rotation >= 360) pos.rotation -= 360;
      break;
    case 'F':
      switch (pos.rotation) {
      case   0: pos.east_west   += instructions[i].value; break;
      case 180: pos.east_west   -= instructions[i].value; break;
      case 270: pos.north_south += instructions[i].value; break;
      case  90: pos.north_south -= instructions[i].value; break;
      }
    }
  }

  if (pos.east_west   < 0) pos.east_west   *= -1;
  if (pos.north_south < 0) pos.north_south *= -1;

  return pos.east_west + pos.north_south;
}

int
solve2(char file_name[8]) {
  int i;
  instruction instructions[MAX_INSTRUCTIONS];
  int instructions_count = parse_input(file_name, instructions);

  int tmp_pos_value;
  position pos_ship = { 0, 0, 0 };
  position pos_waypoint = { 10, 1, 0 };

  for (i=0; i<instructions_count; i++) {
    switch (instructions[i].type) {
    case 'E': pos_waypoint.east_west   += instructions[i].value; break;
    case 'W': pos_waypoint.east_west   -= instructions[i].value; break;
    case 'N': pos_waypoint.north_south += instructions[i].value; break;
    case 'S': pos_waypoint.north_south -= instructions[i].value; break;
    case 'L':
    case 'R':
      if (instructions[i].type == 'R') {
        /**/ if (instructions[i].value == 90) instructions[i].value = 270;
        else if (instructions[i].value == 270) instructions[i].value = 90;
      }

      switch (instructions[i].value) {
      case 90:
        tmp_pos_value            = pos_waypoint.east_west;
        pos_waypoint.east_west   = pos_waypoint.north_south * -1;
        pos_waypoint.north_south = tmp_pos_value;
        break;
      case 180:
        pos_waypoint.east_west   *= -1;
        pos_waypoint.north_south *= -1;
        break;
      case 270:
        tmp_pos_value            = pos_waypoint.east_west;
        pos_waypoint.east_west   = pos_waypoint.north_south;
        pos_waypoint.north_south = tmp_pos_value * -1;
        break;
      }
      break;
     case 'F':
      pos_ship.east_west   += pos_waypoint.east_west   * instructions[i].value;
      pos_ship.north_south += pos_waypoint.north_south * instructions[i].value;
      break;
    }
  }

  if (pos_ship.east_west   < 0) pos_ship.east_west   *= -1;
  if (pos_ship.north_south < 0) pos_ship.north_south *= -1;

  return pos_ship.east_west + pos_ship.north_south;
}

int
main() {
  printf("%d (25)\n", solve1("12i1"));
  printf("%d (938)\n", solve1("12i2"));

  printf("%d (286)\n", solve2("12i1"));
  printf("%d (54404)\n", solve2("12i2"));

  return 0;
}
