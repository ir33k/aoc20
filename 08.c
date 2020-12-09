#include <stdio.h>
#include <stdlib.h>

#define MAX_INSTRUCTIONS 650
#define MAX_LINE_LENGTH 32

typedef struct {
  char type;
  int value;
  int visited;
} instruction;

int
parse_input(instruction instructions[MAX_INSTRUCTIONS],
            char file_name[8]) {
  int i;
  FILE * file = fopen(file_name, "r");
  char line[MAX_LINE_LENGTH];
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

    if (instructions[i].type == 'a') *acc += instructions[i].value;
    if (instructions[i].type == 'j') i    += instructions[i].value - 1;

    instructions[i++].visited = 1;
  }
  return 1;
}

int
solve1(instruction instructions[MAX_INSTRUCTIONS], int instructions_count) {
  int acc = 0;
  printf(">>>> %d\n", check(instructions, instructions_count, 0, &acc));
  return acc;
}

/* int */
/* solve2(instruction instructions[MAX_INSTRUCTIONS]) { */
/*   int i, j; */
/*   int visited[MAX_INSTRUCTIONS]; */
/*   int visited_count = 0; */
/*   int acc = 0; */
/*   int change_index; */

/*   i = 0; */
/*   while (1) { */
/*     for (j=0; j<visited_count; j++) */
/*       if (visited[j] == i) return acc; */

/*     visited[visited_count] = i; */
/*     visited_count++; */

/*     switch (instructions[i].type) { */
/*     case 'a': acc += instructions[i].value; break; */
/*     case 'j': i   += instructions[i].value; continue; */
/*     } */

/*     i++; */
/*   } */

/*   /\* NOTE thie return is not rly needed *\/ */
/*   return 0; */
/* } */

int
main() {
  instruction instructions1[MAX_INSTRUCTIONS];
  instruction instructions2[MAX_INSTRUCTIONS];
  instruction instructions3[MAX_INSTRUCTIONS];

  int instructions1_count = parse_input(instructions1, "08i1");
  int instructions2_count = parse_input(instructions2, "08i2");
  int instructions3_count = parse_input(instructions3, "08i3");

  printf("%d (5)\n",    solve1(instructions1, instructions1_count));
  printf("%d (1744)\n", solve1(instructions2, instructions2_count));
  printf("%d (8)\n",    solve1(instructions3, instructions3_count));

  /* printf("%d (8)\n",    solve2(instructions1)); */
  /* printf("%d (1744)\n", solve1(instructions2)); */

  return 0;
}
