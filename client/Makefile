all: client

BIN = ../bin/client

INC = ../shared

client: client.o main.o shared
	cc $(FLAGS) -I$(INC) -o $(BIN) client.o main.o ../shared/shared.o ../shared/builder.o ../shared/sems.o ../shared/request.o ../shared/response.o ../shared/tools.o

main.o: main.c
	cc $(FLAGS) -I$(INC) -c $< -o $@

client.o: client.c client.h 
	cc $(FLAGS) -I$(INC) -c $< -o $@

shared:
	@(cd $(INC); $(MAKE))

clean: 
	-rm $(BIN) *.o 
