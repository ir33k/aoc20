CFLAGS = -std=c99 -pedantic -Wall -Werror -Wextra -Os
CC = gcc

$(TARGET): $(TARGET).c hashmap.h
	$(CC) $(CFLAGS) -o $@ $^ -lm
	./$@
