CFLAGS = -Wall -Wextra

all: final

final: main.o board.o attacks.o moveArray.o moveGen.o utility.o repl.o
	@echo "Linking"
	gcc $(CFLAGS) main.o board.o attacks.o moveArray.o moveGen.o utility.o repl.o -o final

main.o: main.c
	@echo "Compiling main file..."
	gcc $(CFLAGS) -c main.c

board.o: board.c
	@echo "Compiling board"
	gcc $(CFLAGS) -c board.c

attacks.o: attacks.c
	@echo "Compiling attacks"
	gcc $(CFLAGS) -c attacks.c

moveArray.o: moveArray.c
	@echo "Compiling moveArray"
	gcc $(CFLAGS) -c moveArray.c

moveGen.o: moveGen.c
	@echo "Compiling moveGen"
	gcc $(CFLAGS) -c moveGen.c

utility.o: utility.c
	@echo "Compiling utility"
	gcc $(CFLAGS) -c utility.c

repl.o: repl.c
	@echo "Compiling repl"
	gcc $(CFLAGS) -c repl.c
.PHONY: clean
clean:
	@echo "Cleaning..."
	@rm -f main.o board.o attacks.o moveArray.o moveGen.o utility.o repl.o final