#include <stdio.h>
#include <stdlib.h>

unsigned long
calc(char ** pt) {
  unsigned long result = 0;
  char number_char[64];
  int number_char_length = 0;
  char operator = ' ';

  while (1) {
    if (*pt[0] == ' ' || *pt[0] == ')' || *pt[0] == '\n' || *pt[0] == '\0') {
      if (*pt[0] == ' ' && number_char_length == 0) {
        *pt += 1;
        continue;
      }

      if (number_char_length > 0) {
        number_char[number_char_length] = 0;

        /**/ if (result == 0)     result  = atoi(number_char);
        else if (operator == '+') result += atoi(number_char);
        else if (operator == '*') result *= atoi(number_char);
      }

      if (*pt[0] == ')' || *pt[0] == '\n' || *pt[0] == '\0') {
        if (*pt[0] == ')') *pt += 1;
        return result;
      }

      number_char[0] = 0;
      number_char_length = 0;
      *pt += 1;
      continue;
    }

    if (*pt[0] == '*' || *pt[0] == '+') {
      operator = *pt[0];
      *pt += 1;
      while (*pt[0] == ' ') *pt += 1;
      continue;
    }

    if (*pt[0] == '(') {
      *pt += 1;

      /**/ if (result == 0)     result  = calc(pt);
      else if (operator == '+') result += calc(pt);
      else if (operator == '*') result *= calc(pt);

      number_char[0] = 0;
      number_char_length = 0;
      continue;
    }

    if (*pt[0] == ' ') {
      *pt += 1;
      continue;
    }

    number_char[number_char_length] = *pt[0];
    number_char_length++;
    *pt += 1;
  }
}

unsigned long
compute(char input[1024]) {
  char * input_pt = input;
  return calc(&input_pt);
}

unsigned long
solve1(char file_name[6]) {
  FILE * file = fopen(file_name, "r");
  char line[1024];
  unsigned long result = 0;

  if (file == NULL) return 0;

  while (fgets(line, sizeof line, file) != NULL)
    result += compute(line);

  fclose(file);
  return result;
}

int
main() {
  printf("%lu (6)\n",     compute("2 * 3"));
  printf("%lu (6)\n",     compute("  2   *   3   "));
  printf("%lu (5)\n",     compute("2 + 3"));
  printf("%lu (5)\n",     compute("2 + (3)"));
  printf("%lu (5)\n",     compute("(2) + (3)"));
  printf("%lu (17)\n",    compute("2 + (3 * (1 + 4 ) )"));
  printf("%lu (5)\n",     compute("((2) + (3))"));
  printf("%lu (8)\n",     compute("((2) + (3 * 2))"));

  printf("%lu (26)\n",    compute("2 * 3 + (4 * 5)"));
  printf("%lu (437)\n",   compute("5 + (8 * 3 + 9 + 3 * 4 * 3)"));
  printf("%lu (12240)\n", compute("5 * 9 * (7 * 3 * 3 + 9 * 3 + (8 + 6 * 4))"));
  printf("%lu (13632)\n", compute("((2 + 4 * 9) * (6 + 9 * 8 + 6) + 6) + 2 + 4 * 2"));

  printf("%lu (30753705453324)\n", solve1("18i2"));

  return 0;
}
