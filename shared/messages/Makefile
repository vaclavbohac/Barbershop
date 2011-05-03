all: builder.o

INC = ../

OUT = ../builder.o

builder.o: builder.c types.h special.h builder.h 
	cc $(FLAGS) -I$(INC) -c $< -o $(OUT)

clean:
	-rm $(OUT)
