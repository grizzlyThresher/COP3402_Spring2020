FDIR = pMachineSRC
_SRC = pMachine.c
_DEPS = pMachine.h
PROG = pMachine
TEST = "test.txt"

DEPS = $(patsubst %,$(FDIR)/%,$(_DEPS))
SRC = $(patsubst %,$(FDIR)/%,$(_SRC))
OBJ = $(SRC:%.c=%.o)
EX = $(patsubst %, ./%, $(PROG))


.PHONY: all clean test

all: $(PROG)


$(PROG): $(OBJ)
	$(CC) -o $@ $^


%.o: %.c $(DEPS)
	$(CC) -c $<

test: $(EX)
	$(EX) $(TEST)


clean:
	rm -rf $(PROG) $(OBJ)