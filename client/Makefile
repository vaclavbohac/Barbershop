all: client

BIN = client

INC = ../shared/

FLAGS = -I$(INC)

client: client.o
	cc -o $(BIN) client.o main.c builder.o

client.o: client.h client.c builder.o
	cc $(FLAGS) -c client.h client.c builder.o

builder.o: $(INC)messages/builder.h $(INC)messages/builder.c
	cc $(FLAGS) -c $(INC)messages/builder.h $(INC)messages/builder.c

clean: 
	rm $(BIN) *.o *.gch
