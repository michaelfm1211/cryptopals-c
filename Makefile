CFLAGS += -Wall -Wextra -Werror

SRCS := $(wildcard *.c)
OBJS := $(SRCS:.c=.o)

.PHONY: all
all: 1 2 3 4 5

.PHONY: dev
dev: CFLAGS += -g -fsanitize=address -fsanitize=undefined
dev: all

.PHONY: clean
clean:
	rm -rf $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

1: 1.o common.o
	$(CC) $(CFLAGS) -o $@ $^
2: 2.o common.o
	$(CC) $(CFLAGS) -o $@ $^
3: 3.o common.o
	$(CC) $(CFLAGS) -o $@ $^
4: 4.o common.o
	$(CC) $(CFLAGS) -o $@ $^
5: 5.o common.o
	$(CC) $(CFLAGS) -o $@ $^
