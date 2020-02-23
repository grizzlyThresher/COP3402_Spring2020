# COP3402_Spring2020

## Team Members:
	
	Owen Brahms
	Michaela Green

## Compiling the Virtual Machine

To compile, just run "make" in your terminal from the VirtualMachine directory, that will compile everything in the
pMachineSRC directory and provide the pMachine program.

## Running the Machine

You have several options for running the machine:

1. You may run "./pMachine NAME_OF_INPUT_FILE" from your terminal. This will read in your input file
	and print the results to a file labelled "output.txt".

2. You may run "./pMachine NAME_OF_INPUT_FILE NAME_OF_OUTPUT_FILE" from your terminal.
	This will read in your input file and print the results to the given output file.
	WARNING: If you give an already existing filename for OUTPUT_FILE, it will get overwritten
	with the program data, so be careful what you use.

3. You may run "./pMachine display NAME_OF_INPUT_FILE" from your terminal. This will read in your input
	file and print the results to the terminal.

4. You may run "make run" from your terminal. This will run the machine using the provided "test.txt"
	file and print the results to "output.txt".

5. You may run "make show" from your terminal. This will run the machine using the provided "test.txt"
	file and print the results to the terminal.

NOTE: All commands must be executed from the VirtualMachine directory after compilation.

## Included ISA

NOTE: All SIO commands are mapped to 9. Change test cases accordingly.
NOTE: The result of a logical operation such as A < B is 1 if the case is met
	  and 0 otherwise.

01. LIT R, 0, M   reg[R] <- M

02. RTN 0, 0, 0   sp <- bp - 1
                  bp <- stack[sp + 3]
                  pc <- stack[sp + 4]

03. LOD R, L, M   reg[R] <- stack[base(L, bp) + M]

04. STO R, L, M   stack[base(L, bp) + M] <- R[i]

05. CAL 0, L, M   stack[sp + 1] <- 0
                  stack[sp + 2] <- base(L, bp)
                  stack[sp + 3] <- bp
                  stack[sp + 4] <- pc
                  bp <- sp + 1
                  pc <- M

06. INC 0, 0, M   sp <- sp + M

07. JMP 0, 0, M   pc <- M

08. JPC R, 0, M   if reg[R] == 0, then pc <- M

09. SIO R, 0, 1   print reg[R]

10. SIO R, 0, 2   read input to reg[R]

11. SIO R, 0, 3   set halt flag to one (end of program)

12. NEG           reg[R] <- -reg[R]

13. ADD           reg[R] <- reg[L] + reg[M]

14. SUB           reg[R] <- reg[L] - reg[M]

15. MUL           reg[R] <- reg[L] * reg[M]

16. DIV           reg[R] <- reg[L] * reg[M]

17. ODD           reg[R] <- reg[R] % 2

18. MOD           reg[R] <- reg[L] % reg[M]

19. EQL           reg[R] <- reg[L] == reg[M]

20. NEQ           reg[R] <- reg[L] != reg[M]

21. LSS           reg[R] <- reg[L] < reg[M]

22. LEQ           reg[R] <- reg[L] <= reg[M]

23. GTR           reg[R] <- reg[L] > reg[M]

24. GEQ           reg[R] <- reg[L] >= reg[M]