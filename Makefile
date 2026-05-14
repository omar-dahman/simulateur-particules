SRCDIR = src
OBJDIR = obj
BINDIR = bin
HEADERSDIR = headers
OUTPUT_DIR = output
TESTS_DIR = tests

OPTIONS = -Wall -Wextra -std=c99 -I$(HEADERSDIR) -I$(TESTS_DIR)
CC = gcc
EXECUTABLE = simparticles
TEST_EXEC = test

ALL_HEADERS = $(wildcard $(HEADERSDIR)/*.h) $(wildcard $(TESTS_DIR)/*.h)
# simulation sources and objects
SIM_SOURCES = $(wildcard $(SRCDIR)/*.c)
SIM_OBJECTS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SIM_SOURCES))
SIM_OBJECTS_NO_MAIN = $(filter-out $(OBJDIR)/main.o, $(SIM_OBJECTS))
# tests sources and objects
TEST_SOURCES = $(wildcard $(TESTS_DIR)/*.c)
TEST_OBJECTS = $(patsubst $(TESTS_DIR)/%.c, $(OBJDIR)/%.o, $(TEST_SOURCES))

# Variable to modify to enable or disable debug : leave empty or add -g
DEBUG = -g
# Variable to add library if needed, like maths with -lm
LIBRARIES = -lm -lcunit

.PHONY: all clean mrproper build run valgrind docs build-test run-test valgrind-test

all: build run

clean:
	@rm -rf $(OBJDIR)/*
	@rm -rf $(OUTPUT_DIR)/*
	@find docs/ -mindepth 1 ! -name 'Doxyfile' -exec rm -rf {} +
	@echo "Cleaning done"

mrproper:
	@rm -rf $(BINDIR)/*

build:
	@echo "Compiling simulation..."
	@make $(BINDIR)/$(EXECUTABLE)

run: $(BINDIR)/$(EXECUTABLE)
	@mkdir -p $(OUTPUT_DIR)
	@./$(BINDIR)/$(EXECUTABLE)

valgrind: $(BINDIR)/$(EXECUTABLE)
	@echo "Memory check on simulation..."
	valgrind --leak-check=full --track-origins=yes ./$(BINDIR)/$(EXECUTABLE)

docs:
	cd docs && doxygen Doxyfile

build-test:
	@echo "Compiling tests..."
	@make $(BINDIR)/$(TEST_EXEC)

run-test: $(BINDIR)/$(TEST_EXEC)
	@echo "Running tests..."
	@./$(BINDIR)/$(TEST_EXEC)

valgrind-test: $(BINDIR)/$(TEST_EXEC)
	@echo "Memory check on tests..."
	valgrind --leak-check=full --track-origins=yes ./$(BINDIR)/$(TEST_EXEC)

# === simulation part ===

$(BINDIR)/$(EXECUTABLE) : $(SIM_OBJECTS)
	@mkdir -p $(BINDIR)
	$(CC) $(OPTIONS) $(DEBUG) -o $@ $^ $(LIBRARIES)

$(OBJDIR)/%.o : $(SRCDIR)/%.c $(ALL_HEADERS)
	@mkdir -p $(OBJDIR)
	$(CC) $(OPTIONS) $(DEBUG) -c -o $@ $<

# === tests part ===

$(BINDIR)/$(TEST_EXEC): $(SIM_OBJECTS_NO_MAIN) $(TEST_OBJECTS)
	@mkdir -p $(BINDIR)
	$(CC) $(OPTIONS) $(DEBUG) -o $@ $^ $(LIBRARIES)

$(OBJDIR)/%.o: $(TESTS_DIR)/%.c $(ALL_HEADERS)
	@mkdir -p $(OBJDIR)
	$(CC) $(OPTIONS) $(DEBUG) -c -o $@ $<