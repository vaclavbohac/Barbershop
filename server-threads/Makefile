all: server

BIN = ../bin/server-threads

CFLAGS = $(FLAGS) -I../shared -lpthread -D__USE_REENTRANT

server: main.o server.o shared
	cc $(CFLAGS) -o $(BIN) $< server.o ../shared/server.o ../shared/args.o ../shared/help.o

main.o: main.c
	cc $(CFLAGS) -c $<

server.o: server.c
	cc $(CFLAGS) -c $<

shared:
	@(cd ../shared; $(MAKE) FLAGS="$(CFLAGS)")

clean:
	-rm $(BIN) *.o
