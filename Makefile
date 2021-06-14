EXECA = encode
EXECB = decode
EXECC = entropy
EXECD = error

SOURCES_A = encode.c hamming.c bm.c bv.c
OBJECTS_A = $(SOURCES_A:%.c=%.o)

SOURCES_B = decode.c hamming.c bm.c bv.c
OBJECTS_B = $(SOURCES_B:%.c=%.o)

SOURCES_C = entropy.c 
OBJECTS_C = $(SOURCES_C:%.c=%.o)

SOURCES_D = error.c
OBJECTS_D = $(SOURCES_D:%.c=%.o)

CC = clang
CFLAGS = -Wall -Wextra -Werror -Wpedantic
LFLAGS = -lm

.PHONY: all clean format

all: $(EXECA) $(EXECB) $(EXECC) $(EXECD) 

$(EXECA): $(OBJECTS_A)
	$(CC) -o $@ $^ $(LFLAGS)

$(EXECB): $(OBJECTS_B)
	$(CC) -o $@ $^ $(LFLAGS)

$(EXECC): $(OBJECTS_C)
	$(CC) -o $@ $^ $(LFLAGS)

$(EXECD): $(OBJECTS_D)
	$(CC) -o $@ $^ $(LFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

format:
	clang-format -i -style=file *.[ch]

clean:
	rm -rf $(EXECA) $(EXECB) $(EXECC) $(EXECD) $(OBJECTS_A) $(OBJECTS_B) $(OBJECTS_C) $(OBJECTS_D) 

scan-build: clean
	scan-build make 
