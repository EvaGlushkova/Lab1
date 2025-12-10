
PROJECT = Lab1
TEST_PROJECT = test-Lab1

CXX = g++
CXXFLAGS = -std=c++20 -O3 -pthread -I. -Iparallel
LDFLAGS = -pthread -lm

SRC = main.cpp \
      bmp/bmp_file.cpp \
      bmp/bmp_struct.cpp \
      parallel/bmp_parallel.cpp

OBJ = $(SRC:.cpp=.o)
OBJ_NO_MAIN = $(filter-out main.o,$(OBJ))

TEST_SRC = tests/test.cpp
TEST_OBJ = tests/test.o

.PHONY: all clean cleanall test run_tests

all: $(PROJECT)

$(PROJECT): $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)

test: $(OBJ_NO_MAIN) $(TEST_OBJ)
	$(CXX) -o $(TEST_PROJECT) $^ $(LDFLAGS) -lgtest -lgtest_main

run_tests: test
	./$(TEST_PROJECT)

clean:
	rm -f *.o parallel/*.o bmp/*.o tests/*.o *~ core

cleanall: clean
	rm -f $(PROJECT) $(TEST_PROJECT)
