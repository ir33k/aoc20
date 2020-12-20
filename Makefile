CFLAGS = -std=c99 -pedantic -Wall -Werror -Wextra -Os
CC = gcc

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $@ $^ -lm
	./$@
