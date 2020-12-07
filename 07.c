#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char type[16];              /* light, dotted, faded */
  char color[16];
} bag;

typedef struct {
  unsigned int count;
  bag bag;
} bag_child;

typedef struct {
  bag root;
  bag_child childs[16];
  unsigned int childs_count;
} bag_rule;

int
can_have_bag(bag * target_bag,
             size_t * bag_rules_count,
             bag_rule bag_rules[600],
             bag * target_rule) {
  size_t i, j;

  for (i=0; i<*bag_rules_count; i++) {
    if (strcmp(bag_rules[i].root.type, target_rule->type) == 0 &&
        strcmp(bag_rules[i].root.color, target_rule->color) == 0) {
      printf("%ld %s %s\n", i, bag_rules[i].root.type, bag_rules[i].root.color);
      for (j=0; j<bag_rules[i].childs_count; j++) {
        printf("  %ld %s %s == %s %s\n", j,
               bag_rules[i].childs[j].bag.type,
               bag_rules[i].childs[j].bag.color,
               target_bag->type, target_bag->color);
        if ((strcmp(bag_rules[i].childs[j].bag.type, target_bag->type) == 0 &&
             strcmp(bag_rules[i].childs[j].bag.color, target_bag->color) == 0) ||
            can_have_bag(target_bag,
                         bag_rules_count,
                         bag_rules,
                         &bag_rules[i].childs[j].bag)) {
          printf("->1 \n");
          return 1;
        }
      }
    }
  }

  printf("->0 \n");
  return 0;
}

int
solve1(char input_file_path[8]) {
  int result = 0;
  size_t i, j;

  FILE * file = fopen(input_file_path, "r");
  char line[512];

  char number_str[8];
  bag_rule bag_rules[600];
  size_t bag_rules_count = 0;

  bag my_bag = { "shiny", "gold" };

  /* parse input to gab_rules structure */
  while(fgets(line, sizeof line, file) != NULL) {
    /* get root bag type */
    for (i=0,j=0; line[i] != ' '; i++,j++)
      bag_rules[bag_rules_count].root.type[j] = line[i];

    /* get root bag color */
    for (i++,j=0; line[i] != ' '; i++,j++)
      bag_rules[bag_rules_count].root.color[j] = line[i];

    while (line[++i] != ' ');   /* skip "bags" */
    while (line[++i] != ' ');   /* skip "contains */
    i++;                        /* skip space */

    bag_rules[bag_rules_count].childs_count = 0;

    /* Get childs if next part of the string is not "no other bags" */
    if (line[i] == 'n') {
      bag_rules_count++;
      continue;
    }

    while (1) {
      number_str[0] = 0;

      for (j=0; line[i] != ' '; i++,j++)
        number_str[j] = line[i];

      bag_rules[bag_rules_count]
        .childs[bag_rules[bag_rules_count].childs_count]
        .count = atoi(number_str);

      for (i++,j=0; line[i] != ' '; i++,j++)
        bag_rules[bag_rules_count]
          .childs[bag_rules[bag_rules_count].childs_count]
          .bag.type[j] = line[i];

      for (i++,j=0; line[i] != ' '; i++,j++)
        bag_rules[bag_rules_count]
          .childs[bag_rules[bag_rules_count].childs_count]
          .bag.color[j] = line[i];

      bag_rules[bag_rules_count].childs_count++;

      /* skip "bag[s][,.]" */
      while (line[i] != ',' && line[i] != '.') i++;

      /* break on line end */
      if (line[i] == '.') break;

      /* if there is still more childs then skip space */
      i += 2;
    }

    bag_rules_count++;
  }

  fclose(file);

  /* debug print after parse */
  for (i=0; i<bag_rules_count; i++) {
    printf("%s %s:",
           bag_rules[i].root.type,
           bag_rules[i].root.color);

    for (j=0; j<bag_rules[i].childs_count; j++) {
      printf(" (%d %s %s)",
             bag_rules[i].childs[j].count,
             bag_rules[i].childs[j].bag.type,
             bag_rules[i].childs[j].bag.color);
    }

    printf("\n");
  }

  /* search for my_bag recursively */
  for (i=0; i<bag_rules_count; i++) {
    printf("---\n");
    result += can_have_bag(&my_bag,
                           &bag_rules_count,
                           bag_rules,
                           &bag_rules[i].root);
  }

  return result;
}

int
main() {
  printf("expected 4, got %d\n", solve1("07i1"));
  printf("expected 365?, got %d\n", solve1("07i2")); /* too low */
  return 0;
}
