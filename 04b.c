#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FIELDS_COUNT 7          /* cid is ignored */

int
is_digit(char c) {
  return c >= '0' && c <= '9';
}

int
has_only_digits(char s[256]) {
  for (size_t i = 0; s[i] != '\0'; i++)
    if (!is_digit(s[i]))
      return 0;

  return 1;
}

int
is_field_value_correct(char type[4], char value[32]) {
  size_t i;
  unsigned int length = 0;

  while (value[length] != '\0') length++;

  /* (Birth Year) */
  /* four digits; at least 1920 and at most 2002. */
  if (strcmp("byr", type) == 0) {
    if (length == 4 && has_only_digits(value)) {
      int year = atoi(value);
      return year >= 1920 && year <= 2002;
    }
    return 0;
  }

  /* (Issue Year) */
  /* four digits; at least 2010 and at most 2020. */
  if (strcmp("iyr", type) == 0) {
    if (length == 4 && has_only_digits(value)) {
      int year = atoi(value);
      return year >= 2010 && year <= 2020;
    }
    return 0;
  }

  /* (Expiration Year) */
  /* four digits; at least 2020 and at most 2030. */
  if (strcmp("eyr", type) == 0) {
    if (length == 4 && has_only_digits(value)) {
      int year = atoi(value);
      return year >= 2020 && year <= 2030;
    }
    return 0;
  }

  /* (Height) */
  /* a number followed by either cm or in: */
  /* If cm, the number must be at least 150 and at most 193. */
  /* If in, the number must be at least 59 and at most 76. */
  if (strcmp("hgt", type) == 0) {
    /* cm */
    if (value[length-2] == 'c' && value[length-1] == 'm') {
      value[length-2] = '\0';
      if (has_only_digits(value)) {
        int number = atoi(value);
        return number >= 150 && number <= 193;
      }
    }

    /* in */
    if (value[length-2] == 'i' && value[length-1] == 'n') {
      value[length-2] = '\0';
      if (has_only_digits(value)) {
        int number = atoi(value);
        return number >= 59 && number <= 76;
      }
    }

    return 0;
  }

  /* (Hair Color) */
  /* a # followed by exactly six characters 0-9 or a-f. */
  if (strcmp("hcl", type) == 0) {
    if (value[0] == '#' && length == 7) {
      for (i=1; i<length; i++) {
        if (!(is_digit(value[i]) ||
              (value[i] >= 'a' && value[i] <= 'f'))) {
          return 0;
        }
      }
      return 1;
    }
    return 0;
  }

  /* (Eye Color) */
  /* exactly one of: amb blu brn gry grn hzl oth. */
  if (strcmp("ecl", type) == 0) {
    return (strcmp("amb", value) == 0 ||
            strcmp("blu", value) == 0 ||
            strcmp("brn", value) == 0 ||
            strcmp("gry", value) == 0 ||
            strcmp("grn", value) == 0 ||
            strcmp("hzl", value) == 0 ||
            strcmp("oth", value) == 0);
  }

  /* (Passport ID) */
  /* a nine-digit number, including leading zeroes. */
  if (strcmp("pid", type) == 0) {
    return length == 9 && has_only_digits(value);
  }

  return 0;
}

int
solve(char input_file_path[32]) {
  size_t i;
  char field_type[4];
  char field_value[32];
  int valid_fields_count = 0;
  int valid_passports_count = 0;
  FILE *input_file_pointer;
  char input_char;

  input_file_pointer = fopen(input_file_path, "r");
  input_char = fgetc(input_file_pointer);

  while (1) {
    /* Skip new line char, if we have second new line char in next
       condition or EOF that means we reach end of passport data. */
    if (input_char == '\n') {
      input_char = fgetc(input_file_pointer);
    }

    /* End of passport, validate it. */
    if (input_char == '\n' || feof(input_file_pointer)) {
      if (valid_fields_count >= FIELDS_COUNT) {
        valid_passports_count++;
      }

      /* EOF, return result and close input files. */
      if (feof(input_file_pointer)) {
        fclose(input_file_pointer);
        return valid_passports_count;
      }

      valid_fields_count = 0;
      continue;
    }

    /* If we have no whitespace char that means we can parse field
       with its value. */
    if (input_char == ' ') {
      input_char = fgetc(input_file_pointer);
    } else {
      /* Get field_type */
      for (i=0; input_char != ':'; i++) {
        field_type[i] = input_char;
        input_char = fgetc(input_file_pointer);
      }
      field_type[i] = '\0';

      /* Skip ":" (field_type field_value separator) */
      input_char = fgetc(input_file_pointer);

      /* Get field_value */
      for (i=0; input_char != ' ' && input_char != '\n' && !feof(input_file_pointer); i++) {
        field_value[i] = input_char;
        input_char = fgetc(input_file_pointer);
      }
      field_value[i] = '\0';

      /* Check if field value is correct for given field type */
      if (is_field_value_correct(field_type, field_value))
        valid_fields_count++;
    }
  }
}

int
main() {
  printf("%d (2)\n",   solve("./04input1")); /* 2 */
  printf("%d (0)\n",   solve("./04input3")); /* 0 */
  printf("%d (4)\n",   solve("./04input4")); /* 4 */
  printf("%d (103)\n", solve("./04input2")); /* 103 */

  return 0;
}
