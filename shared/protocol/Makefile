all: request.o response.o

INC = ../

request.o: request.c request.h
	cc $(FLAGS) -I$(INC) -c $< -o ../$@

response.o: response.c response.h 
	cc $(FLAGS) -I$(INC) -c $< -o ../$@

clean:
	-rm ../request.o
	-rm ../response.o
