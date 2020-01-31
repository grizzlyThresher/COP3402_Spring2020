# COP3402_Spring2020

## Team Members:
	
	Owen Brahms
	Michaela Green

## Compiling the Virtual Machine

To compile, just run "make" in your terminal from the VirtualMachine directory, that will automatically compile everything in the
pMachineSRC directory and provide the pMachine program.

## Running the Machine

You have several options for running the machine:

1. You may run "./pMachine INPUT_FILE" from your terminal. This will read in your input file
	and print the results to a file labelled "output.txt".

2. You may run "./pMachine INPUT_FILE OUTPUT_FILE" from your terminal. This will read in your
	input file and print the results to the given output file.
	WARNING: If you give an already existing filename for OUTPUT_FILE, it will get overwritten
	with the program data, so be careful what you use.

3. You may run "./pMachine display INPUT_FILE" from your terminal. This will read in your input
	file and print the results to the terminal.

4. You may run "make run" from your terminal. This will run the machine using the provided "test.txt"
	file and print the results to "output.txt".

5. You may run "make show" from your terminal. This will run the machine using the provided "test.txt"
	file and print the results to the terminal.

