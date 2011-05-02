all: server

BIN = server
INC = messages/

server: server.o help.o main.c builder.o process.o tools.o
	cc -o $(BIN) server.o help.o main.c builder.o process.o tools.o

server.o: server.h server.c
	cc -c server.h server.c 

builder.o: $(INC)builder.h $(INC)builder.c
	@(cd $(INC); $(MAKE); mv builder.o ../)

help.o: help.h help.c
	cc -c help.h help.c

tools.o: tools.h tools.c
	cc -c tools.h tools.c

process.o: process.h process.c
	cc -c process.h process.c

clean:
	rm *.o *.gch $(BIN)
	@(cd $(INC); rm *.gch);
