all: server

BIN = server
INC = ../shared/

FLAGS = -I$(INC)

server: server.o help.o main.c builder.o process.o tools.o barber.h shared.o
	cc -o $(BIN) $(FLAGS) server.o help.o main.c builder.o process.o tools.o barber.h shared.o

server.o: server.h server.c
	cc $(FLAGS) -c server.h server.c 

help.o: help.h help.c
	cc -c help.h help.c

process.o: process.h process.c barber.h
	cc $(FLAGS) -c process.h process.c barber.h

builder.o: 
	@(cd $(INC)messages/; $(MAKE); mv builder.o ../../server-processes/)

shared.o:
	@(cd $(INC)shmemory/; $(MAKE); mv shared.o ../../server-processes/)

tools.o: 
	@(cd $(INC)tools/; $(MAKE); mv tools.o ../../server-processes/)

clean:
	rm *.o *.gch $(BIN)
	@(cd $(INC); rm *.gch);
