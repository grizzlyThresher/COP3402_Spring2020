SRC = $(wildcard *.c)
OBJ = $(SRC:%.c=%.o)
PROG = lexer
DEPS = $(wildcard *.h)

.PHONY: all clean

all: $(PROG)

$(PROG): $(OBJ)
	$(CC) -o $@ $^

%.o: %.c $(DEPS)
	$(CC) -c $<

clean:
	rm -rf $(PROG) $(OBJ)
