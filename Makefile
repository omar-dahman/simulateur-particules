SRCDIR = src
OBJDIR = obj
BINDIR = bin
HEADERSDIR = headers
IMAGESDIR = output

OPTIONS = -Wall -Wextra -std=c99 -I$(HEADERSDIR)
CC = gcc
EXECUTABLE = simparticles

HEADERS = $(wildcard $(HEADERSDIR)/*.h)
OBJECTS_NEEDED = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(wildcard $(SRCDIR)/*.c))

# Variable to modify to enable or disable debug : leave empty or add -g
DEBUG = -g
# Variable to add library if needed, like maths with -lm
LIBRARIES = -lm

.PHONY: all clean mrproper build run valgrind docs

all: build run

clean:
	@rm -rf $(OBJDIR)/*
	@rm -rf $(IMAGESDIR)/*
	@find docs/ -mindepth 1 ! -name 'Doxyfile' -exec rm -rf {} +
	@echo "Cleaning done"

mrproper:
	@rm -rf $(BINDIR)/*

build:
	@echo "Compiling simulation..."
	@make $(BINDIR)/$(EXECUTABLE)

run: $(BINDIR)/$(EXECUTABLE)
	@mkdir -p $(IMAGESDIR)
	@./$(BINDIR)/$(EXECUTABLE)

valgrind: $(BINDIR)/$(EXECUTABLE)
	@echo "Memory check on simulation..."
	valgrind --leak-check=full --track-origins=yes ./$(BINDIR)/$(EXECUTABLE)

docs:
	cd docs && doxygen Doxyfile

$(BINDIR)/$(EXECUTABLE) : $(OBJECTS_NEEDED)
	@mkdir -p $(BINDIR)
	$(CC) $(OPTIONS) $(DEBUG) -o $@ $^ $(LIBRARIES)

$(OBJDIR)/%.o : $(SRCDIR)/%.c $(HEADERS)
	@mkdir -p $(OBJDIR)
	$(CC) $(OPTIONS) $(DEBUG) -c -o $@ $<