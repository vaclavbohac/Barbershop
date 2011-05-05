all: server

BIN = ../bin/server
INC = ../shared

CFLAGS = $(FLAGS) -I$(INC)

server: server.o process.o main.o shared 
	cc $(CFLAGS) -o $(BIN) server.o main.o process.o ../shared/args.o ../shared/help.o ../shared/builder.o  ../shared/tools.o ../shared/shared.o ../shared/sems.o ../shared/request.o ../shared/response.o

main.o: main.c barber.h
	cc $(CFLAGS) -c $<

server.o: server.c server.h 
	cc $(CFLAGS) -c $< 

process.o: process.c process.h  barber.h
	cc $(CFLAGS) -c $<

shared:
	@(cd $(INC); $(MAKE))

clean:
	-rm *.o $(BIN)
