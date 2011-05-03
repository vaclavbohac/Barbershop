all: request.o response.o

FLAGS = -I../

FLAGS += -DDEBUG

request.o: request.h request.c
	cc $(FLAGS) -c request.h request.c

response.o: response.h response.c
	cc $(FLAGS) -c response.h response.c

clean:
	rm *.o *.gch
