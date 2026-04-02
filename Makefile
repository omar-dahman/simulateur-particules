SRCDIR = src
OBJDIR = obj
BINDIR = bin
HEADERSDIR = headers
IMAGESDIR = output

OPTIONS = -Wall -Wextra -std=c99 -I$(HEADERSDIR)
CC = gcc
EXECUTABLE = simparticles
# evaluation executable
EVAL_EXEC = evaluate

ALL_HEADERS = $(wildcard $(HEADERSDIR)/*.h)
ALL_SOURCES = $(wildcard $(SRCDIR)/*.c)
ALL_OBJECTS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(ALL_SOURCES))
# objects needed for the evaluation (exclude main.o)
EVAL_OBJECTS = $(filter-out $(OBJDIR)/main.o, $(ALL_OBJECTS))
# simulation sources (exclude experiments.c and optimization.c)
SIM_SOURCES = $(filter-out $(SRCDIR)/experiments.c $(SRCDIR)/optimization.c, $(ALL_SOURCES))
# simulation objects (exclude experiments.o and optimization.o)
SIM_OBJECTS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SIM_SOURCES))

# Variable to modify to enable or disable debug : leave empty or add -g
DEBUG = -g
# Variable to add library if needed, like maths with -lm
LIBRARIES = -lm

.PHONY: all clean mrproper build run valgrind docs evaluate run-eval valgrind-eval archive

all: build run

clean:
	@rm -rf $(OBJDIR)/*
	@find docs/ -mindepth 1 ! -name 'Doxyfile' -exec rm -rf {} +

mrproper:
	@rm -rf $(BINDIR)/*

build:
	@echo "Compiling..."
	@make $(BINDIR)/$(EXECUTABLE)

run: $(BINDIR)/$(EXECUTABLE)
	@mkdir -p $(IMAGESDIR)
	@./$(BINDIR)/$(EXECUTABLE)

valgrind: $(BINDIR)/$(EXECUTABLE)
	valgrind --leak-check=full --track-origins=yes ./$(BINDIR)/$(EXECUTABLE)

docs:
	cd docs && doxygen Doxyfile

evaluate:
	@echo "Compiling evaluation..."
	@make $(BINDIR)/$(EVAL_EXEC)

run-eval: $(BINDIR)/$(EVAL_EXEC)
	@mkdir -p $(IMAGESDIR)
	@./$(BINDIR)/$(EVAL_EXEC)

valgrind-eval: $(BINDIR)/$(EVAL_EXEC)
	valgrind --leak-check=full --track-origins=yes ./$(BINDIR)/$(EVAL_EXEC)

archive:
	@echo "Creating archive..."
	@tar -czf research_22.tar.gz -C $(SRCDIR) optimization.c -C ../$(HEADERSDIR) optimization.h -C .. article.pdf

$(BINDIR)/$(EVAL_EXEC): $(EVAL_OBJECTS)
	@mkdir -p $(BINDIR)
	$(CC) $(OPTIONS) $(DEBUG) -o $@ $^ $(LIBRARIES)

$(BINDIR)/$(EXECUTABLE) : $(SIM_OBJECTS)
	@mkdir -p $(BINDIR)
	$(CC) $(OPTIONS) $(DEBUG) -o $@ $^ $(LIBRARIES)

$(OBJDIR)/%.o : $(SRCDIR)/%.c $(ALL_HEADERS)
	@mkdir -p $(OBJDIR)
	$(CC) $(OPTIONS) $(DEBUG) -c -o $@ $<