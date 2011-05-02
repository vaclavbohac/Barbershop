all: client

BIN = client

INC = ../share/

FLAGS = -I$(INC)

client: client.o
	cc -o $(BIN) client.o main.c

client.o: client.h client.c
	cc -c client.h client.c

builder.o: $(INC)messages/builder.h $(INC)messages/builder.c
	cc $(FLAGS) -c $(INC)messages/builder.h $(INC)messages/builder.c

clean: 
	rm $(BIN) *.o *.gch
