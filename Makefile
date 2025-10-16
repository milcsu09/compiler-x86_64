
CC := cc

CCFLAGS := -std=c99 -g -Wall -Wextra -Wpedantic
LDFLAGS :=

SRC := $(wildcard src/*.c)
OBJ := $(SRC:.c=.o)

all: $(OBJ)
	$(CC) $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CCFLAGS) -c $< -o $@

clean:
	rm $(OBJ)

