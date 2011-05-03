INC = ../

FLAGS = -I$(INC)

builder.o: types.h special.h builder.h builder.c
	cc $(FLAGS) -c builder.h builder.c special.h types.h

clean:
	rm *.o *.gch
