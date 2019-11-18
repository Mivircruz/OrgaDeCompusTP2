CC = gcc
CFLAGS = -Wall -g 

CFILES = main.c commands.c errors.c cache.c

HFILES = commands.h errors.h types.h 

EXECUTABLE = tp2

build: $(CFILES)
		$(CC) $(CFLAGS) -o $(EXECUTABLE) $(CFILES)

run: build
		./$(EXECUTABLE) 

gdb: build
	gdb ./$(EXECUTABLE)
	
zip: build
	zip $(EXECUTABLE).zip $(CFILES) $(HFILES)

clean:
	rm -f *.o $(EXECUTABLE)