#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _bag {
  char color[32];
  int parents_count;
  int childs_count;
  struct _bag * parents[64];
  struct {
    int count;
    struct _bag * bag;
  } childs[64];
} bag;

int
bag_is_same(bag * a, bag * b) {
  return strcmp(a->color, b->color) == 0;
}

int
bag_index_of(bag bags[1024], int bags_count, char bag_color[32]) {
  for (int i=0; i<bags_count; i++) {
    if (strcmp(bags[i].color, bag_color) == 0)
      return i;
  }

  return -1;
}

int
get_till_space(char target[32], char line[512], int start_index) {
  int i = start_index;
  int j = 0;

  for (j=0; line[i] != ' '; i++,j++) target[j] = line[i];
  target[j] = 0;

  return ++i;
}

int
bag_get_color(char target[32], char line[512], int start_index) {
  int i = start_index;
  int j = 0;

  for (j=0; line[i] != ' '; i++,j++) target[j] = line[i];
  target[j] = line[i];     /* add space */
  for (++i,++j; line[i] != ' '; i++,j++) target[j] = line[i];
  target[j] = 0;

  return ++i;
}

int
skip_to_space(char line[512], int start_index) {
  int i = start_index;
  while (line[i] != ' ' && line[i] != '.') i++;
  return line[i] == '.' ? -1 : ++i;
}

int
solve1(char input_file_path[8]) {
  int result = 0;
  int i, j;

  FILE * file = fopen(input_file_path, "r");
  char line[512];

  char bag_color[32];
  char number_str[8];
  bag bags[1024];
  int bags_count = 0;
  int index_of;
  bag * parent;
  bag * child;

  /* bag my_bag = { "shiny gold" }; */

  while(fgets(line, sizeof line, file) != NULL) {
    printf(">>>> %s", line);
    i = bag_get_color(bag_color, line, 0);

    if ((index_of = bag_index_of(bags, bags_count, bag_color)) < 0) {
      strcpy(bags[bags_count].color, bag_color);
      bags[bags_count].parents_count = 0;
      bags[bags_count].childs_count = 0;
      parent = &bags[bags_count];
      bags_count++;
    } else {
      parent = &bags[index_of];
    }

    /* printf(">> %s\n", parent->color); */

    i = skip_to_space(line, i);
    i = skip_to_space(line, i);

    if (line[i] == 'n') continue;

    while (1) {
      /* TODO I forgot to sotre number_str in bag struct */
      i = get_till_space(number_str, line, i);
      i = bag_get_color(bag_color, line, i);
      i = skip_to_space(line, i);

      if ((index_of = bag_index_of(bags, bags_count, bag_color)) < 0) {
        strcpy(bags[bags_count].color, bag_color);
        bags[bags_count].parents_count = 0;
        bags[bags_count].childs_count = 0;
        child = &bags[bags_count];
        bags_count++;
      } else {
        child = &bags[index_of];
      }

      child->parents[child->parents_count] = parent;
      child->parents_count++;

      parent->childs[parent->childs_count].count = atoi(number_str);
      parent->childs[parent->childs_count].bag = child;
      parent->childs_count++;

      /* printf("%s %s\n", number_str, bag_color); */
      if (i < 0) break;
    }
  }

  for (i=0; i<bags_count; i++) {
    printf("%s %d %d\n",
           bags[i].color,
           bags[i].parents_count,
           bags[i].childs_count);

    for (j=0; j<bags[i].parents_count; j++)
      printf("  P %d %s\n", j, bags[i].parents[j]->color);

    for (j=0; j<bags[i].childs_count; j++)
      printf("  C %d (%d) %s\n", j,
             bags[i].childs[j].count,
             bags[i].childs[j].bag->color);
  }

  fclose(file);
  return result;
}

int
main() {
  printf("expected    4, got %d\n", solve1("07i1"));
  /* printf("expected 365?, got %d\n", solve1("07i2")); */
  return 0;
}
