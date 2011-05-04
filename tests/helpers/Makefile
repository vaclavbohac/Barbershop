BIN = ../bin/test_tools

FLAGS = -Wall

FLAGS += -lgtest_main -lpthread

FLAGS += -I../../shared/tools


test: test_tools.cpp ../../shared/tools/tools.c
	g++ -o $(BIN) $(FLAGS) test_tools.cpp ../../shared/tools/tools.c 

clean:
	-rm $(BIN) *.o
