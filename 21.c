#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashmap.h"

/* $ wc -L 21i2 */
#define MAX_LINE_LENGTH 606

/* $ sed "s/(//g;s/)//g;s/,//g" 21i2 | tr " " "\n" | wc -L */
#define MAX_WORD_LENGTH 10

/* $ sed "s/(//g;s/)//g;s/,//g" 21i2 | tr " " "\n" | wc -l */
#define MAX_INGREDIENTS_COUNT 2838

char data_all_values[MAX_INGREDIENTS_COUNT][MAX_WORD_LENGTH];
size_t data_all_values_count = 0;

typedef struct {
  char ** ingredients_pt;
  size_t ingredients_count;
} Food;

char *
skip_to_next_word(char * line_pt) {
  while (line_pt[0] != ' ' &&
         line_pt[0] != '\n')
    line_pt += 1;

  if (line_pt[0] != '\n')
    while (line_pt[0] == ' ')
      line_pt += 1;

  return line_pt;
}

void
parse_input(char * file_name,
            struct hashmap_s * data_pt,
            char ** ingredients_all_pt,
            size_t * ingredients_all_count) {
  FILE * file = fopen(file_name, "r");

  if (file == NULL) {
    printf("It's a trap!  Does file exists?");
    exit(1);
  }

  size_t i,j;
  char line[MAX_LINE_LENGTH];
  char * line_pt;

  char ingredients[256][MAX_WORD_LENGTH];
  size_t ingredients_count;
  char ingredients_new[256][MAX_WORD_LENGTH];
  size_t ingredients_new_count;

  char allergen[MAX_WORD_LENGTH];
  char * allergen_pt;
  Food * food_pt;

  int collect_allergens_flag;
  void * data_element_pt;
  Food * data_food_pt;

  while (fgets(line, sizeof line, file) != NULL) {
    /* printf(">> %s", line); */

    line_pt = line;
    ingredients_count = 0;
    collect_allergens_flag = 0;

    while (line_pt[0] != '\n') {
      /* printf("line_pt: %s", line_pt); */

      if (collect_allergens_flag) {
        sscanf(line_pt, "%[a-zA-Z]", allergen);
        line_pt = skip_to_next_word(line_pt);
        data_element_pt = hashmap_get(data_pt, allergen, strlen(allergen));

        if (data_element_pt == NULL) {
          allergen_pt = malloc(strlen(allergen));
          strcpy(allergen_pt, allergen);

          food_pt = (Food *) malloc(sizeof(Food));
          food_pt->ingredients_pt = malloc(ingredients_count * sizeof(char *));
          food_pt->ingredients_count = ingredients_count;

          for (i=0; i<ingredients_count; i++) {
            food_pt->ingredients_pt[i] = malloc(strlen(ingredients[i]));
            strcpy(food_pt->ingredients_pt[i], ingredients[i]);
          }

          if (0 != hashmap_put(data_pt, allergen_pt, strlen(allergen),
                               food_pt)) {
            fprintf(stderr, "ERROR: hashmap_put, key: %s\n", allergen);
            exit(1);
          }
        } else {
          data_food_pt = (Food *)data_element_pt;

          /* printf("hashmap: key: '%s' ingredients: ", allergen); */
          /* for (i=0; i < data_food_pt->ingredients_count; i++) */
          /*   printf("%s ", data_food_pt->ingredients_pt[i]); */
          /* printf("\b\n"); */

          ingredients_new_count = 0;

          for (i=0; i < ingredients_count; i++) {
            for (j=0; j < data_food_pt->ingredients_count; j++) {
              if (strcmp(ingredients[i], data_food_pt->ingredients_pt[j]) == 0) {
                strcpy(ingredients_new[ingredients_new_count], ingredients[i]);
                ingredients_new_count++;
              }
            }
          }

          /* printf("ingredients_new: "); */
          /* for (i=0; i < ingredients_new_count; i++) */
          /*   printf("%s ", ingredients_new[i]); */
          /* printf("\b\n"); */

          free(data_food_pt->ingredients_pt);
          data_food_pt->ingredients_pt = malloc(ingredients_new_count * sizeof(char *));
          data_food_pt->ingredients_count = ingredients_new_count;

          for (i=0; i < ingredients_new_count; i++) {
            data_food_pt->ingredients_pt[i] = malloc(strlen(ingredients_new[i]));
            strcpy(data_food_pt->ingredients_pt[i], ingredients_new[i]);
          }
        }
     } else {
        sscanf(line_pt, "%s", ingredients[ingredients_count]);
        line_pt = skip_to_next_word(line_pt);

        ingredients_all_pt[*ingredients_all_count] =
          malloc(strlen(ingredients[ingredients_count]));

        strcpy(ingredients_all_pt[*ingredients_all_count],
               ingredients[ingredients_count]);

        ingredients_count++;
        *ingredients_all_count += 1;

        /* End of ingredients, switch to allergens */
        if (line_pt[0] == '(') {
          /* Skip "contains" word */
          line_pt = skip_to_next_word(line_pt);
          collect_allergens_flag = 1;
        }
      }
    }

    /* printf("ingredients: "); */
    /* for (i=0; i<ingredients_count; i++) printf("%s ", ingredients[i]); */
    /* printf("\b\n"); */
  }

  fclose(file);
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
int
data_log_all(void * const context, struct hashmap_element_s * const element) {
#pragma GCC diagnostic pop
  size_t i;
  Food * food = (Food *)element->data;
  
  for (i=0; i < element->key_len; i++)
    printf("%c", element->key[i]);

  printf(": ");

  for (i=0; i < food->ingredients_count; i++)
    printf("%s ", food->ingredients_pt[i]);

  printf("\b\n");

  return 0;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
int
data_free_all(void * const context, struct hashmap_element_s * const element) {
#pragma GCC diagnostic pop
  Food * food = (Food *)element->data;
  free(food->ingredients_pt);
  free(food);
  return -1;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
int
data_collect_values(void * const context, void * const value) {
#pragma GCC diagnostic pop
  size_t i,j;
  Food * food_pt = (Food *) value;

  for (i=0; i<food_pt->ingredients_count; i++) {
    for (j=0; j<data_all_values_count; j++) {
      if (strcmp(data_all_values[j], food_pt->ingredients_pt[i]) == 0)
        goto skip;
    }
    strcpy(data_all_values[data_all_values_count], food_pt->ingredients_pt[i]);
    data_all_values_count++;
  skip: continue;
  }

  return 1;
}

unsigned int
solve1(char file_name[6]) {
  size_t i,j;
  unsigned int result;
  struct hashmap_s data;
  char ** ingredients_all_pt;
  size_t ingredients_all_count = 0;

  if (0 != hashmap_create(2, &data)) {
    fprintf(stderr, "You shall not pass!  Error while creating hashmap.");
    exit(1);
  }

  ingredients_all_pt = malloc(MAX_INGREDIENTS_COUNT * sizeof(char *));
  parse_input(file_name, &data, ingredients_all_pt, &ingredients_all_count);

  result = ingredients_all_count;

  if (0 != hashmap_iterate_pairs(&data, data_log_all, NULL)) {
    fprintf(stderr, "ERROR: data_log_all\n");
    exit(1);
  }

  if (0 != hashmap_iterate(&data, data_collect_values, NULL)) {
    fprintf(stderr, "ERROR: data_collect_values\n");
    exit(1);
  }

  /* https://youtu.be/CA_N_QVxbKg?t=77 */
  if (0 != hashmap_iterate_pairs(&data, data_free_all, NULL)) {
    fprintf(stderr, "ERROR: data_free_all\n");
    exit(1);
  }

  printf("ingredients_all: ");
  for (i=0; i<ingredients_all_count; i++)
    printf("'%s' ", ingredients_all_pt[i]);
  printf("\b\n");

  printf("data_all_values: ");
  for (i=0; i<data_all_values_count; i++)
    printf("'%s' ", data_all_values[i]);
  printf("\b\n");

  for (i=0; i<data_all_values_count; i++)
    for (j=0; j<ingredients_all_count; j++)
      if (strcmp(data_all_values[i], ingredients_all_pt[j]) == 0)
        result--;

  free(ingredients_all_pt);
  hashmap_destroy(&data);

  return result;
}

int
main() {
  /*
   * mxmxvkd kfcds sqjhc nhms (contains dairy, fish)
   * trh fvjkl sbzzf mxmxvkd (contains dairy)
   * sqjhc fvjkl (contains soy)
   * sqjhc mxmxvkd sbzzf (contains fish)
   * 
   * Result: kfcds, nhms, sbzzf, sbzzf, trh => 5
   */
  printf("%d (5)\n", solve1("21i1"));
  printf("%d (5)\n", solve1("21i2"));

  return 0;
}
