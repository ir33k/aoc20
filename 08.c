#include <stdio.h>
#include <stdlib.h>

#define MAX_INSTRUCTIONS 650

typedef struct {
  char type;
  int value;
  int visited;
} instruction;

int
parse_input(instruction instructions[MAX_INSTRUCTIONS], char file_name[8]) {
  int i;
  FILE * file = fopen(file_name, "r");
  char line[32];
  char number_char[8];
  int instructions_count = 0;

  if (file == NULL) return -1;

  while(fgets(line, sizeof line, file) != NULL) {
    for (i=0; line[i+4] != '\n'; i++) number_char[i] = line[i+4];
    number_char[i] = 0;

    instructions[instructions_count].type    = line[0];
    instructions[instructions_count].value   = atoi(number_char);
    instructions[instructions_count].visited = 0;

    instructions_count++;
  }

  fclose(file);
  return instructions_count;
}

int
check(instruction instructions[MAX_INSTRUCTIONS], int instructions_count,
      int i, int * acc) {
  while (i < instructions_count) {
    if (instructions[i].visited) return 0;

    instructions[i].visited = 1;
    if (instructions[i].type == 'a') *acc += instructions[i].value;
    if (instructions[i].type == 'j') i    += instructions[i].value - 1;
    i++;
  }
  return 1;
}

int
solve1(char file_name[8]) {
  instruction instructions[MAX_INSTRUCTIONS];
  int instructions_count = parse_input(instructions, file_name);
  int acc = 0;
  check(instructions, instructions_count, 0, &acc);
  return acc;
}


int
solve2(char file_name[8]) {
  int i;
  instruction instructions[MAX_INSTRUCTIONS];
  int instructions_count = parse_input(instructions, file_name);
  int acc;
  int change_index = 0;

  while (1) {
    acc = 0;
    i = change_index;

    while (instructions[i].type == 'a') i++;

    instructions[i].type = instructions[i].type == 'j' ? 'n': 'j';
    change_index = i;

    if (check(instructions, instructions_count, 0, &acc)) break;

    for (i=0; instructions[i].visited == 1; i++) {
      instructions[i].visited = 0;
      if (instructions[i].type == 'j') i += instructions[i].value - 1;
    }

    instructions[change_index].type =
      instructions[change_index].type == 'j' ? 'n': 'j';

    change_index++;
  }

  return acc;
}

int
main() {
  printf("%d (5)\n",    solve1("08i1"));
  printf("%d (1744)\n", solve1("08i2"));
  printf("%d (8)\n",    solve1("08i3"));

  printf("%d (8)\n",    solve2("08i1"));
  printf("%d (1174)\n", solve2("08i2"));

  return 0;
}
