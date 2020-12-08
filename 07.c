#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MY_BAG_COLOR "shiny gold"

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
bag_index_of(bag bags[1024], int bags_count, char bag_color[32]) {
  for (int i=0; i<bags_count; i++) {
    if (strcmp(bags[i].color, bag_color) == 0)
      return i;
  }

  return -1;
}

void
bag_count_unique_parent_colors(int * colors_count,
                               char colors[600][32],
                               bag * current_bag) {
  int i,j;

  for (i=0; i<current_bag->parents_count; i++) {
    for (j=0; j < * colors_count; j++) {
      if (strcmp(colors[j], current_bag->parents[i]->color) == 0) {
        j = -1;
        break;
      }
    }

    if (j > -1) {
      strcpy(colors[j], current_bag->parents[i]->color);
      * colors_count += 1;
    }

    bag_count_unique_parent_colors(colors_count, colors, current_bag->parents[i]);
  }
}

int
bag_count_chinds_inside(bag * current_bag) {
  int i, sum = 1;

  for (i=0; i<current_bag->childs_count; i++) {
    sum += current_bag->childs[i].count *
      bag_count_chinds_inside(current_bag->childs[i].bag);
  }

  return sum;
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

void
solve1(char input_file_path[8]) {
  int i;

  FILE * file = fopen(input_file_path, "r");
  char line[512];

  char bag_color[32];
  char number_str[8];
  bag bags[1024];
  int bags_count = 0;
  int index_of;
  bag * parent;
  bag * child;

  while(fgets(line, sizeof line, file) != NULL) {
    /* printf(">>>> %s", line); */
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

    i = skip_to_space(line, i);
    i = skip_to_space(line, i);

    if (line[i] == 'n') continue;

    while (1) {
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

      if (i < 0) break;
    }
  }

  fclose(file);

  /* debug prints */
  /* for (i=0; i<bags_count; i++) { */
  /*   int j; */

  /*   printf("%s %d %d\n", */
  /*          bags[i].color, */
  /*          bags[i].parents_count, */
  /*          bags[i].childs_count); */

  /*   for (j=0; j<bags[i].parents_count; j++) */
  /*     printf("  P %d %s\n", j, bags[i].parents[j]->color); */

  /*   for (j=0; j<bags[i].childs_count; j++) */
  /*     printf("  C %d (%d) %s\n", j, */
  /*            bags[i].childs[j].count, */
  /*            bags[i].childs[j].bag->color); */
  /* } */

  char colors[600][32];
  int unique_parents_count = 0;
  int childs_inside_count = 0;

  index_of = bag_index_of(bags, bags_count, MY_BAG_COLOR);

  /* First solution */
  bag_count_unique_parent_colors(&unique_parents_count, colors, &bags[index_of]);
  printf("unique parents: %d\n", unique_parents_count);

  /* Second solution */
  childs_inside_count = bag_count_chinds_inside(&bags[index_of]);
  childs_inside_count--;	/* Remove one bag (my bag) */

  printf("childs inside: %d\n\n", childs_inside_count);
}

int
main() {
  solve1("07i1");		/* 4, 32 */
  solve1("07i2");		/* 370, 29547 */
  solve1("07i3");		/* 0, 126 */
  return 0;
}
