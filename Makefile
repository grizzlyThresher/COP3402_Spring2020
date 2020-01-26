FDIR = pMachineSRC
TDIR = Tests
_SRC = pMachine.c
_DEPS = pMachine.h
_TEST = "test.txt"
PROG = pMachine


DEPS = $(patsubst %,$(FDIR)/%,$(_DEPS))
SRC = $(patsubst %,$(FDIR)/%,$(_SRC))
TEST = $(patsubst %,$(TDIR)/%,$(_TEST))
OBJ = $(SRC:%.c=%.o)
EX = $(patsubst %, ./%, $(PROG))


.PHONY: all clean test

all: $(PROG)


$(PROG): $(OBJ)
	$(CC) -o $@ $^


$(FDIR)/%.o: %.c $(DEPS)
	$(CC) -c $<

test: $(EX)
	$(EX) $(TEST)


clean:
	rm -rf $(PROG) $(OBJ)