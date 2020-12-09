#include <stdio.h>
#include <stdlib.h>

#define MAX_INSTRUCTIONS 650
#define MAX_LINE_LENGTH 32

typedef struct {
  char type;
  int value;
} instruction;

int
parse_input(instruction instructions[MAX_INSTRUCTIONS],
            char file_name[8]) {
  int i,j;

  FILE * file = fopen(file_name, "r");
  char line[MAX_LINE_LENGTH];

  char number_char[8];
  int instructions_count = 0;

  if (file == NULL) return -1;

  while(fgets(line, sizeof line, file) != NULL) {
    instructions[instructions_count].type = line[0];

    for (i=5,j=0; line[i] != '\n'; i++,j++) {
      number_char[j] = line[i];
    }
    number_char[j] = 0;
    instructions[instructions_count].value = atoi(number_char);

    if (line[4] == '-')
      instructions[instructions_count].value *= -1;
    
    instructions_count++;
  }

  fclose(file);

  /* printf("---\n"); */
  /* for(i=0; i<instructions_count; i++) */
  /*   printf("%c: %d\n", */
  /*          instructions[i].type, */
  /*          instructions[i].value); */

  return instructions_count;
}

int
solve1(instruction instructions[MAX_INSTRUCTIONS]) {
  int i, j;
  int visited[650];
  int visited_count = 0;
  int acc = 0;

  i=0;
  while (1) {
    /* printf("%c: %d\t%d\n", */
    /*        instructions[i].type, */
    /*        instructions[i].value, */
    /*        acc); */

    for (j=0; j<visited_count; j++)
      if (visited[j] == i) return acc;

    visited[visited_count] = i;
    visited_count++;

    switch (instructions[i].type) {
    case 'a': acc += instructions[i].value; break;
    case 'j': i   += instructions[i].value; continue;
    }

    i++;
  }
}

int
solve2() {
  return 0;
}

int
main() {
  instruction instructions1[MAX_INSTRUCTIONS];
  instruction instructions2[MAX_INSTRUCTIONS];

  parse_input(instructions1, "08i1");
  parse_input(instructions2, "08i2");

  printf("%d (5)\n",    solve1(instructions1));
  printf("%d (1744)\n", solve1(instructions2));

  printf("%d (8)\n",    solve2(instructions1));
  /* printf("%d (1744)\n", solve1(instructions2)); */

  return 0;
}
