all: server

BIN = ../bin/server
INC = ../shared

CFLAGS = $(FLAGS) -I$(INC)

server: main.o server.o shared
	cc $(CFLAGS) -o $(BIN) $< server.o ../shared/server.o ../shared/args.o ../shared/help.o ../shared/builder.o  ../shared/tools.o ../shared/shared.o ../shared/sems.o ../shared/request.o ../shared/response.o ../shared/process.o

main.o: main.c
	cc $(CFLAGS) -c $<

server.o: server.c ../shared/protocol/server.h
	cc $(CFLAGS) -c $< 

shared:
	@(cd $(INC); $(MAKE))

clean:
	-rm *.o $(BIN)
