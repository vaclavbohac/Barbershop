all: server

BIN = server
INC = ../shared/

FLAGS = -I$(INC)

server: server.o help.o main.c builder.o process.o tools.o
	cc -o $(BIN) $(FLAGS) server.o help.o main.c builder.o process.o tools.o

server.o: server.h server.c
	cc $(FLAGS) -c server.h server.c 

builder.o: $(INC)messages/builder.h $(INC)messages/builder.c
	@(cd $(INC)messages/; $(MAKE); mv builder.o ../../server-processes/)

help.o: help.h help.c
	cc -c help.h help.c

tools.o: tools.h tools.c
	cc -c tools.h tools.c

process.o: process.h process.c
	cc $(FLAGS) -c process.h process.c

clean:
	rm *.o *.gch $(BIN)
	@(cd $(INC); rm *.gch);
