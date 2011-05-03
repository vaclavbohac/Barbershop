all: tools.o

tools.o: tools.c tools.h
	cc -c $< -o ../$@

clean:
	-rm ../tools.o
