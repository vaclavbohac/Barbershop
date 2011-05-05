all: tools.o help.o args.o

INC = ../

tools.o: tools.c tools.h
	cc -c $< -o ../$@

help.o: help.c help.h
	cc -c $< -o ../$@

args.o: procargs.c procargs.h
	cc -I$(INC) -c $< -o ../$@

clean:
	-rm ../tools.o
	-rm ../help.o
	-rm ../args.o
