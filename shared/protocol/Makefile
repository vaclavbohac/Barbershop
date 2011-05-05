all: server.o request.o response.o process.o

INC = ../

server.o: server.c server.h
	cc $(FLAGS) -I$(INC) -c $< -o ../$@

process.o: process.c process.h
	cc $(FLAGS) -I$(INC) -c $< -o ../$@

request.o: request.c request.h
	cc $(FLAGS) -I$(INC) -c $< -o ../$@

response.o: response.c response.h 
	cc $(FLAGS) -I$(INC) -c $< -o ../$@

clean:
	-rm ../server.o
	-rm ../process.o
	-rm ../request.o
	-rm ../response.o
