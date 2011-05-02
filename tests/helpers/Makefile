SRV_PROC_DIR = ../../server-processes/

BIN = test_tools

FLAGS = -Wall

FLAGS += -lgtest_main -lpthread

FLAGS += -I$(SRV_PROC_DIR)


test: test_tools.cpp tools.o
	g++ -o $(BIN) $(FLAGS) test_tools.cpp tools.o

tools.o: $(SRV_PROC_DIR)tools.c $(SRV_PROC_DIR)tools.h
	g++ -c $(SRV_PROC_DIR)tools.c $(SRV_PROC_DIR)tools.h

clean:
	rm $(BIN) *.o
