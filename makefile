CFLAGS = -Wall -Wextra

all: final

final: main.o board.o attacks.o
	@echo "Linking"
	gcc $(CFLAGS) main.o board.o attacks.o -o final

main.o: main.c
	@echo "Compiling main file..."
	gcc $(CFLAGS) -c main.c

board.o: board.c
	@echo "Compiling board"
	gcc $(CFLAGS) -c board.c

attacks.o: attacks.c
	@echo "Compiling attacks"
	gcc $(CFLAGS) -c attacks.c

.PHONY: clean
clean:
	@echo "Cleaning..."
	@rm -f main.o board.o attacks.o final