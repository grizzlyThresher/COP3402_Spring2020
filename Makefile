SRC = $(wildcard *.c)
OBJ = $(SRC:%.c=%.o)
PROG = pMachine

.PHONY: all clean

all: $(PROG)

$(PROG): $(OBJ)
	$(CC) -o $@ $^

%.o: %.c
	$(CC) -c $<

clean:
	rm -rf $(PROG) $(OBJ)
