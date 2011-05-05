all: client

BIN = ../bin/client
INC = ../shared

CFLAGS = $(FLAGS) -I$(INC)

client: client.o main.o args.o help.o shared
	cc $(CFLAGS) -o $(BIN) client.o main.o args.o help.o ../shared/builder.o ../shared/request.o ../shared/response.o ../shared/tools.o

main.o: main.c
	cc $(CFLAGS) -c $< -o $@

client.o: client.c client.h 
	cc $(CFLAGS) -c $< -o $@

args.o: args.c args.h
	cc $(CFLAGS) -c $<

help.o: help.c help.h
	cc $(CFLAGS) -c $<


shared:
	@(cd $(INC); $(MAKE))

clean: 
	-rm $(BIN) *.o 
