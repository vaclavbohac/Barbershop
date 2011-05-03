all: server

BIN = server
INC = ../shared/

FLAGS = -I$(INC)
FLAGS += -DDEBUG

server: server.o help.o main.c builder.o process.o tools.o barber.h shared.o sems.o request.o response.o
	cc -o $(BIN) $(FLAGS) server.o help.o main.c builder.o process.o tools.o barber.h shared.o sems.o request.o response.o

server.o: server.h server.c
	cc $(FLAGS) -c server.h server.c 

help.o: help.h help.c
	cc -c help.h help.c

process.o: process.h process.c barber.h
	cc $(FLAGS) -c process.h process.c barber.h

request.o:
	@(cd $(INC)protocol/; make request.o; mv request.o ../../server-processes/);

response.o:
	@(cd $(INC)protocol/; make response.o; mv response.o ../../server-processes/);

sems.o: 
	@(cd $(INC)semaphores/; $(MAKE); mv sems.o ../../server-processes/)

builder.o: 
	@(cd $(INC)messages/; $(MAKE); mv builder.o ../../server-processes/)

shared.o:
	@(cd $(INC)shmemory/; $(MAKE); mv shared.o ../../server-processes/)

tools.o: 
	@(cd $(INC)tools/; $(MAKE); mv tools.o ../../server-processes/)

clean:
	rm *.o *.gch $(BIN)
