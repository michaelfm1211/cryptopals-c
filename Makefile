CFLAGS += -Wall -Wextra -Werror -march=native

SRCS := $(wildcard *.c)
OBJS := $(SRCS:.c=.o)

.PHONY: all
all: 1 2 3 4 5 6 7 8 9 10

.PHONY: dev
dev: CFLAGS += -g -fsanitize=address -fsanitize=undefined
dev: all

.PHONY: clean
clean:
	rm -rf $(OBJS) $(TXTS) $(BINS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.txt:
	wget https://cryptopals.com/static/challenge-data/$@

%.bin: %.txt
	base64 -d < $< > $@

1: 1.o common.o
	$(CC) $(CFLAGS) -o $@ $^
2: 2.o common.o
	$(CC) $(CFLAGS) -o $@ $^
3: 3.o common.o
	$(CC) $(CFLAGS) -o $@ $^
4: 4.o common.o 4.txt
	$(CC) $(CFLAGS) -o $@ 4.o common.o
5: 5.o common.o
	$(CC) $(CFLAGS) -o $@ $^
6: 6.o common.o 6.bin
	$(CC) $(CFLAGS) -o $@ 6.o common.o
7: 7.o common.o 7.bin
	$(CC) $(CFLAGS) -o $@ 7.o common.o
8: 8.o common.o 8.txt
	$(CC) $(CFLAGS) -o $@ 8.o common.o
9: 9.o common.o
	$(CC) $(CFLAGS) -o $@ $^
10: 10.o common.o 10.bin
	$(CC) $(CFLAGS) -o $@ 10.o common.o
