all: barber.o

barber.o: barber.c barber.h
	cc $(FLAGS) -c $< -o ../$@

clean:
	-rm ../barber.o
