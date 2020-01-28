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
OUT = output.txt


.PHONY: all clean test run

all: $(PROG)


$(PROG): $(OBJ)
	$(CC) -o $@ $^


$(FDIR)/%.o: %.c $(DEPS)
	$(CC) -c $<

view: $(EX)
	$(EX) $(TEST)

print: $(OUT)
	$(EX) $(TEST) > $(OUT)

clean:
	rm -rf $(PROG) $(OBJ)