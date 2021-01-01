#include <stdio.h>
#include <stdlib.h>

#define RINGBUFFER_SIZE 64

struct ringbuffer {
  int buffer[RINGBUFFER_SIZE];
  size_t start;
  size_t size;
};

void
ringbuffer_init(struct ringbuffer * ringbuffer_pt) {
  for (size_t i=0; i<RINGBUFFER_SIZE; i++) ringbuffer_pt->buffer[i] = 0;
  ringbuffer_pt->start = 0;
  ringbuffer_pt->size = 0;
}

void
ringbuffer_push(struct ringbuffer * ringbuffer_pt, int value) {
  size_t index = ringbuffer_pt->start + ringbuffer_pt->size;

  if (index >= RINGBUFFER_SIZE)
    index -= RINGBUFFER_SIZE;

  ringbuffer_pt->buffer[index] = value;
  ringbuffer_pt->size += 1;
}

int
ringbuffer_shift(struct ringbuffer * ringbuffer_pt) {
  if (ringbuffer_pt->size == 0) return 0;

  int value = ringbuffer_pt->buffer[ringbuffer_pt->start];

  ringbuffer_pt->buffer[ringbuffer_pt->start] = 0;
  ringbuffer_pt->start++;
  ringbuffer_pt->size--;

  if (ringbuffer_pt->start >= RINGBUFFER_SIZE)
    ringbuffer_pt->start -= RINGBUFFER_SIZE;

  return value;
}

void
parse_player(FILE * file, struct ringbuffer * ringbuffer_pt) {
  char line[12];
  int value;
 
 while (fgets(line, sizeof line, file) != NULL) {
    if (line[0] == 'P')
      break;
  }

  while (fgets(line, sizeof line, file) != NULL) {
    if (line[0] == '\n') break;

    sscanf(line, "%d", &value);
    ringbuffer_push(ringbuffer_pt, value);
  }
}

void
ringbuffer_log_all(struct ringbuffer * ringbuffer_pt) {
  for (size_t i=0; i<RINGBUFFER_SIZE; i++)
    printf("%d ", ringbuffer_pt->buffer[i]);
  printf("\b\n");
}

unsigned int
solve1(char file_name[6]) {
  FILE * file = fopen(file_name, "r");
  unsigned int result = 0;

  if (file == NULL) {
    printf("It's a trap!  Does file exists?");
    exit(1);
  }

  struct ringbuffer player1;
  struct ringbuffer player2;

  ringbuffer_init(&player1);
  ringbuffer_init(&player2);

  parse_player(file, &player1);
  parse_player(file, &player2);

  fclose(file);

  int player1_card;
  int player2_card;

  while (player1.size != 0 && player2.size != 0) {
    player1_card = ringbuffer_shift(&player1);
    player2_card = ringbuffer_shift(&player2);

    if (player1_card > player2_card) {
      ringbuffer_push(&player1, player1_card);
      ringbuffer_push(&player1, player2_card);
    } else {
      ringbuffer_push(&player2, player2_card);
      ringbuffer_push(&player2, player1_card);
    }
  }

  struct ringbuffer * winner_pt = player1.size != 0 ? &player1: &player2;

  printf("winner cards: ");
  ringbuffer_log_all(winner_pt);

  size_t i = winner_pt->start;
  size_t counter = winner_pt->size;

  while (counter > 0) {
    result += winner_pt->buffer[i++] * counter--;
    if (i >= RINGBUFFER_SIZE) i -= RINGBUFFER_SIZE;
  }

  return result;
}

int
main() {
  printf("%u (306)\n", solve1("22i1"));
  printf("%u (32448)\n", solve1("22i2"));
  return 0;
}
