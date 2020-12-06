#include <stdio.h>
#include <string.h>

int
solve1(char file_path[32]) {
  int result = 0;
  int i, j;
  FILE * file = fopen(file_path, "r");
  char line[64], questions_list[1024] = "";

  if (file == NULL) return -1;

  while(fgets(line, sizeof line, file) != NULL) {
    /* End of group */
    if (line[0] == '\n') {
      /* printf("EOG %s %ld\n", questions_list, strlen(questions_list)); */
      result += strlen(questions_list);
      questions_list[0] = 0;
    }

    /* printf("line > %s", line); */
    for (i=0; line[i] != '\n'; i++) {
      for (j=0; questions_list[j] != 0; j++) {
        if (line[i] == questions_list[j]) {
          /*
           * -1 is used as indicator that character is already in
           * questions list
           */
          j = -1;
          break;
        }
      }

      /* Add question to list */
      if (j > -1) {
        questions_list[j] = line[i];
        questions_list[j+1] = 0;
      }
    }
  }

  fclose(file);
  return result += strlen(questions_list);
}

int
solve2(char file_path[32]) {
  size_t i, j;
  int result = 0;
  int lines_count = 0;
  FILE * file = fopen(file_path, "r");
  char line[64];

  /*
   * Simple map construction with 2 arrays.  Keys are single
   * characters and values are integers with coresponding indexes.
   */
  char map_keys[1024] = "";
  int map_values[1024] = { 0 };

  if (file == NULL) return -1;

  while(fgets(line, sizeof line, file) != NULL) {
    /*
     * End of group.  **IMPORTANT NOTE** input files ends with 2 empty
     * lines to make simpler code.  Thanks to that we avoid issue with
     * not adding result of last group to final result.  Without
     * double blank line at the end we would miss one last loop in
     * this while.
     */
    if (line[0] == '\n') {
      /*
       * Go key by key in map.  If vale of key is equal to number of
       * lines we increment the result.  At the same time all map
       * values are reset to 0.  After the loop map_keys are also
       * reset by moving '\0' to first index.
       */
      for (i=0; map_keys[i] != 0; i++) {
        if (map_values[i] == lines_count) result++;
        map_values[i] = 0;
      }
      map_keys[0] = 0;
      lines_count = 0;
      continue;
    }

    for (i=0; line[i] != '\n'; i++) {
      /* Increment letter counter if letter is laready in map */
      for (j=0; map_keys[j] != 0; j++) {
        if (line[i] == map_keys[j]) {
          map_values[j]++;
          break;
        }
      }

      /* Add new letter to map when it wasn't already found */
      if (map_keys[j] == 0) {
        map_keys[j] = line[i];
        map_keys[j+1] = 0;
        map_values[j] = 1;
      }
    }

    lines_count++;
  }

  fclose(file);
  return result;
}

int
main() {
  /* first */
  printf("Expect   11, got %d\n", solve1("06i1"));
  printf("Expect 6542, got %d\n", solve1("06i2"));

  /* second */
  printf("Expect    6, got %d\n", solve2("06i1"));
  printf("Expect 3299, got %d\n", solve2("06i2"));

  return 0;
}
