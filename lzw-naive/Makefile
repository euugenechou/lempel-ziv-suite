ENCODER  = encode
DECODER  = decode

ENCODERSRC = encode.c trie.c io.c util.c
DECODERSRC = decode.c word.c io.c util.c

ENCODEROBJ = $(ENCODERSRC:%.c=%.o)
DECODEROBJ = $(DECODERSRC:%.c=%.o)

CC       = clang
CFLAGS   = -Wall -Wpedantic -Wextra -Werror -Ofast

.PHONY: all clean spotless infer

all: $(ENCODER) $(DECODER)

$(ENCODER): $(ENCODEROBJ)
	$(CC) -o $@ $^

$(DECODER): $(DECODEROBJ)
	$(CC) -o $@ $^

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

spotless: clean
	rm -rf $(ENCODER) $(DECODER)

clean:
	rm -rf *.o infer-out

infer: spotless
	infer-capture -- make
	infer-analyze -- make
