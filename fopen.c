#include <stdio.h>

int
main() {
  FILE *fp;
  int c;

  fp = fopen("./04a-input2", "r");

  do {
    c = fgetc(fp);
    printf("%c\n", c);
  } while (!feof(fp));

  fclose(fp);

  return 0;
}
