SRC = $(wildcard *.c)
OBJ = $(SRC:%.c=%.o)
PROG = compile
DEPS = $(wildcard *.h)
EXEC = ./compile

.PHONY: all clean

all: $(PROG)

$(PROG): $(OBJ)
	$(CC) -o $@ $^

%.o: %.c $(DEPS)
	$(CC) -c $<

clean:
	rm -rf $(PROG) $(OBJ)
