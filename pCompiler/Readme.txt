COP3402_Spring2020

Team Members:
	
	Owen Brahms
	Michaela Green

Compiling the Compiler

To compile, just run "make" in your terminal from the pCompiler directory, that will compile everything in the
pCompiler directory and provide the compile program.

Running the Compiler

You have several options for running the Compiler:

1. You may run "./compile NAME_OF_INPUT_FILE.txt" from your terminal. This will read in your input file, attempt to compile the program
	and print the results to a file labelled "output.txt".

2. You may run "./compile NAME_OF_INPUT_FILE.txt NAME_OF_OUTPUT_FILE.txt" from your terminal.
	This will read in your input file, attempt to compile the program and print the results to the given output file.
	WARNING: If you give an already existing filename for OUTPUT_FILE.txt, it will get overwritten
	with the program data, so be careful what you use.

3. You may run either of the above with the following commands:
	a. -l: This will print the output of the lexer to the terminal.
	b. -a: This will print the output of the parser to the terminal.
	c. -v: This will print the stack trace of the Virtual Machine to the terminal.
	NOTE: Commands must be input after"./compile" but before entering the name of the input file.

NOTE: All commands must be executed from the pCompiler directory after compilation.