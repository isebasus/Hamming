EXECA = encode
EXECB = decode
UTIL = ./src/util
HAMMING = ./src/hamming

SOURCEA = $(UTIL)/bm.o $(UTIL)/bv.o $(HAMMING)/hamming.o src/encode.o
SOURCEB = $(UTIL)/bm.o $(UTIL)/bv.o $(HAMMING)/hamming.o src/decode.o

OBJECTSA = $(SOURCEA:%.c=%.o)
OBJECTSB = $(SOURCEB:%.c=%.o)

CC = clang
CFLAGS = -Wall -Wextra -Werror -Wpedantic
LFLAGS = -lm

.PHONY: all clean

all: $(EXECA) $(EXECB)

$(EXECA): $(OBJECTSA)
	$(CC) $(CFLAGS) $(SOURCEA) -o $(EXECA)

$(EXECB): $(OBJECTSB)
	$(CC) $(CFLAGS) $(SOURCEB) -o $(EXECB)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $< $(CFLAGS)

clean:
	rm -rf $(EXECA) $(EXECB) $(OBJECTSA) $(OBJECTSB) 

scan-build: clean
	scan-build make 