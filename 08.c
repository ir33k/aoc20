#include <stdio.h>

int
solve1(char file_name[8]) {
  FILE * file = fopen(file_name, "r");
  char line[32];

  char instructions[650][32];
  int i;
  int instructions_count = 0;

  if (file == NULL) return -1;

  while(fgets(line, sizeof line, file) != NULL) {
    printf("> %s", line);

    for(i=0; line[i] != '\n'; i++)
      instructions[instructions_count][i] = line[i];
    instructions[instructions_count][i] = 0;
    
    instructions_count++;
  }

  // TODO printf
  
  fclose(file);
  return 0;
}

int
main() {
  solve1("08i1");
  /* solve1("08i2"); */
  return 0;
}
