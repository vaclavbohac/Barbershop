SRV_PROC_DIR = ../../server-processes/

BIN = test_builder

FLAGS = -Wall

FLAGS += -lgtest_main -lpthread

FLAGS += -I$(SRV_PROC_DIR)


test: test_builder.cpp builder.o
	g++ -o $(BIN) $(FLAGS) test_builder.cpp builder.o

builder.o: $(SRV_PROC_DIR)messages/builder.c $(SRV_PROC_DIR)messages/builder.h
	g++ -c $(SRV_PROC_DIR)messages/builder.c $(SRV_PROC_DIR)messages/builder.h

clean:
	rm $(BIN) *.o
