CFLAGS = -std=c99 -pedantic -Wall -Werror -Wextra -Os
CC = gcc

$(TARGET): $(TARGET).c hashmap.h linked_list.h
	$(CC) $(CFLAGS) -o $@ $^ -lm
	./$@
