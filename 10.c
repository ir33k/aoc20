#include <stdio.h>
#include <stdlib.h>

#define INPUT_MAX_LENGTH 124
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
cmpfunc(const void * a, const void * b) {
  return (*(int*)a - *(int*)b);
}

int
prepare_input(int * output, int * input, int input_length) {
  int i;

  /* sort */
  qsort(input, input_length, sizeof (int), cmpfunc);

  /* add initial 0 */
  output[0] = 0;

  /* put all values of input to output */
  for (i=0; i<input_length; i++) output[i+1] = input[i];

  /* last value max+3  */
  output[input_length+1] = input[input_length-1] + 3;

  /* return outpu array length */
  return input_length+2;
}

int
solve1(int * init_input, int init_length) {
  int input[INPUT_MAX_LENGTH];
  int input_length = prepare_input(input, init_input, init_length);

  int diff_1 = 0;
  int diff_3 = 0;

  for (int i=1; i<input_length; i++)
    (input[i] - input[i-1] == 1) ? diff_1++ : diff_3++;

  return diff_1 * diff_3;
}

unsigned long long int
solve2(int * init_input, int init_length) {
  int input[INPUT_MAX_LENGTH];
  int input_length = prepare_input(input, init_input, init_length);

  int i,j;
  int diff_1_count = 0;
  int tmp = 0;
  unsigned long long int result = 1;
  
  for (i=1; i<input_length; i++) {
    if (input[i] - input[i-1] == 1) {
      diff_1_count++;
    } else {
      for (j=1; j<=diff_1_count-1; j++) tmp += j;
      result *= tmp + 1;
      diff_1_count = tmp = 0;
    }
  }

  return result;
}

int
main() {
  printf("%d (35)\n",   solve1(input1, INPUT_1_LENGTH));
  printf("%d (220)\n",  solve1(input2, INPUT_2_LENGTH));
  printf("%d (2592)\n", solve1(input3, INPUT_3_LENGTH));

  printf("%lld (8)\n",               solve2(input1, INPUT_1_LENGTH));
  printf("%lld (19208)\n",           solve2(input2, INPUT_2_LENGTH));
  printf("%lld (198428693313536)\n", solve2(input3, INPUT_3_LENGTH));
  return 0;
}
