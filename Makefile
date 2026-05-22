SRCDIR = src
OBJDIR = obj
BINDIR = bin
HEADERSDIR = headers
OUTPUT_DIR = output
RESULTS_DIR = results
REPRODUCTION_DIR = reproduction
TESTER_EXEC = tester

OPTIONS = -Wall -Wextra -std=c99 -I$(HEADERSDIR) -I$(REPRODUCTION_DIR)
CC = gcc
EXECUTABLE = simparticles
# evaluation executable
EVAL_EXEC = evaluate

ALL_HEADERS = $(wildcard $(HEADERSDIR)/*.h $(REPRODUCTION_DIR)/*.h)
ALL_SOURCES = $(wildcard $(SRCDIR)/*.c $(REPRODUCTION_DIR)/*.c)
ALL_OBJECTS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(filter-out $(SRCDIR)/optimization.c,$(filter $(SRCDIR)/%.c,$(ALL_SOURCES)))) $(patsubst $(REPRODUCTION_DIR)/%.c,$(OBJDIR)/%.o,$(filter $(REPRODUCTION_DIR)/%.c,$(ALL_SOURCES)))# objects needed for the evaluation (exclude main.o)
EVAL_OBJECTS = $(filter-out $(OBJDIR)/main.o, $(ALL_OBJECTS))
# simulation sources (exclude experiments.c and optimization.c)
SIM_SOURCES = $(filter-out $(SRCDIR)/experiments.c $(SRCDIR)/optimization.c, $(ALL_SOURCES))
# simulation objects (exclude experiments.o and optimization.o)
SIM_OBJECTS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SIM_SOURCES))
# reproduction test objects
TESTER_OBJECTS = $(filter-out $(OBJDIR)/main.o $(OBJDIR)/experiments.o $(OBJDIR)/optimization.o, $(ALL_OBJECTS)) $(OBJDIR)/reproduction.o $(OBJDIR)/optimization.o

# Variable to modify to enable or disable debug : leave empty or add -g
DEBUG = -g
# Variable to add library if needed, like maths with -lm
LIBRARIES = -lm

.PHONY: all clean mrproper build run valgrind docs build-eval run-eval valgrind-eval archive

all: build run

clean:
	@rm -rf $(OBJDIR)/*
	@rm -rf $(OUTPUT_DIR)/*
	@rm -rf $(RESULTS_DIR)/*.csv
	@rm -f research_22.tar.gz
	@find docs/ -mindepth 1 ! -name 'Doxyfile' -exec rm -rf {} +

mrproper:
	@rm -rf $(BINDIR)/*

build:
	@echo "Compiling..."
	@make $(BINDIR)/$(EXECUTABLE)

run: $(BINDIR)/$(EXECUTABLE)
	@mkdir -p $(OUTPUT_DIR)
	@./$(BINDIR)/$(EXECUTABLE)

valgrind: $(BINDIR)/$(EXECUTABLE)
	valgrind --leak-check=full --track-origins=yes ./$(BINDIR)/$(EXECUTABLE)

docs:
	cd docs && doxygen Doxyfile

build-eval:
	@echo "Compiling evaluation..."
	@make $(BINDIR)/$(EVAL_EXEC)
	@echo "Evaluation compiled successfully..."

run-eval: $(BINDIR)/$(EVAL_EXEC)
	@echo "Running evaluation..."
	@mkdir -p $(OUTPUT_DIR)
	@mkdir -p $(RESULTS_DIR)
	@./$(BINDIR)/$(EVAL_EXEC)

valgrind-eval: $(BINDIR)/$(EVAL_EXEC)
	valgrind --leak-check=full --track-origins=yes ./$(BINDIR)/$(EVAL_EXEC)

archive:
	@echo "Creating archive research_22.tar.gz ..."
	@tar -czf research_22.tar.gz $(SRCDIR)/optimization.c $(HEADERSDIR)/optimization.h article.pdf
	@echo "Archive created : research_22.tar.gz"

build-tester: 
	@echo "Compiling tester..."
	@mkdir -p $(BINDIR)
	@make $(BINDIR)/$(TESTER_EXEC)

run-tester: $(BINDIR)/$(TESTER_EXEC)
	@mkdir -p $(OUTPUT_DIR)
	@./$(BINDIR)/$(TESTER_EXEC)


$(BINDIR)/$(TESTER_EXEC): $(TESTER_OBJECTS)
	@mkdir -p $(BINDIR)
	$(CC) $(OPTIONS) $(DEBUG) -o $@ $^ $(LIBRARIES)

$(BINDIR)/$(EVAL_EXEC): $(EVAL_OBJECTS)
	@mkdir -p $(BINDIR)
	$(CC) $(OPTIONS) $(DEBUG) -o $@ $^ $(LIBRARIES)

$(BINDIR)/$(EXECUTABLE) : $(SIM_OBJECTS)
	@mkdir -p $(BINDIR)
	$(CC) $(OPTIONS) $(DEBUG) -o $@ $^ $(LIBRARIES)

$(OBJDIR)/reproduction.o : $(REPRODUCTION_DIR)/reproduction.c $(ALL_HEADERS)
	@mkdir -p $(OBJDIR)
	$(CC) $(OPTIONS) $(DEBUG) -c -o $@ $<

$(OBJDIR)/optimization.o : $(REPRODUCTION_DIR)/optimization.c $(ALL_HEADERS)
	@mkdir -p $(OBJDIR)
	$(CC) $(OPTIONS) $(DEBUG) -c -o $@ $<

$(OBJDIR)/%.o : $(SRCDIR)/%.c $(ALL_HEADERS)
	@mkdir -p $(OBJDIR)
	$(CC) $(OPTIONS) $(DEBUG) -c -o $@ $<