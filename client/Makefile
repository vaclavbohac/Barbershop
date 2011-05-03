all: client

BIN = client

INC = ../shared/

FLAGS = -I$(INC)
FLAGS += -DDEBUG

client: client.o builder.o shared.o sems.o tools.o response.o request.o
	cc $(FLAGS) -o $(BIN) client.o main.c builder.o shared.o sems.o tools.o response.o request.o

client.o: client.h client.c builder.o
	cc $(FLAGS) -c client.h client.c builder.o

builder.o: $(INC)messages/builder.h $(INC)messages/builder.c
	cc $(FLAGS) -c $(INC)messages/builder.h $(INC)messages/builder.c

request.o:
	@(cd $(INC)protocol/; make request.o; mv request.o ../../client/);

response.o:
	@(cd $(INC)protocol/; make response.o; mv response.o ../../client/);

shared.o:
	@(cd $(INC)shmemory/; $(MAKE); mv shared.o ../../client/);

sems.o:
	@(cd $(INC)semaphores/; $(MAKE); mv sems.o ../../client/);

tools.o:
	@(cd $(INC)tools/; $(MAKE); mv tools.o ../../client/);

clean: 
	rm $(BIN) *.o *.gch
