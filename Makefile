CFLAGS += -Wall -Wextra -Werror -march=native

SRCS := $(wildcard *.c)
OBJS := $(SRCS:.c=.o)

TXTS = 4.txt 6.txt
BINS = 6.bin

.PHONY: all
all: $(TXTS) $(BINS) 1 2 3 4 5 6

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
4: 4.o common.o
	$(CC) $(CFLAGS) -o $@ $^
5: 5.o common.o
	$(CC) $(CFLAGS) -o $@ $^
6: 6.o common.o 6.bin
	base64 -d < 6.txt > 6.bin
	$(CC) $(CFLAGS) -o $@ 6.o common.o
