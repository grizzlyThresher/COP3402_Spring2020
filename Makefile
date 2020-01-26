FDIR = pMachineSRC
_SRC = pMachine.c
_DEPS = pMachine.h
PROG = pMachine

DEPS = $(patsubst %,$(FDIR)/%,$(_DEPS))
SRC = $(patsubst %,$(FDIR)/%,$(_SRC))
OBJ = $(SRC:%.c=%.o)


.PHONY: all clean

all: $(PROG)


$(PROG): $(OBJ)
	$(CC) -o $@ $^


%.o: %.c $(DEPS)
	$(CC) -c $<


clean:
	rm -rf $(PROG) $(OBJ)