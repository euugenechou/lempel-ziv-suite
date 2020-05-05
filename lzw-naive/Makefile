CC      = clang
CFLAGS 	= -Wall -Wpedantic -Wextra -Werror -Ofast

.PHONY: all clean

all: encode decode

encode: encode.o lzw.o word.o trie.o io.o util.o
	$(CC) -o $@ $^

decode: decode.o lzw.o word.o trie.o io.o util.o
	$(CC) -o $@ $^

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f encode decode *.o
