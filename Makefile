CC = gcc
CFLAGS = -Wall -g 

CFILES = main.c commands.c errors.c cache.c

HFILES = commands.h errors.h types.h 

EXECUTABLE = tp2

build: $(CFILES)
		$(CC) $(CFLAGS) -o $(EXECUTABLE) $(CFILES)

run_pruebas_todas: build
		./$(EXECUTABLE) prueba1.mem prueba2.mem prueba3.mem prueba3b.mem prueba4.mem prueba5.mem 

run_prueba1: build
		./$(EXECUTABLE) prueba1.mem 

zip: build
	zip $(EXECUTABLE).zip $(CFILES) $(HFILES)

clean:
	rm -f *.o $(EXECUTABLE)