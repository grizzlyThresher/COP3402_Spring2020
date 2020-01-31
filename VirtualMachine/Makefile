FDIR = pMachineSRC
_SRC = pMachine.c
_DEPS = pMachine.h
PROG = pMachine
DEFTEXT = test.txt

DEPS = $(patsubst %,$(FDIR)/%,$(_DEPS))
SRC = $(patsubst %,$(FDIR)/%,$(_SRC))
OBJ = $(SRC:%.c=%.o)
EX = $(patsubst %, ./%, $(PROG))
OUT = output.txt


.PHONY: all clean test run show

all: $(PROG)

$(PROG): $(OBJ)
	$(CC) -o $@ $^

$(FDIR)/%.o: %.c $(DEPS)
	$(CC) -c $<

run: $(EX) $(DEFTEXT)
	$(EX) $(DEFTEXT)

show: $(EX) $(DEFTEXT)
	$(EX) display $(DEFTEXT)

clean:
	rm -rf $(PROG) $(OBJ)