CC = gcc
CFLAGS = -g

all : SudokuSATSolver


InputToSAT.o: InputToSAT.c 
	$(CC) $(CFLAGS) -c InputToSAT.c
	
SudokuSATSolver : InputToSAT.o 
	$(CC) $(CFLAGS) -o SudokuSATSolver InputToSAT.o

clean :
	rm InputToSAT.o
