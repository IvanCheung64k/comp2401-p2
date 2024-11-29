OBJ = main.o event.o manager.o resource.o system.o
OPTS = -g -Wall -Wextra -pthread

all: $(OBJ)
	gcc $(OPTS) -o p2 $(OBJ) 

main.o: main.c defs.h
	gcc $(OPTS) -c main.c 

event.o: event.c defs.h
	gcc $(OPTS) -c event.c

manager.o: manager.c defs.h
	gcc $(OPTS) -c manager.c 

resource.o: resource.c defs.h
	gcc $(OPTS) -c resource.c 

system.o: system.c defs.h
	gcc $(OPTS) -c system.c 

clean: 
	rm -f $(OBJ) p2