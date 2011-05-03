all: shared.o

shared.o: shared.c shared.h
	cc -c $< -o ../$@

clean:
	-rm ../shared.o
