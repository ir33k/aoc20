#include <stdio.h>
#include <string.h>

#define FIELDS_COUNT 7          /* cid is ignored in this example */

int
solve(char input_file_path[32]) {
  size_t i;
  char field[64];
  int valid_fields_count = 0;
  int valid_passports_count = 0;
  int get_field_flag = 1;

  char required_fields[FIELDS_COUNT][4] = {
    "byr", /* (Birth Year) */
    "iyr", /* (Issue Year) */
    "eyr", /* (Expiration Year) */
    "hgt", /* (Height) */
    "hcl", /* (Hair Color) */
    "ecl", /* (Eye Color) */
    "pid", /* (Passport ID) */
    /* "cid",  (Country ID) */
  };

  FILE *input_file_pointer;
  char input_char;
  input_file_pointer = fopen(input_file_path, "r");

  while (1) {
    input_char = fgetc(input_file_pointer);

    if (input_char == ' ') {
      get_field_flag = 1;
      continue;
    }

    if (input_char == '\n') {
      get_field_flag = 1;
      input_char = fgetc(input_file_pointer);
    }

    if (input_char == '\n' || feof(input_file_pointer)) {
      if (valid_fields_count >= FIELDS_COUNT) {
        valid_passports_count++;
      }

      valid_fields_count = 0;
      
      if (feof(input_file_pointer)) {
        fclose(input_file_pointer);
        return valid_passports_count;
      }

      continue;
    }

    if (get_field_flag) {
      for (i=0; input_char != ':'; i++) {
        field[i] = input_char;
        input_char = fgetc(input_file_pointer);
      }
      field[i] = '\0';

      for (i=0; i < FIELDS_COUNT; i++) {
        if (strcmp(required_fields[i], field) == 0) {
          valid_fields_count++;
          break;
        }
      }

      get_field_flag = 0;
    }
  }
}

int
main() {
  printf("%d\n", solve("./04input1")); /* 2 */
  printf("%d\n", solve("./04input2")); /* 170 */

  return 0;
}
