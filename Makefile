# Компиляторы и флаги
CC      = gcc
CXX     = g++
CFLAGS  = -Wall -Wextra -Wpedantic -Werror -std=c11
CXXFLAGS= -Wall -Wextra -Wpedantic -Werror -std=c++17

# Каталоги
BUILD_DIR             = build
SRC_DIR               = src
UNIT_TEST_DIR         = tests/unit
INTEGRATION_TEST_DIR  = tests/integration

# Исходники приложения
MAIN_SRC      = $(SRC_DIR)/main.c

# Исполняемый файл приложения
APP_EXE       = $(BUILD_DIR)/app.exe

# Исходник юнит-тестов
UNIT_TEST_SRC = $(UNIT_TEST_DIR)/unit-test.cpp
# Объектный файл юнит-тестов
UNIT_TEST_OBJ = $(BUILD_DIR)/unit-test.o
# Исполняемый файл юнит-тестов
UNIT_TEST_EXE = $(BUILD_DIR)/unit-tests.exe

# Линковка для тестов (GoogleTest)
GTEST_LIBS        = -lgtest -lgtest_main -pthread

# Виртуальное окружение для интеграционных тестов
VENV    = venv
PYTHON  = $(VENV)/bin/python
PIP     = $(VENV)/bin/pip

.PHONY: all clean run-int run-float run-unit-test run-integration-tests venv

all: $(APP_EXE) $(UNIT_TEST_EXE)

# Создание каталога build
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Сборка приложения
$(APP_EXE): $(MAIN_SRC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $(MAIN_SRC) -o $(BUILD_DIR)/main.o
	$(CC) $(CFLAGS) $(BUILD_DIR)/main.o -o $(APP_EXE) -lstdc++ -lm

# Сборка юнит-тестов (файл unit-test.cpp уже включает src/main.c с UNIT_TEST)
$(UNIT_TEST_OBJ): $(UNIT_TEST_SRC) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -DUNIT_TEST -I. -c $(UNIT_TEST_SRC) -o $(UNIT_TEST_OBJ)

$(UNIT_TEST_EXE): $(UNIT_TEST_OBJ) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(UNIT_TEST_OBJ) -o $(UNIT_TEST_EXE) $(GTEST_LIBS)

# Цель для создания виртуального окружения и установки pytest
venv:
	python3 -m venv $(VENV)
	$(PIP) install --upgrade pip
	$(PIP) install pytest

# Запуск приложения (int режим)
run-int: $(APP_EXE)
	./$(APP_EXE)

# Запуск приложения (float режим)
run-float: $(APP_EXE)
	./$(APP_EXE) --float

# Запуск юнит-тестов
run-unit-test: $(UNIT_TEST_EXE)
	./$(UNIT_TEST_EXE)

# Запуск интеграционных тестов (например, через pytest)
run-integration-tests: $(APP_EXE)
	APP_PATH=$(abspath $(APP_EXE)) $(PYTHON) -m pytest -v $(INTEGRATION_TEST_DIR)

clean:
	rm -rf $(BUILD_DIR) $(VENV)
