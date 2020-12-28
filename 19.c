#include <stdio.h>
#include <stdlib.h>

#define RULES_COUNT 256

enum RuleType {
  RULETYPE_EMPTY = 0,
  RULETYPE_VALUE = 1,
  RULETYPE_RULES = 2,
};

typedef struct {
  enum RuleType type;
  char value;
  int have_pipe;
  int rules1[4];
  int rules2[4];
  unsigned short rules1_count;
  unsigned short rules2_count;
} Rule;

int
is_valid_message(Rule * rules, int rule_index, char ** message_pt,
                 int is_last_rule) {
  unsigned short i;
  char * starting_point = *message_pt;

  if (rules[rule_index].type == RULETYPE_VALUE) {
    if (rules[rule_index].value == *message_pt[0]) {
      *message_pt += 1;
      return 1;
    }

    return 0;
  }

  for (i=0; i<rules[rule_index].rules1_count; i++)
    if (!is_valid_message(rules, rules[rule_index].rules1[i], message_pt,
                          is_last_rule && i+1 == rules[rule_index].rules1_count))
      goto invalid1;

  if (*message_pt[0] == '\n' && !is_last_rule)
    goto invalid1;

  return 1;

 invalid1: *message_pt = starting_point;

  /* Try again for second part of rules if available */
  if (!rules[rule_index].have_pipe)
    return 0;

  for (i=0; i<rules[rule_index].rules2_count; i++)
    if (!is_valid_message(rules, rules[rule_index].rules2[i], message_pt,
                          is_last_rule && i+1 == rules[rule_index].rules2_count))
      goto invalid2;

  if (*message_pt[0] == '\n' && !is_last_rule)
    goto invalid2;

  return 1;

 invalid2: *message_pt = starting_point;

  return 0;
}

unsigned long
solve1(char file_name[6], int start_index) {
  FILE * file = fopen(file_name, "r");

  if (file == NULL) exit(1);

  unsigned long result = 0;
  char line[256];
  char * message_pt;
  int parse_rules_flag = 1;
  int rule_index;
  char rule[256];
  char * rule_pt;
  Rule rules[RULES_COUNT] = { 0 };

  while (fgets(line, sizeof line, file) != NULL) {
    if (parse_rules_flag) {
      /* End parsing */
      if (line[0] == '\n') {
        parse_rules_flag = 0;
        continue;
      }

      rule_pt = rule;;
      sscanf(line, "%d: %[^\n]\n", &rule_index, rule_pt);

      if (rule_pt[1] == 'a' || rule_pt[1] == 'b') {
        rules[rule_index].type = RULETYPE_VALUE;
        rules[rule_index].value = rule_pt[1];
      } else {
        rules[rule_index].type = RULETYPE_RULES;
        rules[rule_index].have_pipe = 0;
        rules[rule_index].rules1_count = 0;
        rules[rule_index].rules2_count = 0;

        while (1) {
          if (rule_pt[0] == '|') {
            rules[rule_index].have_pipe = 1;
            rule_pt += 2;
            continue;
          }

          if (rules[rule_index].have_pipe)
            sscanf(rule_pt, "%d",
                   &rules[rule_index].rules2[rules[rule_index].rules2_count++]);
          else
            sscanf(rule_pt, "%d",
                   &rules[rule_index].rules1[rules[rule_index].rules1_count++]);

          while (rule_pt[0] != ' ' && rule_pt[0] != '\n' && rule_pt[0] != '\0')
            rule_pt += 1;

          if (rule_pt[0] == '\n' || rule_pt[0] == '\0') break;
          else rule_pt += 1;
        }
      }
    } else {
      message_pt = line;
      if (is_valid_message(rules, start_index, &message_pt, 1)  && message_pt[0] == '\n')
        result++;
    }
  }

  fclose(file);
  return result;
}

int
main() {
  printf("%lu\t(3)\n",   solve1("19i1", 0));
  printf("%lu\t(156)\n", solve1("19i2", 0));
  printf("%lu\t(160)\n", solve1("19i3", 0));
  printf("%ld\t(1)\n",   solve1("19i4", 0));

  /* printf("%ld (156)\n", solve1("19i5", 0)); */
  /* printf("%ld (12)\n", solve1("19i6", 0)); */

  return 0;
}
