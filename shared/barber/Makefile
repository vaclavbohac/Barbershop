all: barber.o

barber.o: barber.c barber.h
	cc -I../ $(FLAGS) -c $< -o ../$@

clean:
	-rm ../barber.o
