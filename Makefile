CC      = gcc
CXX     = g++
CFLAGS  = -Wall -Wextra -O2
CXXFLAGS= -Wall -Wextra -O2

MAIN_SRC        = ./src/main.c
UNIT_TEST_SRC   = tests/unit/unit-test.cpp

APP_EXE         = app.exe
UNIT_TEST_EXE   = unit-tests.exe

GTEST_LIBS      = -lgtest -lgtest_main -pthread

.PHONY: 
	all clean run-int run-float run-unit-test venv run-integration-tests

all: $(APP_EXE) $(UNIT_TEST_EXE)

$(APP_EXE): $(MAIN_SRC)
	$(CC) $(CFLAGS) $(MAIN_SRC) -o $(APP_EXE)

$(UNIT_TEST_EXE): $(MAIN_SRC) $(UNIT_TEST_SRC)
	mkdir -p build 
	$(CXX) -std=c++14 $(CXXFLAGS) -DUNIT_TEST -I. $(UNIT_TEST_SRC) $(MAIN_SRC) -o $(UNIT_TEST_EXE) $(GTEST_LIBS)

run-int: $(APP_EXE)
	./$(APP_EXE)

run-float: $(APP_EXE)
	./$(APP_EXE) --float

run-unit-test: $(UNIT_TEST_EXE)
	./$(UNIT_TEST_EXE)

format:
	clang-format -i *.c *.h tests/unit/*.cpp

clean:
	rm -f $(APP_EXE) $(UNIT_TEST_EXE)
	rm -rf venv
