#include <stdio.h>
#include <stdlib.h>

#define INPUT_1_LENGTH 11
#define INPUT_2_LENGTH 31
#define INPUT_3_LENGTH 107

int input1[INPUT_1_LENGTH] = { 16, 10, 15, 5, 1, 11, 7, 19, 6, 12, 4 };

int input2[INPUT_2_LENGTH] = {
  28, 33, 18, 42, 31, 14, 46, 20, 48, 47, 24, 23, 49, 45, 19, 38, 39,
  11, 1, 32, 25, 35, 8, 17, 7, 9, 4, 2, 34, 10, 3
};

int input3[INPUT_3_LENGTH] = {
  83, 69, 170, 56, 43, 111, 117, 135, 136, 176, 154, 65, 107, 169, 141,
  151, 158, 134, 108, 143, 114, 104, 49, 55, 72, 73, 144, 13, 35, 152,
  98, 133, 31, 44, 150, 70, 118, 64, 39, 137, 142, 28, 130, 167, 101,
  100, 120, 79, 153, 157, 89, 163, 177, 3, 1, 38, 16, 128, 18, 62, 76,
  78, 17, 63, 160, 59, 175, 168, 54, 34, 22, 174, 53, 25, 129, 90, 42,
  119, 92, 173, 4, 166, 19, 2, 121, 7, 71, 99, 66, 46, 124, 86, 127,
  159, 12, 91, 140, 52, 80, 45, 33, 9, 8, 77, 147, 32, 95
};

int
cmpfunc (const void * a, const void * b) {
  return (*(int*)a - *(int*)b);
}

int
solve1(int * input, int input_length) {
  int i;
  int diff_1 = 0;
  int diff_3 = 0;

  qsort(input, input_length, sizeof (int), cmpfunc);

  /**/ if (input[0] == 1) diff_1++;
  else if (input[0] == 3) diff_3++;
  
  for (i=1; i<input_length; i++) {
    /**/ if (input[i] - input[i-1] == 1) diff_1++;
    else if (input[i] - input[i-1] == 3) diff_3++;
  }
  diff_3++;

  return diff_1 * diff_3;
}

int
main() {
  printf("%d (7 * 5 = 35)\n", solve1(input1, INPUT_1_LENGTH));
  printf("%d (22 * 10 = 220)\n", solve1(input2, INPUT_2_LENGTH));
  printf("%d (72 * 36 = 2592)\n", solve1(input3, INPUT_3_LENGTH));

  return 0;
}
