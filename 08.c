#include <stdio.h>
#include <stdlib.h>

typedef struct {
  char type;
  int value;
} instruction;

int
solve1(char file_name[8]) {
  FILE * file = fopen(file_name, "r");
  char line[32];

  char number_char[8];
  instruction instructions[650];
  int i,j;
  int instructions_count = 0;
  int visited[650];
  int visited_count = 0;
  int acc = 0;

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

  /* printf("---\n");  */
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
    case 'a':
      acc += instructions[i].value;
      break;
    case 'j':
      i += instructions[i].value;
      continue;
    }

    i++;
  }
}

int
main() {
  printf("%d (5)\n", solve1("08i1"));
  printf("%d (1744)\n", solve1("08i2"));
  return 0;
}
