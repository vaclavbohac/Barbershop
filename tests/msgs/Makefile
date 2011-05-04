all: test

BIN = ../bin/test_builder

FLAGS = -Wall

FLAGS += -lgtest_main -lpthread

FLAGS += -I../../shared

test: test_builder.cpp ../../shared/messages/builder.c ../../shared/tools/tools.c
	g++ $(FLAGS) -o $(BIN) $^

clean:
	-rm $(BIN) *.o
