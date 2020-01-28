FDIR = pMachineSRC
_SRC = pMachine.c
_DEPS = pMachine.h
PROG = pMachine

DEPS = $(patsubst %,$(FDIR)/%,$(_DEPS))
SRC = $(patsubst %,$(FDIR)/%,$(_SRC))
OBJ = $(SRC:%.c=%.o)
EX = $(patsubst %, ./%, $(PROG))
OUT = output.txt


.PHONY: all clean test run 

all: $(PROG)

$(PROG): $(OBJ)
	$(CC) -o $@ $^

$(FDIR)/%.o: %.c $(DEPS)
	$(CC) -c $<

run: $(EX)
	$(EX)


clean:
	rm -rf $(PROG) $(OBJ)